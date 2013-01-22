#include "dfygraviti.h"

// constructors for setting the baud rates and initializing the values
DfygravitiServer::DfygravitiServer() {
	DfygravitiServer::baud = 9600;
	irrecv.enableIRIn();
	initialize();
}

// initializes the RTC
void DfygravitiServer::initialize() {
	irrecv.enableIRIn();
	DBman.initializeDB(rtcclk.getTime());
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
	//Serial.print(datex);
	for (int i = 0; i < 8; i++)
		while (1)
			if (Serial.available()) {
				timex[i] = Serial.read();
				break;
			}
	timex[7] = '\0';
	//Serial.println(timex);
	//Serial.flush();
	//time = dat[3] << (3 * 8) | dat[2] << (2 * 8) | dat[1] << (1 * 8) | dat[0];
	rtcclk.setTime(datex, timex);
	getRtcTime();
}

//Sends the unix time to the Phone in JSON format.. needs to be tested
void DfygravitiServer::getRtcTime() {
	Serial.print(rtcclk.getTime());
}

//Sets the tv channel.. 
// Method of invoking: 
// *first byte is the number of digits
// * rest of the two bytes are the channel number code 
void DfygravitiServer::setTvChannel() {
	byte dat[2];
	uint16_t channel = 0;
	uint8_t digits = 0;
	//char * buffer = getSerialData(2);
//	channel = buffer[1] << (8 * 1) | buffer[0];
	irsend.sendOnida(channel, 16);

	//irsend.sendSony(0x490, 17);
//	for (int i = 0; i < 10; i++)
//		irsend.sendSony(0x490, 20);
//		delay(100);
}

void DfygravitiServer::irRemotebuttonPressPoll() {
	if (irrecv.decode(&results)) {
		if (results.decode_type != UNKNOWN) {
			Serial.print(results.value, HEX);
			DBman.push(rtcclk.getTime(), results.value);
		}
		irrecv.resume();
	}
}
void DfygravitiServer::clearMemory() {
	for (int i = 0; i < 512; i++)
		EEPROM.write(i, 0);
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

//////////////////////////////////////////////////Database Manager////////////////////////

DatabaseManager::DatabaseManager() {
	uint8_t programCode = EEPROM.read(0);
	if (programCode != DatabaseString)
		initializeDB(rtcclk.getTime());
}

bool DatabaseManager::initializeDB(uint32_t timenow) {
	pushToEEPROM(0, 1, DatabaseString);
	pushlastUpdateTime(timenow);
	pushlastDumpTime(timenow);
	setEEPROMLocation (INTERNAL);
	stackEntries(0);
	stackStartAddress(13);
	stackpointer(13);
}
///////////////////////////////////////////////////////stack pointer set & retrive//////
bool DatabaseManager::stackpointer(uint8_t address) {
	return pushToEEPROM(12, 1, address);
}

uint8_t DatabaseManager::stackpointer() {
	return getFromEEPROM(13, 1);
}

///////////////////////////////////////////////////////// Set stack start address///////
bool DatabaseManager::stackStartAddress(uint8_t address) {
	return pushToEEPROM(11, 1, address);
}
uint8_t DatabaseManager::stackStartAddress() {
	return getFromEEPROM(11, 1);
}

////////////////////////////////////////////////////////// count of the total number of entries///
bool DatabaseManager::stackEntries(uint8_t number) {
	return pushToEEPROM(10, 1, number);
}
uint8_t DatabaseManager::stackEntries() {
	return getFromEEPROM(10, 1);
}

////////////////////////////////////////////////////////////////Pushing and getting to eeprom main Func//////
// Little endian storage (LSB = Lower memory location)
uint32_t DatabaseManager::getFromEEPROM(uint8_t start, uint8_t bytes) {
	// make changes here for xternal eeprom
	// for Internal EEprom
	uint32_t temp = 0;
	for (int i = 0; i < bytes; i++) {
		temp = temp | EEPROM.read(start + i);
		temp <<= 8;
	}
	return temp;
}

bool DatabaseManager::pushToEEPROM(uint8_t start, uint8_t bytes,
		uint32_t data) {
	// make changes here for xternal eeprom
	// for Internal EEprom
	uint32_t temp = 0;
	for (int i = 0; i < bytes; i++) {
		temp = data & 0xff;
		EEPROM.write(start + i, temp);
		data >>= 8;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////// last updated time /////////////
uint32_t DatabaseManager::getlastUpdateTime() {
	return getFromEEPROM(1, 4);
}

bool DatabaseManager::pushlastUpdateTime(uint32_t mytime) {
	return pushToEEPROM(1, 4, mytime);
}

/////////////////////////////////////////////////////////////last Dump time //////////////

uint32_t DatabaseManager::getlastDumpTime() {
	return getFromEEPROM(5, 4);
}

bool DatabaseManager::pushlastDumpTime(uint32_t mytime) {
	return pushToEEPROM(5, 4, mytime);
}

/////////////////////////////////////////////////////////////Set external or internal eeprom/////////
bool DatabaseManager::getEEPROMLocation() {
	return getFromEEPROM(9, 1) & 0x01;
}

bool DatabaseManager::setEEPROMLocation(bool type) {
	uint8_t temp = getFromEEPROM(9, 1) | (type * 0x01);
	return pushToEEPROM(9, 1, temp);
}
void DatabaseManager::incStackCount() {
	uint8_t temp = getFromEEPROM(10, 1);
	pushToEEPROM(10, 1, temp + 1);
}

///////////////////////////////////////////////////////////////////Housekeeping to get no of bytes////
uint8_t DatabaseManager::getByteLen(uint32_t number) {
	uint8_t count = 1;
	while (number) {
		number >>= 8;
		if (number != 0)
			count++;
	}
	return count;
}
/////////////////////////////////////////////////////////////////////push values to the database ------------
uint8_t DatabaseManager::push(uint32_t receiption_time, uint32_t key) {
	uint32_t dTime = receiption_time - getlastDumpTime();
	uint8_t dtimeLen = getByteLen(dTime);
	uint8_t keylen = getByteLen(key);
	uint8_t sp = stackpointer();
	uint8_t status;
	Serial.print("   Pushing  ");
	Serial.print("  stackPointer:");
	Serial.print(sp);

	//if ((dTime <= 1 && key == -1) || dTime <= 1 && key == lastkey) {
	//	if (stackStartAddress() != stackpointer())
	pushToEEPROM(sp - 1, 1, getFromEEPROM(sp - 1, 1) + 1);
	//	return 0;
	//}
	status = ((dtimeLen - 1) << 6) | ((keylen - 1) << 4)
			| (getFromEEPROM(sp, 1) & 0x00ff);
	Serial.print("  Register:");
	Serial.print(status);
	pushToEEPROM(sp, 1, status);
	pushToEEPROM(sp + 1, dtimeLen, dTime);
	pushToEEPROM(sp + dtimeLen + 1, keylen, key);
	pushToEEPROM(sp + dtimeLen + keylen + 1, 1, 1);
	stackpointer(sp + dtimeLen + keylen + 1 + 1);
	pushToEEPROM(sp + dtimeLen + keylen + 1 + 1, 1,
			(sp + dtimeLen + keylen + 1 + 1) & 0xff);
	incStackCount();
	lastkey = key;
	Serial.print(sp + dtimeLen + keylen + 1 + 1);
	Serial.print("\n");
	return sp + dtimeLen + keylen + 1 + 1;
}

clock::clock() {
	Wire.begin();
	RTC.begin();
	if (!RTC.isrunning()) {
		Serial.println("RTC Connected");
	} else {
		Serial.println("Rtc disabled");
	}
	
}

uint32_t clock::getTime() {
	now = RTC.now();
	timeSnapshot = now.unixtime();
	return timeSnapshot;
}

bool clock::setTime(char*datex, char*timex) {
	RTC.adjust(DateTime(datex, timex));
}
