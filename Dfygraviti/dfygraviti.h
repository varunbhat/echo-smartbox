#ifndef DFYGRAVITI_H
#define DFYGRAVITI_H
#include <stdint.h>

#include <IRremote.h>
#include <Wire.h>
#include <RTClib.h>

#include <Arduino.h>


//class BufferSerial {
//public:
//	BufferSerial(); 
//	uint8_t buffer[30];
//	uint8_t bufferLen;
//};

class DfygravitiServer {
public:
	DfygravitiServer();
	void initialize();
	int setRtcTime();
	void getRtcTime();
	void setTvChannel();
	void irRemotebuttonPressPoll();
private:
	uint16_t baud;
	boolean rtc_enabled;
	RTC_DS1307 RTC;
	IRrecv irrecv;
	IRsend irsend;
	decode_results results;
	char * getSerialData(int);
};


#endif
