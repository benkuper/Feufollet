#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

#if USE_OSC
#include <OSCMessage.h>
#endif


class DeviceSettings
{
public:
  static String deviceID;
  static String deviceType;

  static DeviceSettings * instance;

  DeviceSettings()
  {
    
  }

  static DeviceSettings * getInstance()
  {
    if(instance == nullptr) instance = new DeviceSettings();
    return instance;
  }

  void init()
  {
    uint64_t did = ESP.getEfuseMac();
    deviceID = String((uint32_t)(did >> 32)) + String((uint32_t)(did & 0xffffffff));
    
    DBG("DeviceSettings init, device ID " + deviceID);
  }

};

String DeviceSettings::deviceID = "notset";
String DeviceSettings::deviceType = "Farfadet";
DeviceSettings * DeviceSettings::instance = nullptr;
  
#endif //DEVICESETTINGS_H
