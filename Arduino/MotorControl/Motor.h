#ifndef MOTOR_H
#define MOTOR_H

#include <TMC5160.h>
#include "OSCMessage.h"

class Motor
{
public:
  TMC5160_UART_Transceiver motor;
  
  int index;
  double spoolDiameter = 0.085; //in meters
  
  double x;
  double y;

  double targetOffsetX;
  double targetOffsetY;

  bool invert;

  float targetPosition;
  
  Motor()
  {
    targetPosition = 0;
    invert = false;
    x = 0;
    y = 0;
    targetOffsetX = 0;
    targetOffsetY = 0;
  }

  void init(int index, uint8_t rxPin, uint8_t txPin, uint8_t txEnPin, HardwareSerial * port, TMC5160::PowerStageParameters powerStageParams, TMC5160::MotorParameters motorParams)
  {
    port->begin(500000, SERIAL_8N1, rxPin, txPin);
    port->setTimeout(2);

    motor = TMC5160_UART_Transceiver(txEnPin, *port, 0);
    motor.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);


    
    motor.setEncoderResolution(200, 360, false);
  }

  void setup(double _spoolDiameter, float _x, float _y, float _targetOffsetX, float _targetOffsetY, bool _invert)
  {
    
    spoolDiameter = _spoolDiameter;
    x = _x;
    y = _y;
    targetOffsetX = _targetOffsetX;
    targetOffsetY = _targetOffsetY;
    invert = _invert;
  }

  void setTargetPosition(float value)
  {
    targetPosition = value;
    motor.setTargetPosition(metersToSteps(targetPosition));
  }

  
  void resetPosition(float positionAtReset)
  {
    motor.setCurrentPosition(metersToSteps(positionAtReset));
  }

  void setMaxSpeed(float value)
  {
    motor.setMaxSpeed(metersToSteps(value));
  }

  void setAcceleration(float value)
  {
    motor.setAcceleration(metersToSteps(value));
  }

  void setControlMode(bool positionMode)
  {
    motor.setRampMode(positionMode? TMC5160::POSITIONING_MODE : TMC5160::VELOCITY_MODE);
  }

  float getRealPosition()
  {
    return motor.getCurrentPosition();
  }

  //Helpers
  float metersToSteps(float m) { return m / (PI * spoolDiameter) * 200.0; } //200 steps per turn
  float stepsToMeters(float steps) { return (steps * PI * spoolDiameter) / 200.0; }


#if USE_OSC
  void handleMessage(OSCMessage &msg, int offset)
  {
    
    if(msg.match("/setPosition",offset))
    {
      if(msg.isFloat(1)) setTargetPosition(msg.getFloat(1));
      else
      {
        DBG("Motor position message argument 1 should be float");
      }
    }else if(msg.match("/resetPosition",offset))
    {
      if(msg.isFloat(1)) resetPosition(msg.getFloat(1));
      else if(msg.size() == 0) resetPosition(0);
      else
      {
        DBG("Motor reset position message argument 1 should be float, or no argument");
      }
    }else
    {
      DBG("Not handled");
    }
    
  }


  void sendOSCFeedback()
  {
    /*
    OSCMessage msg;
  
    msg.empty();
    msg.setAddress(driverStatusFeedbackPattern);
    char buffer[20];
    sprintf(buffer, "0x%x / 0x%08x", motors[motorIndex].readRegister(TMC5160_Reg::GSTAT), motors[motorIndex].readRegister(TMC5160_Reg::DRV_STATUS));
    msg.add(buffer); //TODO add human readable string
    udp.beginPacket(outIp, outPort);
    msg.send(udp);
    udp.endPacket();
    msg.empty();
  
    msg.setAddress(speedFeedbackPattern);
    msg.add(stepsToMeters(motors[motorIndex].getCurrentSpeed()));
    udp.beginPacket(outIp, outPort);
    msg.send(udp);
    udp.endPacket();
    msg.empty();
  
    msg.setAddress(positionFeedbackPattern);
    msg.add(stepsToMeters(motors[motorIndex].getCurrentPosition()));
    udp.beginPacket(outIp, outPort);
    msg.send(udp);
    udp.endPacket();
    msg.empty();
  
    msg.setAddress(encoderPositionFeedbackPattern);
    msg.add(stepsToMeters(motors[motorIndex].getEncoderPosition()));
    udp.beginPacket(outIp, outPort);
    msg.send(udp);
    udp.endPacket();
    msg.empty();
    */
  }
#endif
  
};

#endif
