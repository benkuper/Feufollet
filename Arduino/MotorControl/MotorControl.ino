#define SERIAL_DEBUG 1

#if SERIAL_DEBUG
#define DBG(textToWrite) Serial.println(textToWrite)
#else
#define DBG(textToWrite)
#endif


#define USE_WIFI 1
#define USE_OSC 1
#define USE_ETHERNET 0
#define USE_SERIAL 1
#define USE_MOTORS 1

#include "DeviceSettings.h"

#include <Preferences.h>
Preferences preferences;


#if USE_WIFI
#include "WiFiManager.h"
WiFiManager wifiManager;
#endif

#if USE_OSC
#include "OSCManager.h"
OSCManager oscManager(wifiManager.oscUDP);
#endif

#if USE_ETHERNET
#include "EthernetManager.h"
EthernetManager ethernetManager;
#endif 

#if USE_MOTORS
#include "MotorManager.h"
MotorManager motorManager;
#endif


const int LED_PIN = 4;

//forware declaration
void resetESP(bool);

#if USE_WIFI
void wifiConnectingUpdate(int curTry)
{
  //Handle connection behaviors here
}
#endif

#if USE_OSC
void messageReceived(OSCMessage &msg)
{
  char address[256];
  msg.getAddress(address);
  DBG("OSC Message received : " + String(address));

  if (msg.match("/reset"))
  {
    resetESP(false);
  }
  else if (msg.match("/bootloader"))
  {
    resetESP(true);
  }
  
#if USE_MOTORS
  if(motorManager.handleMessage(msg)) return;
#endif

   DBG("...message not handled");
}

void oscConnectionChanged(bool isConnected)
{
  DBG("OSC connection change "+String(isConnected));
}
#endif

void setup()
{
  Serial.begin(115200);
  Serial.println("");

  preferences.begin("farfadet-settings", false);
  DeviceSettings::getInstance()->init();

  
  //WiFi & OSC
#if USE_WIFI
  wifiManager.addCallbackConnectingUpdate(&wifiConnectingUpdate);
  wifiManager.init();
  
  #if USE_OSC
      oscManager.isReadyToSend = true;
      oscManager.init();
      oscManager.addCallbackMessageReceived(&messageReceived);
      oscManager.addCallbackConnectionChanged(&oscConnectionChanged);
  #endif
#endif

#if USE_MOTORS
  motorManager.init();
#endif

}

void loop()
{
  #if USE_OSC
  oscManager.update();
  #endif

  #if USE_MOTORS
  motorManager.update();
  #endif
}


void resetESP(bool toBootloader)
{
  if (toBootloader)
  {
    pinMode(0, OUTPUT);
    digitalWrite(0, LOW);
  }

  ESP.restart();
}
