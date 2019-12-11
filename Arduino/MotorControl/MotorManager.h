#ifndef MOTORMANAGER_H
#define MOTORMANAGER_H

#include "Motor.h"
#include "MotorConfig.h"

class MotorManager
{
  public:
    Motor motors[NUM_MOTORS];
    MotorConfig config;

    bool mode2D = false;

    double curPos[2];
    double curSpeed[2];

    double targetPos[2];

    double prevTime;

    double force = 1;
    double friction = 2;

    int dbgLimiter;

    MotorManager()
    {

    }

    void init()
    {
      DBG("Motor Manager init");
      for (int i = 0; i < NUM_MOTORS; i++) motors[i].init(i, config.uartRxPins[i], config.uartTxPins[i], config.uartTxEnPins[i], config.serialPorts[i], config.powerStageParams[i], config.motorParams[i]);

      mode2D = true;

      
      setAcceleration(preferences.getFloat("acceleration",1), false); //default value
      setMaxSpeed(preferences.getFloat("maxSpeed",1), false); //default value

      setup(preferences.getFloat("spoolRadius",0.085f),
            preferences.getFloat("tx1",-3),
            preferences.getFloat("ty1",2),
            preferences.getFloat("offset1X",0),
            preferences.getFloat("offset1Y",0),
            preferences.getBool("invert1",false),
            preferences.getFloat("tx2",3),
            preferences.getFloat("ty2",2),
            preferences.getFloat("offset2X",0),
            preferences.getFloat("offset2Y",0),
            preferences.getBool("invert2",false),
            false
           );

      setPhysics(preferences.getFloat("force",1),preferences.getFloat("friction",2), false);
     
    }

    void setup(float spoolRadius, float tx1, float ty1, float offset1X, float offset1Y, bool invert1, float tx2, float ty2, float offset2X, float offset2Y, bool invert2, bool save = true)
    {
      DBG("Setup : "+String(spoolRadius)+" /  "+String(tx1)+","+String(ty1)+":"+String(offset1X)+","+String(offset1Y)+":"+String(invert1)+" / "+String(tx2)+","+String(ty2)+":"+String(offset2X)+","+String(offset2Y)+":"+String(invert2));

      
      motors[0].setup(spoolRadius, tx1, ty1, offset1X, offset1Y, invert1);
      motors[1].setup(spoolRadius, tx2, ty2, offset2X, offset2Y, invert2);

      if(save)
      {
        preferences.putFloat("spoolRadius",spoolRadius);
        preferences.putFloat("tx1",tx1);
        preferences.putFloat("ty1",ty1);
        preferences.putFloat("offset1X",offset1X);
        preferences.putFloat("offset1Y",offset1Y);
        preferences.putBool("invert1",invert1);
        preferences.putFloat("tx2",tx2);
        preferences.putFloat("ty2",ty2);
        preferences.putFloat("offset2X",offset2X);
        preferences.putFloat("offset2Y",offset2Y);
        preferences.putBool("invert2",invert2);
      }
      
    }

   

    void update()
    {
      if (mode2D)
      {
        processMotion();
        computeDistances();
      }

      if (dbgLimiter++ == 0) 
      {
        DBG("Target2D : "+String(targetPos[0])+" / "+String(targetPos[1])+"\t Cur pos : "+String(curPos[0])+","+String(curPos[1])+" / Motors : " + String(motors[0].targetPosition) + "\t" + String(motors[1].targetPosition));
      }
      
      if (dbgLimiter > 100) dbgLimiter = 0;
    }

    void processMotion()
    {
      double curTime = micros() / 1e6;
      double deltaTime = curTime - prevTime;

      for (int i = 0; i < 2; i++)
      {
        double targetAcc = (targetPos[i] - curPos[i]) * force;
        targetAcc -= curSpeed[i] * friction;

        curSpeed[i] += targetAcc * deltaTime;
        curPos[i] += curSpeed[i] * deltaTime;
      }

      prevTime = curTime;
    }


    void computeDistances()
    {
      for (int i = 0; i < NUM_MOTORS; i++)
      {
        double txWithOffset = curPos[0] + motors[i].targetOffsetX;
        double tyWithOffset = curPos[1] + motors[i].targetOffsetY;
        double dist = getPointDistance(motors[i].x, motors[i].y, txWithOffset, tyWithOffset);
        motors[i].setTargetPosition(dist);
      }

      
    }


    void setAcceleration(float value, bool save = true)
    {
      DBG("Acceleration : "+String(value));
      if(save) preferences.putFloat("acceleration",value);
      for (int i = 0; i < NUM_MOTORS; i++) motors[i].setAcceleration(value);
    }

    void setMaxSpeed(float value, bool save = true)
    {
     DBG("Max Speed : "+String(value));
     if(save) preferences.putFloat("maxSpeed",value);
      for (int i = 0; i < NUM_MOTORS; i++) motors[i].setMaxSpeed(value);
    }

     void setPhysics(float _force, float _friction, bool save = true)
    {
      force = _force;
      friction = _friction;
      
      DBG("Set Physics "+String(force)+", "+String(friction));
     
       if(save)
      {
        preferences.putFloat("force",force);
        preferences.putFloat("friction",friction);
      }
    }
    
    void setTargetPosition2D(float tx, float ty)
    {
      mode2D = true;
      targetPos[0] = tx;
      targetPos[1] = ty;
    }


#if USE_OSC
    bool handleMessage(OSCMessage &msg)
    {

      int offset = msg.match("/motor");
      if (offset == 0) return false;

      if (msg.match("/setAcceleration", offset))
      {
        if (msg.isFloat(0))
        {
          setAcceleration(msg.getFloat(0));
        } else
        {
          DBG("MotorManager :: Acceleration message 1st argument should be float");
        }
      } else if (msg.match("/setMaxSpeed", offset))
      {
        if (msg.isFloat(0))
        {
          setMaxSpeed(msg.getFloat(0));
        } else
        {
          DBG("MotorManager :: Max speed message 1st argument should be float");
        }
      } else if (msg.match("/setPositions", offset))
      {
        for (int i = 0; i < NUM_MOTORS && i < msg.size(); i++)
        {
          if (msg.isFloat(i)) motors[i].setTargetPosition(msg.getFloat(i));
          else {
            DBG("MotorManager :: Positions message arguments should be float");
          }
        }
      } else if (msg.match("/setPosition2D", offset))
      {
        if (msg.size() >= 2 && msg.isFloat(0) && msg.isFloat(1)) setTargetPosition2D(msg.getFloat(0), msg.getFloat(1));
      } else if (msg.match("/setPhysics", offset))
      {
        if (msg.size() >= 2 && msg.isFloat(0) && msg.isFloat(1)) 
        {
          force = msg.getFloat(0);
          friction = msg.getFloat(1);
        }
      } else if (msg.match("/setPositionMode", offset))
      {
        for (int i = 0; i < NUM_MOTORS && i < msg.size(); i++) motors[i].setControlMode(true);
      } else if (msg.match("/setVelocityMode", offset))
      {
        for (int i = 0; i < NUM_MOTORS && i < msg.size(); i++)  motors[i].setControlMode(false);
      } else if (msg.isInt(0))
      {
        int index = msg.getInt(0);
        if (index >= 0 && index < NUM_MOTORS) motors[index].handleMessage(msg, offset);
      } else
      {

        char addr[64];
        msg.getAddress(addr);
        DBG("Message not handled : " + String(addr));
      }

    }
#endif

#if USE_SERIAL
    bool handleSerialMessage(char command, String * values, int numValues)
    {
      switch (command)
      {
        case 'a':
          setAcceleration(values[0].toFloat());
          break;

        case 's':
          setMaxSpeed(values[0].toFloat());
          break;


        case 'p':
          {
            int index = values[0].toInt();
            if (index >= 0 && index < NUM_MOTORS) motors[index].setTargetPosition(values[1].toFloat());
          }
          break;

        case 'P':
          motors[0].setTargetPosition(values[0].toFloat());
          motors[1].setTargetPosition(values[1].toFloat());
          break;

        case 'r':
          motors[0].resetPosition(numValues >= 1?values[0].toFloat():0);
          motors[1].resetPosition(numValues >= 2?values[1].toFloat():(numValues >= 1?values[0].toFloat():0));
          break;

        case 't':
          setTargetPosition2D(values[0].toFloat(), values[1].toFloat());
          break;

        case 'S':
          DBG("We are here");
          setup(values[0].toFloat(), values[1].toFloat(), values[2].toFloat(), values[3].toFloat(), values[4].toFloat(), values[5].toInt() > 0, values[6].toFloat(), values[7].toFloat(), values[8].toFloat(), values[9].toFloat(), values[10].toInt() > 0);
          break;

        case 'f':
          setPhysics(values[0].toFloat(), values[1].toFloat());
          break;
        
        default:
          return false;
      }

      return true;
    }
#endif



    //Helpers

    double getPointDistance(double p1x, double p1y, double p2x, double p2y)
    {
      return sqrt(pow(p2x - p1x, 2) + pow(p2y - p1y, 2));
    }

    /*
      int findCircleCircleIntersections(
      double c0x, double c0y, float radius0,
      double c1x, double c1y, float radius1,
      double & p1x, double & p1y, double &p2x, double &p2y)
      {
      // Find the distance between the centers.
      float dx = c0x - c1x;
      float dy = c0y - c1y;
      double dist = Mathf.Sqrt(dx * dx + dy * dy);

      // See how many solutions there are.
      if (dist > radius0 + radius1)
      {
        // No solutions, the circles are too far apart.
        // intersection1 = new Vector2(float.NaN, float.NaN);
        //intersection2 = new Vector2(float.NaN, float.NaN);
        return 0;
      }
      else if (dist < Mathf.Abs(radius0 - radius1))
      {
        // No solutions, one circle contains the other.
        //intersection1 = new Vector2(float.NaN, float.NaN);
        //intersection2 = new Vector2(float.NaN, float.NaN);
        return 0;
      }
      else if ((dist == 0) && (radius0 == radius1))
      {
        // No solutions, the circles coincide.
        // intersection1 = new Vector2(float.NaN, float.NaN);
        // intersection2 = new Vector2(float.NaN, float.NaN);
        return 0;
      }
      else
      {
        // Find a and h.
        double a = (radius0 * radius0 -
                    radius1 * radius1 + dist * dist) / (2 * dist);
        double h = Mathf.Sqrt((float)(radius0 * radius0 - a * a));

        // Find P2.
        double cx2 = c0x + a * (c1x - c0x) / dist;
        double cy2 = c0y + a * (c1y - c0y) / dist;

        // Get the points P3.
        p1x = (float)(cx2 + h * (c1y - c0y) / dist);
        p1y = (float)(cy2 - h * (c1x - c0x) / dist);
        p2x = (cx2 - h * (c1y - c0y) / dist);
        p2y = (float)(cy2 + h * (c1x - c0x) / dist);

        // See if we have 1 or 2 solutions.
        if (dist == radius0 + radius1) return 1;
        return 2;
      }

      }
    */
};


#endif
