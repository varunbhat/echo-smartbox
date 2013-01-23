#include <stdint.h>
#include "IRremote.h"
#include <RTClib.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "Wire.h"

#define DatabaseString 149

#define EXTERNAL 1
#define INTERNAL 0

class DatabaseManager {
public:
	DatabaseManager();
	bool initializeDB(uint32_t timenow);
	uint32_t lastDumpTime();
	uint32_t lastUpdateTime();
	uint8_t begin();
	bool lastDumpTime(uint32_t mytime);
	bool lastUpdateTime(uint32_t mytime);
	bool getEEPROMLocation();
	bool setEEPROMLocation(bool type);
	uint8_t stackEntries(void);
	bool stackEntries(uint8_t);
	uint8_t push(uint32_t receiption_time, uint32_t key);
	bool stackStartAddress(uint8_t address);
	uint8_t stackStartAddress(void);
	bool stackpointer(uint16_t address);
	uint8_t stackpointer(void);
	uint8_t lastLoc;
	void clearMemory();
	void readRaw();
	void readRaw(uint8_t start,uint8_t stop);
	void pop(uint16_t address);
private:
	uint8_t stackEntrys;
	uint32_t getFromEEPROM(uint8_t start, uint8_t bytes);
	uint16_t pushToEEPROM(uint8_t start, uint8_t bytes, uint32_t data);
	uint8_t getByteLen(uint32_t);
	void incStackCount();
	uint8_t stackCount();
	uint32_t lastkey;
};

class DfygravitiServer {
public:
	DfygravitiServer();
	int setRtcTime();
	int setRtcTime(bool unixTime);
	void getRtcTime();
	void setTvChannel();
	void irRemotebuttonPressPoll();
	void clearMemory();
	uint8_t begin();
	void readRaw();
private:
	uint16_t baud;
	IRrecv irrecv;
	IRsend irsend;
	decode_results results;
	char * getSerialData(int);
	DatabaseManager DBman;
};

