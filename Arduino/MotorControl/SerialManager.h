#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

class SerialManager
{
public:


  String buffer;
  
  SerialManager()
  { 
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
    //  Search for the next comma just after the first
    int secondCommaIndex = buffer.indexOf(',', commaIndex + 1);
    String firstValue = buffer.substring(0, commaIndex);
    String secondValue = buffer.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = buffer.substring(secondCommaIndex + 1); // To the end of the string
    
    float p1 = secondValue.toFloat();
    float p2 = thirdValue.toFloat();
    //Serial.println("Got : "+String(firstValue)+" / "+String(p1)+" / "+String(p2));
    if(firstValue.equals("p"))
    {
      //setMotorPos(0, p1);
      //setMotorPos(1, p2);
    }
  }
}


#endif
