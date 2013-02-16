#include <dfygraviti.h>
DfygravitiServer echo;

//Jan 23 201315:33:24

void setup()
{
  Serial.begin(9600);
  Serial.println("started");
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(A3,OUTPUT);
  analogWrite(A3,255);
  pinMode(A2,OUTPUT);
  analogWrite(A2,0);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(11,INPUT);

  echo.begin();
}

void loop()  
{
  if (Serial.available()>=2)
    switch (Serial.read())
    {
    case 0x33: // 33a0
      if (Serial.read()==0xa0)
        echo.setRtcTime(); 
    case 0x34: // 34a0
      if (Serial.read()==0xa0)
        echo.setRtcTime(1); 
      break;
    case 0x19://1991
      if (Serial.read()==0x91)
        echo.pop(); 
      break;
    case 0x38://3839
      if (Serial.read()==0x39)
        Serial.print("yeah i get it");
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
    case 0xc3: //c3ab
      if (Serial.read()==0xab)
        echo.readRaw();
    case 0xaa: //c3ab
      if (Serial.read()==0xbb)
        echo.begin();  
      break;
    default:
      break;
    }
  echo.irRemotebuttonPressPoll();
}


