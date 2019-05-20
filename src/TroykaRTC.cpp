/****************************************************************************/
//  Function: Cpp file for TroykaRTC
//  Hardware: DS1307
//  Arduino IDE: Arduino-1.8.9
//  Author:  Igor Dementiev
//  Date:    May 10,2019
//  Version: v1.2.0
//  by www.amperka.ru
/****************************************************************************/

#include <Wire.h>
#include "TroykaRTC.h"

const uint8_t daysArray [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

void RTC::begin() {
    Wire.begin();
}

// the clock timing will start
void RTC::start() {
    // set the ClockHalt bit low to start the rtc
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
	// Register 0x00 holds the oscillator start/stop bit
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
	// save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
    _second = Wire.read() & 0x7f;
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
	// write seconds back and start the clock
    Wire.write((uint8_t)_second);
    Wire.endTransmission();
}

// the clock timing will stop
void RTC::stop() {
    // set the ClockHalt bit high to stop the rtc
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    // register 0x00 holds the oscillator start/stop bit
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
    // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped
    _second = Wire.read() | 0x80;
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    // write seconds back and stop the clock
    Wire.write((uint8_t)_second);
    Wire.endTransmission();
}

/****************************************************************/
// read time and date from RTC
void RTC::read() {
	// reset the register pointer
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
	// a few of these need masks because certain bits are control bits
    _second   = BcdToDec(Wire.read() & 0x7f);
    _minute   = BcdToDec(Wire.read());
    // Need to change this if 12 hour am/pm
    _hour     = BcdToDec(Wire.read() & 0x3f);
    _weekDay  = BcdToDec(Wire.read());
    _day      = BcdToDec(Wire.read());
    _month    = BcdToDec(Wire.read());
    _year     = BcdToDec(Wire.read()) + 2000;
}

// write the time that includes the date to the RTC chip 
void RTC::set(const char* compileTimeStamp) {
    int i = 0;
//   Serial.println(compileTimeStamp);

/*--------------------Week day----------------------------*/
    char dow[4];
    for (i = 0; i < 3; i++)
        dow[i] =  compileTimeStamp[i];
    dow[i] = '\0';

    if (!strcmp(dow, "Mon"))
        _weekDay = 1;
    else if (!strcmp(dow, "Tue"))
        _weekDay = 2;
    else if (!strcmp(dow, "Wed"))
        _weekDay = 3;
    else if (!strcmp(dow, "Thu"))
        _weekDay = 4;
    else if (!strcmp(dow, "Fri"))
        _weekDay = 5;
    else if (!strcmp(dow, "Sat"))
        _weekDay = 6;
    else if (!strcmp(dow, "Sun"))
        _weekDay = 7;

/*--------------------------Day----------------------------*/
    _day = ((compileTimeStamp[8] - '0') * 10 + (compileTimeStamp[9] - '0'));

/*--------------------------Month---------------------------*/
    char month[4];
    for (i = 0; i < 3; i++)
        month[i] = compileTimeStamp[i + 4];
    month[i] = '\0';
    if (!strcmp(month, "Jan"))
        _month = 1;
    else if (!strcmp(month, "Feb"))
        _month = 2;
    else if (!strcmp(month, "Mar"))
        _month = 3;
    else if (!strcmp(month, "Apr"))
        _month = 4;
    else if (!strcmp(month, "May"))
        _month = 5;
    else if (!strcmp(month, "Jun"))
        _month = 6;
    else if (!strcmp(month, "Jul"))
        _month = 7;
    else if (!strcmp(month, "Aug"))
        _month = 8;
    else if (!strcmp(month, "Sep"))
        _month = 9;
    else if (!strcmp(month, "Oct"))
        _month = 10;
    else if (!strcmp(month, "Nov"))
        _month = 11;
    else if (!strcmp(month, "Dec"))
        _month = 12;

/*--------------------------Year----------------------------*/
    _year = ((compileTimeStamp[20] - '0') * 1000
        + (compileTimeStamp[21] - '0') * 100
        + (compileTimeStamp[22] - '0') * 10
        + (compileTimeStamp[23] - '0')) - 2000;
/*--------------------------Time----------------------------*/
    _hour = ((compileTimeStamp[11] - '0') * 10
        + (compileTimeStamp[12] - '0'));

    _minute = ((compileTimeStamp[14] - '0') * 10
        + (compileTimeStamp[15] - '0'));

    _second = ((compileTimeStamp[17] - '0') * 10
        + (compileTimeStamp[18] - '0'));


    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    // 0 to bit 7 starts the clock
    Wire.write(DecToBcd(_second));
    Wire.write(DecToBcd(_minute));
    // If you want 12 hour am/pm you need to set bit 6
    Wire.write(DecToBcd(_hour));
    Wire.write(DecToBcd(_weekDay));
    Wire.write(DecToBcd(_day));
    Wire.write(DecToBcd(_month));
    Wire.write(DecToBcd(_year));
    Wire.endTransmission();
}

// write the time that manually
void RTC::set(uint8_t hour, uint8_t minute, uint8_t second, uint16_t day,
        uint8_t month, uint8_t year, uint8_t weekDay) {
    _hour = hour;
    _minute = minute;
    _second = second;
    _day = day;
    _month = month;
    _year = year - 2000;
    _weekDay = weekDay;

    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    // 0 to bit 7 starts the clock
    Wire.write(DecToBcd(_second));
    Wire.write(DecToBcd(_minute));
    // if you want 12 hour am/pm you need to set bit 6
    Wire.write(DecToBcd(_hour));
    Wire.write(DecToBcd(_weekDay));
    Wire.write(DecToBcd(_day));
    Wire.write(DecToBcd(_month));
    Wire.write(DecToBcd(_year));
    Wire.endTransmission();
}

// write the hour that manually
void RTC::setHour(int8_t hour) {
    _hour = hour;
    parsingTime();
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x02);
    Wire.write(DecToBcd(_hour));
    Wire.endTransmission();
}

// write the minute that manually
void RTC::setMinute(int8_t minute) {
    _minute = minute;
    parsingTime();
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x01);
    Wire.write(DecToBcd(_minute));
    Wire.endTransmission();
}

// write the second that manually
void RTC::setSecond(int8_t second) {
    _second = second;
    parsingTime();
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.write(DecToBcd(_second));
    Wire.endTransmission();
}

// write the day that manually
void RTC::setDay(uint8_t day) {
    _day = day;
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x04);
    Wire.write(DecToBcd(_day));
    Wire.endTransmission();
}

// write the month that manually
void RTC::setMonth(uint8_t month) {
    _month = month;
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x05);
    Wire.write(DecToBcd(_month));
    Wire.endTransmission();
}

// write the year that manually
void RTC::setYear(uint8_t year) {
    _year = year - 2000;
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x06);
    Wire.write(DecToBcd(_year));
    Wire.endTransmission();
}

// write the week day that manually
void RTC::setWeekDay(uint8_t weekDay) {
    _weekDay = weekDay;
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x03);
    Wire.write(DecToBcd(_weekDay));
    Wire.endTransmission();
}

void RTC::getTimeStamp(String& timeStr, String& dateStr, String& weekDayStr) const {
    getTime(timeStr);
    getDate(dateStr);
    getWeekDay(weekDayStr);
}

void RTC::getTimeStamp(char* time, char* date, char* weekDay) const {
    getTime(time);
    getDate(date);
    getWeekDay(weekDay);
}

void RTC::getTime(String& timeStr) const {
    char time[12];
    getTime(time);
    timeStr = String(time);
}

void RTC::getTime(char* time) const {
    char buff[8];
    if (_hour < 10)
        buff[0] = '0';
    else
        buff[0] = char((_hour / 10) + 48);
    buff[1] = char((_hour % 10) + 48);
    buff[2] = ':';

    if (_minute < 10)
        buff[3] = '0';
    else
        buff[3] = char((_minute / 10) + 48);
    buff[4] = char((_minute % 10) + 48);
    buff[5] = ':';

    if (_second < 10)
        buff[6] = '0';
    else
        buff[6] = char((_second / 10) + 48);
    buff[7] = char((_second % 10) + 48);
    buff[8] = '\0';
    int i;
    for (i = 0; i < 8; i++ ) {
        time[i] = buff[i];
    }
    time[i] = '\0';
}

void RTC::getDate(String& dateStr) const {
    char date[12];
    getDate(date);
    dateStr = String(date);
}

void RTC::getDate(char* date) const {
    char buff[10];
    if (_day < 10)
        buff[0] = '0';
    else
        buff[0] = char((_day / 10) + 48);
    buff[1] = char((_day % 10) + 48);
    buff[2] = '.';

    if (_month < 10)
        buff[3] = '0';
    else
        buff[3] = char((_month / 10) + 48);
    buff[4] = char((_month % 10) + 48);
    buff[5] = '.';
    buff[6]=char((_year / 1000)+48);
    buff[7]=char(((_year % 1000) / 100) + 48);
    buff[8]=char(((_year % 100) / 10) + 48);
    buff[9]=char((_year % 10) + 48);
    buff[10]='\0';

    int i;
    for (i = 0; i < 10; i++ ) {
        date[i] = buff[i];
    }
    date[i] = '\0';
}

void RTC::getWeekDay(String& weekDayStr) const {
    char weekDay[12];
    getWeekDay(weekDay);
    weekDayStr = String(weekDay);
}

void RTC::getWeekDay(char* weekDay) const {
    char buff[10];
    switch (_weekDay) {
        case 1: {
            strcpy(buff, "Monday");
            break;
        }
        case 2: {
            strcpy(buff, "Tuesday");
            break;
        }
        case 3: {
            strcpy(buff, "Wednesday");
            break;
        }
        case 4: {
            strcpy(buff, "Thursday");
            break;
        }
        case 5: {
            strcpy(buff, "Friday");
            break;
        }
        case 6: {
            strcpy(buff, "Saturday");
            break;
        }
        case 7: {
            strcpy(buff, "Sunday");
            break;
        }
    }

    int s = strlen(buff);
    int i;
    for (i = 0; i < s; i++ ) {
        weekDay[i] = buff[i];
    }
    weekDay[i] = '\0';
}

uint32_t RTC::dateToDays(uint32_t year, uint8_t month, uint8_t day) {
    year = year - 2000;
    uint32_t days = day;

    for (uint8_t i = 1; i < month; i++) {
        days += pgm_read_byte(daysArray + i - 1);
    }

    if ((month == 2) && (year % 4 == 0)) {
        days++;
    }

    return days + 365 * year + (year + 3) / 4 - 1;
}

uint32_t RTC::daysToSeconds(uint32_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
    return ((days * 24L + hours) * 60 + minutes) * 60 + seconds;
}

uint32_t RTC::getUnixTime() {
    uint32_t unixTime;
    unixTime = daysToSeconds(dateToDays(_year, _month, _day), _hour, _minute, _second);
    unixTime += SECONDS_FROM_1970_TO_2000;
    return unixTime;
}

uint8_t RTC::DecToBcd(uint8_t val) {
    return (val / 10 * 16) + (val % 10);
}

uint8_t RTC::BcdToDec(uint8_t val) {
    return (val / 16 * 10) + (val % 16);
}

void RTC::parsingTime() {
    if (_hour > 23)
        _hour = 0;
    else if (_hour < 0)
        _hour = 23;

    if (_minute > 59)
        _minute = 0;
    else if (_minute < 0)
        _minute = 59;

    if (_second > 59)
        _second = 0;
    else if (_second < 0)
        _second = 59;
}
