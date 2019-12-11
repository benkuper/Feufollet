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
    char command = buffer.substring(0, commaIndex).charAt(0);
    
    String values[16];
    
    int valueIndex = 0;
    int prevCommaIndex = commaIndex;
    int nextCommaIndex = buffer.indexOf(',', commaIndex + 1);
    while(nextCommaIndex != -1 && valueIndex < 15)
    {
       values[valueIndex] = buffer.substring(prevCommaIndex + 1, nextCommaIndex);
       valueIndex++;
      
       prevCommaIndex = nextCommaIndex;
       nextCommaIndex = buffer.indexOf(',', nextCommaIndex+1);
    }


    if(prevCommaIndex != -1)
    {
      values[valueIndex] = buffer.substring(prevCommaIndex+1);
      valueIndex++;
    }

    
    DBG("Received buffer, numValues "+String(valueIndex));
    
    onMessageReceived(command, values, valueIndex);

  }


   typedef void(*onEvent)(char, String *, int);
    void (*onMessageReceived) (char command, String * values, int numValues);

    void addCallbackMessageReceived (onEvent func) {
      onMessageReceived = func;
    }
  
    static void defaultCallback(char command, String * values, int numValues)
    {
      //nothing
    }
  
};


#endif
