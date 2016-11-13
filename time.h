enum DayOfWeek {Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday}; //день недели
struct time {
  DayOfWeek day; //Текущий день недели
  int hour, min, sec; //часы, минуты, секунды

  time() {}; //Конструктор. Такая реализация необходима в связи с особенностями языка Arduino C++

  time(DayOfWeek Day, int Hour, int Min, int Sec) { //конструктор
    day = Day;
    hour = Hour;
    min = Min;
    sec = Sec;
  }

  int getSecVal() { //полчение разницы между текущем временем и началом суток в секундах
    return hour * 60 * 60 + min * 60 + sec;
  }

  void plus_sec() { //прибавление одной секунды к текущему времени
    sec++;
    if (sec >= 60) {
      min++;
      sec = 0;
    }

    if (min >= 60) {
      hour++;
      min = 0;
    }

    if (hour >= 24) {
      day = (DayOfWeek)((int)day + 1);
      hour = 0;
    }

    if ((int)day > (int)DayOfWeek::Sunday) {
      day = Monday;
    }

  }

  void write() { //Вывод текущего времени на Универсальную Последовательную Шину
    Serial.print(day); //вывод даты
    Serial.print(" ");
    Serial.print(hour); //часов
    Serial.print(":");
    Serial.print(min); //минут
    Serial.print(":");
    Serial.print(sec); //секунд
    Serial.println();
  }
};
