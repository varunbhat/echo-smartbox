#include "dfygraviti.h"

//#define DEBUG

#define UNIXTIME 1
RTC_DS1307 RTC;
DateTime now;

//////////////////////////////////////////////////////server/////////
DfygravitiServer::DfygravitiServer() {
}

uint8_t DfygravitiServer::begin() {
	Wire.begin();
	RTC.begin();
	if (RTC.isrunning() == 0) {
		Serial.println("RTC Disabled");
	} else {
		Serial.println("RTC Enabled");
	}
	getRtcTime();
	DBman.begin();
	irrecv.enableIRIn();
}
/////////////////////////////////////RTC time set//
int DfygravitiServer::setRtcTime() {
	char datex[12];
	char timex[8];
	for (int i = 0; i < 11; i++)
		while (1)
			if (Serial.available()) {
				datex[i] = Serial.read();
				break;
			}
//	datex = getSerialData(12);
	datex[11] = '\0';
	Serial.println(datex);
	for (int i = 0; i < 8; i++)
		while (1)
			if (Serial.available()) {
				timex[i] = Serial.read();
				break;
			}
//	timex = getSerialData(12);
	timex[8] = '\0';
	Serial.println(timex);
	RTC.adjust(DateTime(datex, timex));
	getRtcTime();
}

int DfygravitiServer::setRtcTime(bool unixTime) {
	uint32_t time = 0;
	for (int i = 0; i < 9; i++)
		while (1)
			if (Serial.available() > 0) {
				time = time + (Serial.read() - 0x30);
				time *= 10;
				break;
			}
	while (1)
		if (Serial.available() > 0) {
			time = time + (Serial.read() - 0x30);
			break;
		}
	Serial.println("time set is ");
	Serial.println(time);
	RTC.adjust(DateTime(time));
	getRtcTime();
}
/////////////////////////////////////RTC time get//
void DfygravitiServer::getRtcTime() {
	now = RTC.now();
	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(' ');
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println();
}
//////////////////////////////////////Remote polling//

void DfygravitiServer::irRemotebuttonPressPoll() {
#ifdef DEBUG	
	if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX);
		irrecv.resume(); // Receive the next value
	}
#else
//	now = RTC.now();
//	DBman.push(now.unixtime(), 0xabcde);
	if (irrecv.decode(&results)) {
		if (results.decode_type != UNKNOWN) {
			Serial.println(results.value, HEX);
			now = RTC.now();
			DBman.push(now.unixtime(), results.value);
		}
		irrecv.resume();
	}
#endif
}

void DfygravitiServer::setTvChannel() {
	byte dat[2];
	uint16_t channel = 0;
	uint8_t digits = 0;
//	char * buffer = getSerialData(2);
	char buffer[] = {'\0','\0'};
	for (int i = 0; i < 8; i++)
		while (1)
			if (Serial.available()) {
				buffer[1] = Serial.read();
				break;
			}
	for (int i = 0; i < 8; i++)
			while (1)
				if (Serial.available()) {
					buffer[0] = Serial.read();
					break;
				}
	channel = buffer[1] << (8 * 1) | buffer[0];
	irsend.sendOnida1(channel, 16);
	irrecv.enableIRIn();
	Serial.print("Sent Channel is:");
	Serial.println(channel, HEX);
}

void DfygravitiServer::clearMemory() {
	DBman.clearMemory();
}

void DfygravitiServer::pop() {
	DBman.pop();
}
void DfygravitiServer::readRaw() {
	DBman.readRaw();
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

//////////////////////////////////////////////database manager//
DatabaseManager::DatabaseManager() {
}

uint8_t DatabaseManager::begin() {
	Serial.println("Database manager");
	uint8_t programCode = EEPROM.read(0);
	if (programCode != DatabaseString) {
		now = RTC.now();
		initializeDB(now.unixtime());
	}
}

bool DatabaseManager::initializeDB(uint32_t timenow) {
	pushToEEPROM(0, 1, DatabaseString);
	lastUpdateTime(timenow);
	lastDumpTime(timenow);
	setEEPROMLocation (INTERNAL);
	stackEntries(0);
	stackStartAddress(13);
	stackpointer(13);
}

void DatabaseManager::clearMemory() {
	for (int i = 0; i < 512; i++)
		EEPROM.write(i, 0);
	now = RTC.now();
	initializeDB(now.unixtime());
}

void DatabaseManager::readRaw() {

	for (int i = 0; i < 512; i++) {
		Serial.print(EEPROM.read(i));
		Serial.print(" ");
	}
}

void DatabaseManager::readRaw(uint8_t start, uint8_t stop) {
	Serial.println("\nReading EEPROM:");
	for (int i = start; i < stop; i++) {
		Serial.print(EEPROM.read(i), HEX);
		Serial.print(" ");
	}
	Serial.println();
}
///////////////////////////////////////////////////////stack pointer set & retrive//////
bool DatabaseManager::stackpointer(uint16_t address) {
	return pushToEEPROM(12, 1, address);
}

uint8_t DatabaseManager::stackpointer() {
	return getFromEEPROM(12, 1);
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
	for (int i = bytes - 1; i >= 0; i--) {
		temp <<= 8;
		temp = temp | EEPROM.read(start + i);

	}
	return temp;
}

uint16_t DatabaseManager::pushToEEPROM(uint8_t start, uint8_t bytes,
		uint32_t data) {
// make changes here for xternal eeprom

// for Internal EEprom
	uint8_t temp = 0;
	for (int i = 0; i < bytes; i++) {
		temp = data & 0xff;
		EEPROM.write(start + i, temp);
		data >>= 8;
	}
	return start + bytes;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////// last updated time /////////////
uint32_t DatabaseManager::lastUpdateTime() {
	return getFromEEPROM(1, 4);
}

bool DatabaseManager::lastUpdateTime(uint32_t mytime) {
	return pushToEEPROM(1, 4, mytime);
}

/////////////////////////////////////////////////////////////last Dump time //////////////

uint32_t DatabaseManager::lastDumpTime() {
	return getFromEEPROM(5, 4);
}

bool DatabaseManager::lastDumpTime(uint32_t mytime) {
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

uint8_t DatabaseManager::stackCount() {
	return getFromEEPROM(10, 1);
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

uint8_t DatabaseManager::push(uint32_t receiption_time, uint32_t key) {
	uint32_t dTime = receiption_time - lastDumpTime();
	uint32_t dUpdTime = receiption_time - lastUpdateTime();
	uint8_t status;
	uint32_t sp = stackpointer();
	uint8_t dtimeLen = getByteLen(dTime);
	uint8_t keylen = getByteLen(key);

	uint8_t sp2 = sp;

	if ((dUpdTime < 2 && key == -1) || dUpdTime <= 1 && key == lastkey) {
		if (stackStartAddress() != sp) {
			pushToEEPROM(sp - 1, 1, getFromEEPROM(sp - 1, 1) + 1);
			lastUpdateTime(receiption_time);
			lastkey = key;
//			readRaw(0, sp + 10);
		}
//		Serial.println("count incremented");
		return 0;
	}
	status = ((dtimeLen - 1) << 6) | ((keylen - 1) << 4)
			| (getFromEEPROM(sp, 1) & 0x0f);

	sp = pushToEEPROM(sp, 1, status);
	sp = pushToEEPROM(sp, dtimeLen, dTime);
	sp = pushToEEPROM(sp, keylen, key);
	sp = pushToEEPROM(sp, 1, 1);
	pushToEEPROM(sp, 1, ((sp - sp2) & 0x0f));

	stackpointer(sp);
	lastUpdateTime(receiption_time);
	incStackCount();

	lastkey = key;
//	Serial.println("added to Database");
//	readRaw(0, sp + 10);
//	pop (sp);
}

void DatabaseManager::pop() {
	uint16_t address = stackpointer();
	uint16_t beginaddress = stackStartAddress();
	uint8_t address1;
	Serial.print("[");
	while (address - beginaddress) {
		address = address - (getFromEEPROM(address, 1) & 0x0f);
		address1 = address;
		uint32_t temp = getFromEEPROM(address, 1);
		uint32_t buttonTimeInstant = ((temp >> 6) & 0x03) + 1;
		uint32_t key = ((temp >> 4) & 0x03) + 1;
		uint8_t keypressCount = 0;
		uint8_t totalByteCount = 0;

		address++;

		temp = getFromEEPROM(address, buttonTimeInstant) + lastDumpTime();
		address += buttonTimeInstant;
		buttonTimeInstant = temp;

		temp = getFromEEPROM(address, key);
		address += key;
		key = temp;

		keypressCount = getFromEEPROM(address++, 1);

		totalByteCount = getFromEEPROM(address, 1) & 0xff;

		Serial.print("{'Key':");
		Serial.print(key, HEX);
		Serial.print(",'Count':");
		Serial.print(keypressCount);
		Serial.print(",'Time':");
		Serial.print(buttonTimeInstant);
		Serial.println("}");

		address = address1;
	}
	Serial.print("]");
}
