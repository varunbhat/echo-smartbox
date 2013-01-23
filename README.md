# Dfygraviti Library

##About
This library contains Sub libraries including

* IRRemote
* EEPROM
* RTClib
* Wire
* DfygravitiServer
* DatabaseManager

## Burning the code to Arduino

* Copy the **Dfygraviti** folder in the repository into the following path

	~/path/to/arduino/libraries/

* Copy the IRDfygraviti folder into your sketchbook directory, or just open it in **Arduino IDE**
* Connect the Arduino Uno to the USB port
* Upload the code.

##Using the Library in Android
* To use the library create a Bluetooth socket.
* Send the hex codes through the socket once the connection is successful.
* the Codes are as follows.


| Values to be Sent               | Description                                                               |
|:-------------------------------:|:--------------------------------------------------------------------------|
| 0x38 and 0x39                   |Send the data to the TV through IR                                         |
| 0x45 and 0x21                   |Get the RTC time                                                           |    
| 0x51 and 0x4a                   |clear the EEPROM Stored Data                                               |
| 0x19 and 0x91 and 0xFF          |Get all the stack data stored in the EEPROM                                |
| 0x19 and 0x91 and 0x01 and *id* |Get one element in the stack ex:0x19 0x91 0x01 0x0a to get the 10th element|



