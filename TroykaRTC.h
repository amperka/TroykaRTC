/****************************************************************************/
//  Function: Header file for troyka-RTC
//  Hardware: DS1307
//  Arduino IDE: Arduino-1.8.2
//  Author:  Igor Dementiev
//  Date:    Sep 10,2015
//  Version: v1.1
//  by www.amperka.ru
/****************************************************************************/
#ifndef TROYKARTC_H_
#define TROYKARTC_H_

#include <Arduino.h>

#define SECONDS_FROM_1970_TO_2000 946684800

#define DS1307_I2C_ADDRESS 0x68

#define MONDAY      1
#define TUESDAY     2
#define WEDNESDAY   3
#define THURSDAY    4
#define FRIDAY      5
#define SATURDAY    6
#define SUNDAY      7

class RTC
{
public:
    void begin();
    void start();
    void stop();
    void read();
    void set(const char *compileTimeStamp);
    void set(uint8_t hour, uint8_t minute, uint8_t second, uint16_t day,
        uint8_t month, uint8_t year, uint8_t weekDay);
    void setHour(int8_t hour);
    void setMinute(int8_t minute);
    void setSecond(int8_t second);
    void setDay(uint8_t day);
    void setMonth(uint8_t month);
    void setYear(uint8_t year);
    void setWeekDay(uint8_t weekDay);
    void getTimeStamp(char* time, char* date, char* weekDay) const;
    void getTimeStr(char* output) const;
    void getDateStr(char* output) const;
    void getWeekDayStr(char* output) const;
    uint8_t getSecond() const { return _second; }
    uint8_t getMinute() const { return _minute; }
    uint8_t getHour() const { return _hour; }
    uint8_t getWeekDay() const { return _weekDay; }
    uint8_t getDay() const { return _day; }
    uint8_t getMonth() const { return _month; }
    uint16_t getYear() const { return _year; }
    uint32_t getUnixTime();

private:
	void parsingTime();
	uint32_t dateToDays(uint32_t year, uint8_t month, uint8_t day);
    uint32_t daysToSeconds(uint32_t days, uint8_t h, uint8_t m, uint8_t s);
    uint8_t DecToBcd(uint8_t val);
    uint8_t BcdToDec(uint8_t val);
    int8_t _second;
    int8_t _minute;
    int8_t _hour;
    uint8_t _weekDay;  // day of week, 1 = Monday
    uint8_t _day;
    uint8_t _month;
    uint16_t _year;
};

#endif  // TROYKARTC_H_
