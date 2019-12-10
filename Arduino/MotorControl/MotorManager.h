#ifndef MOTORMANAGER_H
#define MOTORMANAGER_H

#include "Motor.h"
#include "MotorConfig.h"

class MotorManager
{
public:
  Motor motors[NUM_MOTORS];
  MotorConfig config;
  MotorManager()
  {
    
  }

  void init()
  {
    for(int i=0;i<NUM_MOTORS;i++) motors[i].init(i, config.uartRxPins[i], config.uartTxPins[i],config.uartTxEnPins[i],config.serialPorts[i],config.powerStageParams[i],config.motorParams[i]);
  }

  void update()
  {
    for(int i=0;i<NUM_MOTORS;i++) motors[i].update();
  }

#if USE_OSC
  bool handleMessage(OSCMessage &msg)
  {
    
    int offset = msg.match("/motor");
    if(offset == 0) return false;

    if(msg.match("/setAcceleration", offset))
    {
      if(msg.isFloat(0))
      {
        for(int i=0;i<NUM_MOTORS;i++) motors[i].setAcceleration(msg.getFloat(0));
      }else
      {
        DBG("MotorManager :: Acceleration message 1st argument should be float");
      }
    }else if(msg.match("/setMaxSpeed", offset))
    {
      if(msg.isFloat(0))
      {
        for(int i=0;i<NUM_MOTORS;i++) motors[i].setMaxSpeed(msg.getFloat(0));
      }else
      {
        DBG("MotorManager :: Max speed message 1st argument should be float");
      }
    }else if(msg.match("/setPositions",offset))
    {
      for(int i=0;i<NUM_MOTORS && i < msg.size();i++)
      {
        if(msg.isFloat(i)) motors[i].setTargetPosition(msg.getFloat(i));
        else {
          DBG("MotorManager :: Positions message arguments should be float");
        }
      }
    }else if(msg.match("/setPositionMode",offset))
    {
      for(int i=0;i<NUM_MOTORS && i < msg.size();i++) motors[i].setControlMode(true);
    }else if(msg.match("/setVelocityMode",offset))
    {
      for(int i=0;i<NUM_MOTORS && i < msg.size();i++)  motors[i].setControlMode(false);
    }else if(msg.isInt(0))
    {
      int index = msg.getInt(0); 
      if(index >= 0 && index < NUM_MOTORS) motors[index].handleMessage(msg, offset);
    }else
    {
      
      char addr[64];
      msg.getAddress(addr);
      DBG("Message not handled : "+ String(addr));
    }
    
  }
#endif
};


#endif
