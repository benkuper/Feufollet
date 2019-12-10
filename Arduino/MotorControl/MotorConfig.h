#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H

#include <TMC5160.h>

#define NUM_MOTORS 2

class MotorConfig 
{
public:
  const uint8_t uartRxPins[NUM_MOTORS] = {35, 34};
  const uint8_t uartTxPins[NUM_MOTORS] = {15, 2};
  const uint8_t uartTxEnPins[NUM_MOTORS] = {12, 23}; // Differential transceiver TX enable pin
  HardwareSerial * serialPorts[NUM_MOTORS] = {&Serial1, &Serial2};
 
  TMC5160::PowerStageParameters powerStageParams[2];
  TMC5160::MotorParameters motorParams[2];
  
  MotorConfig()
  {
    powerStageParams[0].drvStrength = 0;
    powerStageParams[0].bbmTime = 0;
    powerStageParams[0].bbmClks = 0;
    motorParams[0].globalScaler = 191;
    motorParams[0].irun = 31;
    motorParams[0].ihold = 15;
    motorParams[0].freewheeling = TMC5160_Reg::PWMCONF_freewheel_Values::FREEWHEEL_NORMAL;
    motorParams[0].pwmOfsInitial = 50;
    motorParams[0].pwmGradInitial = 53;
    
    powerStageParams[1].drvStrength = 0;
    powerStageParams[1].bbmTime = 0;
    powerStageParams[1].bbmClks = 0;
    motorParams[1].globalScaler = 191;
    motorParams[1].irun = 31;
    motorParams[1].ihold = 15;
    motorParams[1].freewheeling = TMC5160_Reg::PWMCONF_freewheel_Values::FREEWHEEL_NORMAL;
    motorParams[1].pwmOfsInitial = 44;
    motorParams[1].pwmGradInitial = 58;
  }
};

#endif
