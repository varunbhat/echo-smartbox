

/////////// workaround for arduino i2c eeprom//////////
//
//
//void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
//  int rdata = data;
//  Wire.beginTransmission(deviceaddress);
//  Wire.send((int)(eeaddress >> 8)); // MSB
//  Wire.send((int)(eeaddress & 0xFF)); // LSB
//  Wire.send(rdata);
//  Wire.endTransmission();
//}
//
//// WARNING: address is a page address, 6-bit end will wrap around
//// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
//void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
//  Wire.beginTransmission(deviceaddress);
//  Wire.send((int)(eeaddresspage >> 8)); // MSB
//  Wire.send((int)(eeaddresspage & 0xFF)); // LSB
//  byte c;
//  for ( c = 0; c < length; c++)
//    Wire.send(data[c]);
//  Wire.endTransmission();
//}
//
//byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
//  byte rdata = 0xFF;
//  Wire.beginTransmission(deviceaddress);
//  Wire.send((int)(eeaddress >> 8)); // MSB
//  Wire.send((int)(eeaddress & 0xFF)); // LSB
//  Wire.endTransmission();
//  Wire.requestFrom(deviceaddress,1);
//  if (Wire.available()) rdata = Wire.receive();
//  return rdata;
//}
//
//// maybe let's not read more than 30 or 32 bytes at a time!
//void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
//  Wire.beginTransmission(deviceaddress);
//  Wire.send((int)(eeaddress >> 8)); // MSB
//  Wire.send((int)(eeaddress & 0xFF)); // LSB
//  Wire.endTransmission();
//  Wire.requestFrom(deviceaddress,length);
//  int c = 0;
//  for ( c = 0; c < length; c++ )
//    if (Wire.available()) buffer[c] = Wire.receive();
//}
//
//
//
//
//void setup() 
//{
//  char somedata[] = "this is data from the eeprom"; // data to write
//  Wire.begin(); // initialise the connection
//  Serial.begin(9600);
//  i2c_eeprom_write_page(0x50, 0, (byte *)somedata, sizeof(somedata)); // write to EEPROM 
//  delay(10); //add a small delay
//  Serial.println("Memory written");
//}
//
//void loop() 
//{
//  int addr=0; //first address
//  byte b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory
//
//  while (b!=0) 
//  {
//    Serial.print((char)b); //print content to serial port
//    addr++; //increase address
//    b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
//  }
//  Serial.println(" ");
//  delay(2000);
//
//}
//
//
//






////		Serial.print("\nstatus : ");
////		Serial.print(status);
////		Serial.print(" = ");
//	sp = pushToEEPROM(sp, 1, status);
////		Serial.print(getFromEEPROM(sp - 1, 1));
////		Serial.print(" address : ");
////		Serial.print(sp - 1);
////		Serial.print(" ");
////		Serial.println(1);
//
////		Serial.print("diffTime: ");
////		Serial.print("  ");
////		Serial.print(dTime);
////		Serial.print(" = ");
//	sp = pushToEEPROM(sp, dtimeLen, dTime);
////		Serial.print(getFromEEPROM(sp - dtimeLen, dtimeLen));
////		Serial.print(" address : ");
////		Serial.print(sp - dtimeLen);
////		Serial.print(" ");
////		Serial.println(dtimeLen);
//
////		Serial.print("key  ");
////		Serial.print(key, HEX);
////		Serial.print(" = ");
//	sp = pushToEEPROM(sp, keylen, key);
////		Serial.print(getFromEEPROM(sp - keylen, keylen), HEX);
////		Serial.print("  address : ");
////		Serial.print(sp - keylen);
////		Serial.print(" ");
////		Serial.println(keylen);
//
////		Serial.print("count  ");
////		Serial.print(1);
////		Serial.print(" = ");
//	sp = pushToEEPROM(sp, 1, 1);
////		Serial.print(getFromEEPROM(sp - 1, 1));
////		Serial.print("   address : ");
////		Serial.print(sp - 1);
////		Serial.print(" ");
////		Serial.println(1);
//
////		Serial.print("count ");
////		Serial.print((sp - sp2) & 0x0f, HEX);
////		Serial.print(" = ");
//	pushToEEPROM(sp, 1, ((sp - sp2) & 0x0f));
////		Serial.print(getFromEEPROM(sp, 1), HEX);
////		Serial.print("  address : ");
////		Serial.print(sp);
////		Serial.print(" ");
////		Serial.println(1);






//#include <dfygraviti.h>
//DfygravitiServer echo;
//
////Jan 23 201315:33:24
//
//void setup()
//{
//  Serial.begin(9600);
//  Serial.println("started");
//  echo.begin();
//  pinMode(2,OUTPUT);
//  pinMode(3,OUTPUT);
//  digitalWrite(2,LOW);
//  pinMode(13,OUTPUT);
//  digitalWrite(13,HIGH);
//  pinMode(11,INPUT);
//  digitalWrite(2,LOW);
//  pinMode(A3,OUTPUT);
//  digitalWrite(A3,HIGH);
//  pinMode(A2,OUTPUT);
//  digitalWrite(A2,LOW);
//
//}
//
//void loop()  
//{
//  if (Serial.available()>=2)
//    switch (Serial.read())
//    {
//    case 'a': // 33a0
//      if (Serial.read()=='a')
//        echo.setRtcTime(); 
//    case 0x34: // 34a0
//      if (Serial.read()==0xa0)
//        echo.setRtcTime(1); 
//      break;
//    case 'g'://1991
//      if (Serial.read()=='g')
//        echo.pop(); 
//      break;
//    case 'p'://3839
//      if (Serial.read()=='p')
//        Serial.print("yeah i get it");
//        echo.setTvChannel();
//      break;
//    case 'r'://4521
//      if (Serial.read()=='r')
//        echo.getRtcTime(); 
//      break; 
//    case 'c': //514a
//      if (Serial.read()=='c')
//        echo.clearMemory();
//      break;
//    case 0xc3: //c3ab
//      if (Serial.read()==0xab)
//        echo.readRaw();
//    case 0xaa: //c3ab
//      if (Serial.read()==0xbb)
//        echo.begin();  
//      break;
//    default:
//      break;
//    }
//  echo.irRemotebuttonPressPoll();
//}
