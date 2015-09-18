/****************************************************************************/
//  Function: Cpp file for troyka-RTC
//  Hardware: DS1307
//  Arduino IDE: Arduino-1.6.5
//  Author:  Igor Dementiev
//  Date:    Sep 10,2015
//  Version: v1.0
//  by www.amperka.ru
/****************************************************************************/

#include <Wire.h>
#include "TroykaRTC.h"
#include <EEPROM.h>

uint8_t RTC::decToBcd(uint8_t val) {
    return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
uint8_t RTC::bcdToDec(uint8_t val) {
    return ( (val/16*10) + (val%16) );
}

void RTC::begin() {
    Wire.begin();
}

/* Function: The clock timing will start */

void RTC::start() {     // set the ClockHalt bit low to start the rtc
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
/*Function: The clock timing will stop */
void RTC::stop() {  // set the ClockHalt bit high to stop the rtc
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    // Register 0x00 holds the oscillator start/stop bit
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
/*Function: Read time and date from RTC */
void RTC::read() {
// Reset the register pointer
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
// A few of these need masks because certain bits are control bits
    _second    = bcdToDec(Wire.read() & 0x7f);
    _minute    = bcdToDec(Wire.read());
    // Need to change this if 12 hour am/pm
    _hour      = bcdToDec(Wire.read() & 0x3f);
    _dayOfWeek  = bcdToDec(Wire.read());
    _day = bcdToDec(Wire.read());
    _month      = bcdToDec(Wire.read());
    _year      = bcdToDec(Wire.read()) + 2000;
}
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void RTC::set(uint8_t hour, uint8_t minute, uint8_t second, uint16_t day,
              uint8_t month, uint8_t year, uint8_t dow) {
    _hour = hour;
    _minute = minute;
    _second = second;
    _day = day;
    _month = month;
    _year = year-2000;
    _dayOfWeek = dow;

    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    // 0 to bit 7 starts the clock
    Wire.write(decToBcd(_second));
    Wire.write(decToBcd(_minute));
    // If you want 12 hour am/pm you need to set bit 6
    Wire.write(decToBcd(_hour));
    Wire.write(decToBcd(_dayOfWeek));
    Wire.write(decToBcd(_day));
    Wire.write(decToBcd(_month));
    Wire.write(decToBcd(_year));
    Wire.endTransmission();
}

void RTC::set(const char* compileTimeStamp) {
    int i = 0;
//  Serial.println(compileTimeStamp);

/*--------------------DayOfWeek----------------------------*/
    char dow[4];
    for (i = 0; i < 3; i++)
        dow[i] =  compileTimeStamp[i];
    dow[i] = '\0';

    if (strcmp(dow, "Mon") == 0)
        _dayOfWeek = 1;
    else if (strcmp(dow, "Tue") == 0)
        _dayOfWeek = 2;
    else if (strcmp(dow, "Wed") == 0)
        _dayOfWeek = 3;
    else if (strcmp(dow, "Thu") == 0)
        _dayOfWeek = 4;
    else if (strcmp(dow, "Fri") == 0)
        _dayOfWeek = 5;
    else if (strcmp(dow, "Sat") == 0)
        _dayOfWeek = 6;
    else if (strcmp(dow, "Sun") == 0)
        _dayOfWeek = 7;

/*--------------------------Day----------------------------*/
    _day = ((compileTimeStamp[8] - '0') * 10 + (compileTimeStamp[9] - '0'));

/*--------------------------Month---------------------------*/
    char month[4];
    for (i = 0; i < 3; i++)
        month[i] =  compileTimeStamp[i+4];
    month[i] = '\0';

    if (strcmp(month, "Jan") == 0)
        _month = 1;
    else if (strcmp(month, "Feb") == 0)
        _month = 2;
    else if (strcmp(month, "Mar") == 0)
        _month = 3;
    else if (strcmp(month, "Apr") == 0)
        _month = 4;
    else if (strcmp(month, "May") == 0)
        _month = 5;
    else if (strcmp(month, "Jun") == 0)
        _month = 6;
    else if (strcmp(month, "Jul") == 0)
        _month = 7;
    else if (strcmp(month, "Aug") == 0)
        _month = 8;
    else if (strcmp(month, "Sep") == 0)
        _month = 9;
    else if (strcmp(month, "Oct") == 0)
        _month = 10;
    else if (strcmp(month, "Nov") == 0)
        _month = 11;
    else if (strcmp(month, "Dec") == 0)
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

    unsigned int hash =  _hour * 60 * 60 + _minute  * 60 + _second;

    if (EEPROMReadInt(0) != hash) {
        EEPROMWriteInt(0, hash);
        Wire.beginTransmission(DS1307_I2C_ADDRESS);
        Wire.write((uint8_t)0x00);
    // 0 to bit 7 starts the clock
    Wire.write(decToBcd(_second));
    Wire.write(decToBcd(_minute));
    // If you want 12 hour am/pm you need to set bit 6
    Wire.write(decToBcd(_hour));
    Wire.write(decToBcd(_dayOfWeek));
    Wire.write(decToBcd(_day));
    Wire.write(decToBcd(_month));
    Wire.write(decToBcd(_year));
    Wire.endTransmission();
}
}

void RTC::getTimeStr(char* output, uint8_t len) const {
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

    len--;
    int i = 0;
    while (i < 8 && len > 0) {
        output[i] = buff[i];
        i++;
        len--;
    }
    output[i] = '\0';
}

void RTC::getDateStr(char* output, uint8_t len) const {
    char buff[10];
    if (_day < 10)
        buff[0] = '0';
    else
        buff[0] = char((_day / 10)+ 48);
    buff[1] = char((_day % 10) + 48);
    buff[2] = '.';

    if (_month < 10)
        buff[3] = '0';
    else
        buff[3] = char((_month / 10) + 48);
    buff[4] = char((_month % 10) + 48);
    buff[5] = '.';
    buff[6]=char((_year / 1000)+48);
    buff[7]=char(((_year % 1000) / 100)+48);
    buff[8]=char(((_year % 100) / 10)+48);
    buff[9]=char((_year % 10)+48);
    buff[10]='\0';

    len--;
    int i = 0;
    while (i < 10 && len > 0) {
        output[i] = buff[i];
        i++;
        len--;
    }
    output[i] = '\0';
}

void RTC::getDOWStr(char* output, uint8_t len) const {
    char buff[10];
    switch (_dayOfWeek) {
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

    int i = 0;
    len--;
    int s = strlen(buff);
    while (i < s && len > 0) {
        output[i] = buff[i];
        i++;
        len--;
    }
    output[i] = '\0';
}

//Запись двухбайтового числа в память
void EEPROMWriteInt(int address, int value)
{
    EEPROM.write(address, lowByte(value));
    EEPROM.write(address + 1, highByte(value));
}

//Чтение числа из памяти
unsigned int EEPROMReadInt(int address)
{
    byte lowByte = EEPROM.read(address);
    byte highByte = EEPROM.read(address + 1);

    return (highByte << 8) | lowByte;
}
