#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

class SerialManager
{
public:


  String buffer;
  
  SerialManager()
  {
    addCallbackMessageReceived(&SerialManager::defaultCallback); 
  }

  void update()
  {
    parseSerial();
  }

  void parseSerial()
  {
    while(Serial.available() > 0)
    {
      char c = Serial.read();
      if(c == '\n')
      {
        parseBuffer();
        buffer = "";
      }else
      {
        buffer += c;
      }
    }
  }

  
  void parseBuffer()
  {
    int commaIndex = buffer.indexOf(',');
    int secondCommaIndex = buffer.indexOf(',', commaIndex + 1);
    String cc = buffer.substring(0, commaIndex);
    String v1 = buffer.substring(commaIndex + 1, secondCommaIndex);
    String v2 = buffer.substring(secondCommaIndex + 1); // To the end of the string

    char command = v1.charAt(0);

    onMessageReceived(command, v1, v2);

  }


   typedef void(*onEvent)(char, String, String);
    void (*onMessageReceived) (char command, String val1, String val2);

    void addCallbackMessageReceived (onEvent func) {
      onMessageReceived = func;
    }
  
    static void defaultCallback(char command, String val1, String val2)
    {
      //nothing
    }
  
};


#endif
