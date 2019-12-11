#ifndef ETHERNETMANAGER_H
#define ETHERNETMANAGER_H

#include <ETH.h>
const int ETH_MDC_PIN = 18;
const int ETH_MDIO_PIN = 5;


class EthernetManager :
{
  public:
    bool eth_connected = false;

    EthernetManager()
    {
      
    }

    void init()
    {
      WiFi.onEvent(wiFiEvent);
      ETH.begin(0, -1, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_PHY_LAN8720, ETH_CLOCK_GPIO17_OUT);
      ETH.config(IPAddress(2, 0, 0, 6), IPAddress(192, 168, 0, 254), IPAddress(255, 0, 0, 0), IPAddress(192, 168, 0, 254));
      int numTries = 0;
      while (!eth_connected && numTries < 5)
      {
        delay(1000);
        numTries++;
      }
    }


    void wiFiEvent(WiFiEvent_t event) {
      switch (event) {
        
        case SYSTEM_EVENT_ETH_START:
          DBG("ETH Started");
          //set eth hostname here
          ETH.setHostname("esp32-ethernet");
          break;
          
        case SYSTEM_EVENT_ETH_CONNECTED:
          DBG("ETH Connected");
          break;
          
        case SYSTEM_EVENT_ETH_GOT_IP:
          String s = "ETH MAC: "+String(ETH.macAddress())+", IPv4: "+String(ETH.localIP()))+", ";
          if (ETH.fullDuplex()) s += "FULL_DUPLEX, ";
          s += String(ETH.linkSpeed())+" Mbps";
          DBG(s);
          eth_connected = true;
          break;
          
        case SYSTEM_EVENT_ETH_DISCONNECTED:
          DBG("ETH Disconnected");
          eth_connected = false;
          break;
          
        case SYSTEM_EVENT_ETH_STOP:
          DBG("ETH Stopped");
          eth_connected = false;
          break;
          
        default:
          break;
      }
    }
};


#endif
