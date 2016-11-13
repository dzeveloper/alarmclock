#include "time.h"

struct TimeInterval { //интервал времени
  time t1, t2;

  TimeInterval() {};

  TimeInterval(time* time1, time* time2) { //конструктор
    t1 = *time1;
    t2 = *time2;
  }

  bool inBounds(time t) { //Возвращает true, если t находится между t1 и t2, иначе false.
    if (t.day >= t1.day && t.day <= t2.day) //если находимся в интервале дней недели
      if (t.getSecVal() >= t1.getSecVal() && t.getSecVal() < t2.getSecVal()) // сравнение секундного значения
        return 1;
    return 0;
  }
};

struct AlarmIntervals { //несколько временных интервалов
  const static int count = 10; //и их количество
  TimeInterval* intervals[count]; //массив временных интервалов

  AlarmIntervals() {  //конструктор
    for (int i = 0; i < count; i++)//инициализируем массив значениями NULL
      intervals[i] = NULL;
  }

  void set(int number, TimeInterval* interval) { //Запись в элемент массива number временного интервала interval
    intervals[number] = interval;
  }

  bool inBounds(time t) { //если время t находится в хотя бы одном временном интервале AlarmIntervals, то возвращается true, иначе false
    for (int i = 0; i < count; i++)
      if (intervals[i] != NULL && intervals[i]->inBounds(t))
        return 1;

    return 0;
  }
};

class Light { //класс для управления подачей тока на розетку и управления освещением
    bool condition;//состояния освещения
  public:
    static const int pin = 4, led_pin = 13; //pin - номер ножки микроконтроллера, к которой подключено реле; led_pin - номер ножки светодиода индикации

    Light() { //констуктор
      condition = 0;
    }

    bool getCondition() { //получение состояния освещения
      return condition;
    }

    void turn_on() { //включение освещения
      if (condition == 1)
        return; //действие не требуется, если освещение уже включено
      digitalWrite(pin, HIGH); //подаётся на реле ток
      digitalWrite(led_pin, HIGH); //подаётся на светодиод индикации ток
      condition = 1; //изменение состояния освещения
      Serial.println("LIGHT IS ON"); //отчитываемся на шину USB о включении освещения
    }

    void turn_off() { //выключение освещения
      if (condition == 0)
        return; //действие не требуется, если освещение уже выключено
      digitalWrite(pin, LOW); //прекращается подача тока на реле
      digitalWrite(led_pin, LOW); //прекращается подача тока на светодиод индикации
      condition = 0; //изменение состояния освещения
      Serial.println("LIGHT IS OFF"); //отчитываемся на шину USB о выключении освещения
    }
};

AlarmIntervals alarms; //набор временных интервалов
time current(Tuesday, 0, 21, 0);
Light light; //глобальный объект управления освещением
int testLightTime = 0, prevPinVal = 0;

void setup() { 
  Serial.begin(9600); 
  pinMode(Light::pin, OUTPUT); 
  pinMode(Light::led_pin, OUTPUT); 

  testLightTime = 5;
  alarms.set(0, new TimeInterval(new time(current.day, current.hour, current.min, current.sec + 5), new time(current.day, current.hour, current.min, current.sec + 15)));
  alarms.set(1, new TimeInterval(new time(Monday, 6, 45, 0), new time(Sunday, 7, 25, 0)));
}

void loop() { //функция, которая выполняется непрерывно во время работы микроконтроллера. Вызывается сразу после setup()
  delay(1000); //Ожидание в 1 сек.
  current.plus_sec(); //прибавляем секунду к текущему времени
  current.write(); //выводим на шину USB значение текущего времени
  Serial.println(prevPinVal);

  int pinBuf = digitalRead(A5);
  if (testLightTime <= 0) {
    if (pinBuf != prevPinVal) {
      testLightTime = 5;
      light.turn_on();
    }
    else {
      if (alarms.inBounds(current)) //если текущее время находится в хотя бы одном интервале alarms
        light.turn_on(); //то включается освещение
      else
        light.turn_off(); //иначе освещение выключается.
    }
  }
  else 
    testLightTime--;
  
  prevPinVal = pinBuf;
}
