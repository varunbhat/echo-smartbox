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
|:--------------------------------|:--------------------------------------------------------------------------|
| 0x38 and 0x39                   |Send the data to the TV through IR                                         |
| 0x45 and 0x21                   |Get the RTC time                                                           |
| 0x33 and 0xa0 followed by *time in mmm dd yyyyhh:mm:ss format*|Set the RTC time                     |
| 0x34 and 0xa0 followed by *time in unixtime format sent as string*|Set the RTC time in unix format                      |
| 0x51 and 0x4a                   |clear the EEPROM Stored Data                                               |
| 0xc3 and 0xab| get raw data that is dumped in memory (dor debugging)|
| 0x19 and 0x91 |Get all the stack data stored in the EEPROM  in json format                               |


###Data format for date setting
The time needs to be sent as:

		MMM dd yyyyhh:mm:ss

example:

		Jan 23 201315:33:24

Please note the spaces and the colens. Wrong format will not be accepted and will lead to erraneous output.


**OR**


Send the unix time in string format ie 

		"1353442556"

Note that the string is 10 characters long.

###Setting Channel
The prefix 0x38 and 0x39 should for each channel to be transmitted
example:
		0x38 0x39 0xA8B92 0x38 0x39 0x40B92 ...........
###Dumping the data stored in EEPROM to the app
Sending the above command will result in getting a stream of json encoded string as shown:

		[{'Key':A8B92,'Count':6,'Time':2657146590}
		{'Key':80B92,'Count':4,'Time':2657146473}
		{'Key':40B92,'Count':5,'Time':2657146472}
		{'Key':A8B92,'Count':7,'Time':2657146470}
		]

Separation of the characters and the algorithms should be used at the app end for efficient use.

###Raw data
It dumps all the bytes stored in the EEPROM in a Raw form. Use it only for Debugging.

##RTC issue
For some reason the RTC stops to work when completely powered off (coin cell removed). If the RTC does not switch on, Burn the RTClib example on to the chip and then burn the IRDfygraviti.ino.
Please leave a comment if you find out what the error is.
---------
## Bugs to be fixed
* RTC stops working for no reason
* I2C EEPROM extension not working.



