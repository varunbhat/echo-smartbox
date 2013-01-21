#include <dfygraviti.h>

DfygravitiServer echo;

void setup()
{
  Serial.begin(9600);
  Serial.println("started");
  echo.initialize();

  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(11,INPUT);

}

void loop()  
{
  if (Serial.available()>2)
    switch (Serial.read())
    {
      // case to Set the RTCtime.. !important
      case 0x33: // 33a0
      if (Serial.read()==0xa0)
        echo.setRtcTime(); 
      break;
      // Set the channel on the tv. Sit back and enjoy the movie
    case 0x38:
      if (Serial.read()==0x39)
        echo.setTvChannel();
      break;
      // Get the time.. 
    case 0x45:
      if (Serial.read()==0x21)
        echo.getRtcTime();  
    default:

      break;
    }
    // poll for button press on the remote
  echo.irRemotebuttonPressPoll();
}

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


