#include <dfygraviti.h>
DfygravitiServer echo;

//Jan 23 201315:33:24

void setup()
{
  Serial.begin(9600);
  Serial.println("started");
  echo.begin();

  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(11,INPUT);
  digitalWrite(2,LOW);
  pinMode(A3,OUTPUT);
  digitalWrite(A3,HIGH);
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);

}

void loop()  
{
  if (Serial.available()>=2)
    switch (Serial.read())
    {
    case 0x33: // 33a0
      if (Serial.read()==0xa0)
        echo.setRtcTime(); 
    case 0x34: // 33a0
      if (Serial.read()==0xa0)
        echo.setRtcTime(1); 
      break;
    case 0x19:
      if (Serial.read()==0x91)
        echo.pop(); 
      break;
    case 0x38:
      if (Serial.read()==0x39)
        echo.setTvChannel();
      break;
    case 0x45://4521
      if (Serial.read()==0x21)
        echo.getRtcTime(); 
      break; 
    case 0x51: //514a
      if (Serial.read()==0x4a)
        echo.clearMemory();
      break;
    case 0xc3: //514a
      if (Serial.read()==0xab)
        echo.readRaw();
      break;
    default:
      break;
    }
  echo.irRemotebuttonPressPoll();
}





