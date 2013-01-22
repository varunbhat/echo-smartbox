#ifndef DFYGRAVITI_H
#define DFYGRAVITI_H

#define DatabaseString 149
#define EXTERNAL 1
#define INTERNAL 0
#include <stdint.h>
#include <IRremote.h>
#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>
#include <EEPROM.h>
class DatabaseManager {
public:
	DatabaseManager();
	bool initializeDB(uint32_t timenow);
	uint32_t getlastDumpTime();
	uint32_t getlastUpdateTime();
	bool pushlastDumpTime(uint32_t mytime);
	bool pushlastUpdateTime(uint32_t mytime);
	bool getEEPROMLocation();
	bool setEEPROMLocation(bool type);
	uint8_t stackEntries(void);
	bool stackEntries(uint8_t);
	uint8_t push(uint32_t receiption_time, uint32_t key);
	bool stackStartAddress(uint8_t address);
	uint8_t stackStartAddress(void);
	bool stackpointer(uint8_t address);
	uint8_t stackpointer(void);
	uint32_t lastkey;
	uint8_t lastLoc;

private:
	uint32_t lastUpdateTime;
	uint32_t lastDumpTime;
	uint8_t stackEntrys;
	uint32_t getFromEEPROM(uint8_t start, uint8_t bytes);
	bool pushToEEPROM(uint8_t start, uint8_t bytes, uint32_t data);
	uint8_t getByteLen(uint32_t);
	void incStackCount();
};

class DfygravitiServer {
public:
	DfygravitiServer();
	void initialize();
	int setRtcTime();
	void getRtcTime();
	void setTvChannel();
	void irRemotebuttonPressPoll();
	void clearMemory();
private:
	uint16_t baud;
	boolean rtc_enabled;
	IRrecv irrecv;
	IRsend irsend;
	decode_results results;
	char * getSerialData(int);
	DatabaseManager DBman;
};

class clock {
public:
	clock();
	uint32_t getTime();
	bool setTime(char *, char *);
private:
	RTC_DS1307 RTC;
	DateTime now;
	uint32_t timeSnapshot;
} rtcclk;

#endif
