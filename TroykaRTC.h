/****************************************************************************/
//  Function: Header file for troyka-RTC
//  Hardware: DS1307
//  Arduino IDE: Arduino-1.6.5
//  Author:  Igor Dementiev
//  Date:    Sep 10,2015
//  Version: v1.0
//  by www.amperka.ru
/****************************************************************************/
#ifndef TROYKARTC_H_
#define TROYKARTC_H_

#include <Arduino.h>
#define DS1307_I2C_ADDRESS 0x68

class RTC
{
 public:
    void begin();
    void start();
    void stop();
    void set(uint8_t hour, uint8_t minute, uint8_t second, uint16_t day,
        uint8_t month, uint8_t year, uint8_t dow);

    void set(const char *compileTimeStamp);
    void read();
    void getTimeStr(char* output, uint8_t len) const;
    void getDateStr(char* output, uint8_t len) const;
    void getDOWStr(char* output, uint8_t len) const;

    uint8_t getSecond() const { return _second; }
    uint8_t getMinute() const { return _minute; }
    uint8_t getHour() const { return _hour; }
    uint8_t getDOW() const { return _dayOfWeek; }
    uint8_t getDay() const { return _day; }
    uint8_t getMonth() const { return _month; }
    uint16_t getYear() const { return _year; }
 private:
    uint8_t decToBcd(uint8_t val);
    uint8_t bcdToDec(uint8_t val);
    uint8_t _second;
    uint8_t _minute;
    uint8_t _hour;
    uint8_t _dayOfWeek;  // day of week, 1 = Monday
    uint8_t _day;
    uint8_t _month;
    uint16_t _year;
};


//Запись двухбайтового числа в память
void EEPROMWriteInt(int address, int value);


//Чтение числа из памяти
unsigned int EEPROMReadInt(int address);

#endif  // TROYKARTC_H_
