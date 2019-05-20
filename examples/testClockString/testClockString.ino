// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с часами реального времени
#include <TroykaRTC.h>

// создаём объект для работы с часами реального времени
RTC clock;

void setup() {
  // открываем последовательный порт
  Serial.begin(9600);
  // инициализация часов
  clock.begin();
  // метод установки времени и даты в модуль вручную
  // clock.set(10,25,45,27,07,2005,THURSDAY);    
  // метод установки времени и даты автоматически при компиляции
  clock.set(__TIMESTAMP__);
  // что бы время менялось при прошивки или сбросе питания
  // закоментируйте оба метода clock.set();
}

void loop() {
  // переменные для времени, даты и дня недели
  String timeStr;
  String dateStr;
  String weekDayStr;
  // запрашиваем данные с часов
  clock.read();
  // получаем текущее время, дату и день недели в переменные
  clock.getTimeStamp(timeStr, dateStr, weekDayStr);
  // выводим в serial порт текущее время, дату и день недели
  Serial.print(timeStr);
  Serial.print("\t");
  Serial.print(dateStr);
  Serial.print("\t");
  Serial.println(weekDayStr);
  // ждём одну секунду
  delay(1000);
}
