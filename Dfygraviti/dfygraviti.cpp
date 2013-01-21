#include "dfygraviti.h"
#include <EEPROM.h>
// constructors for setting the baud rates and initializing the values
DfygravitiServer::DfygravitiServer() {
	DfygravitiServer::baud = 9600;
	irrecv.enableIRIn();
	initialize();
}

// initializes the RTC
void DfygravitiServer::initialize() {
	Wire.begin();
	RTC.begin();
	if (!RTC.isrunning())
		rtc_enabled = false;
	else
		rtc_enabled = true;
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
	//Serial.flush();
	//time = dat[3] << (3 * 8) | dat[2] << (2 * 8) | dat[1] << (1 * 8) | dat[0];
	if (rtc_enabled)
		RTC.adjust(DateTime(datex, timex));
	getRtcTime();
}

//Sends the unix time to the Phone in JSON format.. needs to be tested
void DfygravitiServer::getRtcTime() {
	DateTime now = RTC.now();
	Serial.print(now.unixtime());
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
		Serial.println(results.value, HEX);
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

//IRUserDatabase::IRUserDatabase() {
//_id =
//}

DatabaseManager::DatabaseManager() {
	byte programCode = EEPROM.read(0);
	if (programCode != DatabaseString)
		initializeDB();
}

bool DatabaseManager::initializeDB() {
	RTC.begin();
	DateTime now = RTC.now();
	EEPROM.write(0, DatabaseString);
	pushlastUpdateTime(now.unixtime());
	pushlastDumpTime(now.unixtime());
	setEEPROMLocation (INTERNAL);
	stackEntries(0);
	stackStartAddress(13);
}

bool DatabaseManager::stackStartAddress(uint8_t address){
	return pushToEEPROM(11,1,address)
}

uint8_t DatabaseManager::stackStartAddress(){
	return getFromEEPROM(11, 1);
}

bool DatabaseManager::stackEntries(uint8_t number) {
	return pushToEEPROM(10, 1, number);
}

uint8_t DatabaseManager::stackEntries() {
	return getFromEEPROM(10, 1);
}

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

uint32_t DatabaseManager::getlastUpdateTime() {
	return getFromEEPROM(1, 4);
}

uint32_t DatabaseManager::getlastDumpTime() {
	return getFromEEPROM(5, 4);
}

bool DatabaseManager::pushlastUpdateTime(uint32_t mytime) {
	return pushToEEPROM(1, 4, mytime);
}

bool DatabaseManager::pushlastDumpTime(uint32_t mytime) {
	return pushToEEPROM(5, 4, mytime);
}

bool DatabaseManager::getEEPROMLocation() {
	return getFromEEPROM(9, 1) & 0x01;
}

bool DatabaseManager::setEEPROMLocation(bool type) {
	uint8_t temp = getFromEEPROM(6, 1) | (type * 0x01);
	return pushToEEPROM(6, 1, temp);
}

uint8_t DatabaseManager::push(IRUserDatabase db) {
	pushToEEPROM()
}

