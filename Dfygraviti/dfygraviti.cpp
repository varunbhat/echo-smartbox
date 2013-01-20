#include "dfygraviti.h"

// constructors for setting the baud rates and initializing the values
DfygravitiServer::DfygravitiServer() {
	DfygravitiServer::baud = 9600;
	irrecv.enableIRIn();
}
// initializes the RTC
void DfygravitiServer::initialize() {
	Wire.begin();
	RTC.begin();
	if (!RTC.isrunning()) {
		rtc_enabled = false;
		//	Serial.println("RTC Disabled");
	} else {
		rtc_enabled = true;
		//	Serial.println("RTC Enabled");
	}
	irrecv.enableIRIn();

}

//Set the RTC for the present time.. needs to be invoked for maintainance
int DfygravitiServer::setRtcTime() {
	uint32_t time;
	char datex[12];
	char timex[9];
	byte receivedBytes = 4;
	for (int i = 0; i < 11; i++)
		while (1)
			if (Serial.available()) {
				datex[i] = Serial.read();
				break;
			}
	datex[11] = '\0';

	Serial.print(datex);

	for (int i = 0; i < 8; i++)
		while (1)
			if (Serial.available()) {
				timex[i] = Serial.read();
				break;
			}

	timex[7] = '\0';
	Serial.println(timex);
	Serial.flush();
	//time = dat[3] << (3 * 8) | dat[2] << (2 * 8) | dat[1] << (1 * 8) | dat[0];
	if (rtc_enabled)
		RTC.adjust(DateTime(datex, timex));
	getRtcTime();
}

//Sends the unix time to the Phone in JSON format.. needs to be tested
void DfygravitiServer::getRtcTime() {
	DateTime now = RTC.now();
	Serial.print(now.unixtime());
//	Serial.print("Received time is :");
//	Serial.print(now.year());
//	Serial.print(" ");
//	Serial.print(now.month());
//	Serial.print(" ");
//	Serial.print(now.day());
//	Serial.print(" ");
//	Serial.print(now.hour());
//	Serial.print(" ");
//	Serial.print(now.minute());
//	Serial.print(" ");
//	Serial.print(now.second());
}

//Sets the tv channel.. 
// Method of invoking: 
// *first byte is the number of digits
// * rest of the two bytes are the channel number code 
void DfygravitiServer::setTvChannel() {
	byte dat[2];
	uint16_t channel = 0;
	uint8_t digits = 0;
	char * buffer = getSerialData(2);
	channel = buffer[1] << (8 * 1) | buffer[0];
	irsend.sendOnida(channel, 16);
}

void DfygravitiServer::irRemotebuttonPressPoll() {
	if (irrecv.decode(&results)) {
		results.value results.
		irrecv.resume();
	}
}

char * DfygravitiServer::getSerialData(int byteLen) {
	char buffer[30];
	for (int i = 0; i < byteLen; i++)
		while (1)
			if (Serial.available()) {
				buffer[byteLen - i - 1] = Serial.read();
				break;
			}
	return buffer;
}

//BufferSerial::BufferSerial() {
//	for (int i = 0; i < 30; i++)
//		buffer[i] = 0;
//	bufferLen = 0;
//}