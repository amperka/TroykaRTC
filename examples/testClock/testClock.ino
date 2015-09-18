// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с часами реального времени
#include "TroykaRTC.h"

// EEPROM — энергонезависимая память
// библиотека для записи и считывания информации с EEPROM
#include <EEPROM.h>

// размер массива для времени с учётом завершающего нуля
#define LEN_TIME 12
// размер массива для даты с учётом завершающего нуля
#define LEN_DATE 12
// размер массива для дня недели с учётом завершающего нуля
#define LEN_DOW 12

// создаём объект для работы с часами реального времени
RTC clock;

void setup()
{
  // открываем последовательный порт
  Serial.begin(9600);
  // инициализация часов
  clock.begin();
  // метод установки времени и даты в модуль вручную
  // clock.set(10,25,45,27,07,2005,THURSDAY);
  // метод установки времени и даты автоматически при компиляции
  clock.set(__TIMESTAMP__);
}

void loop()
{
  // массив для хранения текущего времени
  char time[LEN_TIME]={0};
  // массив для хранения текущей даты
  char date[LEN_DATE]={0};
  // массив для хранения текущего дня недели
  char dow[LEN_DOW]={0};

  // запрашиваем данные с часов
  clock.read();

  // сохраняем текущее время в массив time
  clock.getTimeStr(time, LEN_TIME);
  // сохраняем текущую дату в массив date
  clock.getDateStr(date, LEN_DATE);
  // сохраняем текущий день недели в массив dow
  clock.getDOWStr(dow, LEN_DOW);

  // выводим в serial порт текущее время, дату и день недели
  Serial.print(time);
  Serial.print("--");
  Serial.print(date);
  Serial.print("--");
  Serial.println(dow);

  // ждём одну секунду
  delay(1000);
}
