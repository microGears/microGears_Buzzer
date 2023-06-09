## microGears Buzzer

Простая и функциональная библиотека для Arduino(esp8266/esp32 & etc.), устроена таким образом, что зуммер издает звуковой сигнал или серию звуковых сигналов разной продолжительности и/или повторяется много раз, не блокируя выполнение другого кода программы.

## Содержание

- [Установка](#install)
- [Инициализация](#init)
- [Функции](#functions)
- [Использование](#usage)
- [Обратная связь](#feedback)

<a id="install"></a>

## Установка

- [Скачать библиотеку](https://github.com/microGears/microGears_Buzzer/archive/refs/heads/master.zip) .zip архивом для ручной установки:
  - Распаковать и положить в _C:\Program Files (x86)\Arduino\libraries_ (Windows x64)
  - Распаковать и положить в _C:\Program Files\Arduino\libraries_ (Windows x32)
  - Распаковать и положить в _lib_ проекта PlatformIO
  - (Arduino IDE) автоматическая установка из .zip: _Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…_ и указать скачанный архив

<a id="init"></a>

## Инициализация

```cpp
#define PIN 3

#include <Buzzer.h>

microGears_Buzzer buzzer();
microGears_Buzzer buzzer(PIN); // с указанием пина явным образом


void setup(){
    buzzer.init(PIN); // инициализация

    // ...
}
```

<a id="functions"></a>

## Функции

```cpp
void init(uint8_t pin);                                 // инициализация;
void setCallbackHandler(beep_callback_t callback);      // назначение своего обработчика для воспроизведения звука
void beep(uint16_t duration, uint32_t repeat);          // воспроизвести сигнал длительностью *duration*, повторять *repeat* кол-во раз
void beep(beep_scenario_t scenario, uint32_t repeat);   // воспроизвести сценарий(сигналов разной длительности), повторять *repeat* кол-во раз
void circulate(beep_scenario_t scenario);               // воспроизводить циклично сценарий(сигналов)
void circulate(uint16_t duration);                      // воспроизводить циклично сигнал длительностью *duration*
bool isBeep();                                          // возвращает TRUE если выполняется воспроизведение
void stop();                                            // остановить вопроизведение
void tick();                                            // обновление


#define BD_SHORT 75             // длительность короткого сигнала
#define BD_LONG 750             // длительность длинного сигнала
#define BD_SCENARIO_LENGTH 8    // максимальный размер сценария сигналов

typedef void (*beep_callback_t)(uint8_t val);           // (тип) функция-обработчик
typedef int16_t beep_scenario_t[BD_SCENARIO_LENGTH];    // (тип) сценария
```

<a id="usage"></a>

## Использование

```cpp
#define PIN 3

#include <Buzzer.h>
microGears_Buzzer buzzer();


void my_beep(uint8_t val){
    #if DEBUG > 0
        Serial.print(F("Beep: "));
        Serial.println((_val ? F("ON") : F("OFF")));
    #endif
    digitalWrite(PIN,val);
}

void setup(){
    // инициализация
    buzzer.init(PIN);

    // назначить свой обработчик для воспроизведения
    // по-умолчанию для подачи звукового сигнала используется ф-ция digitalWrite(__pin, HIGH|LOW);
    buzzer.setCallbackHandler(my_beep);

    // ...
}

void loop(){
    buzzer.tick();
    // ...
}

void some_func(){
    // воспроизвести сигнал длительностью 250 мсек., повторить 1 раз
    buzzer.beep(250);

    // если воспроизведение не запущено
    if(!buzzer.isBeep()){
        // воспроизвести сигнал длительностью 450 мсек., повторить 3 раза
        buzzer.beep(450,3);
    }

    // воспроизвести сценарий из 4-х сигналов, повторить 3 раза
    // пауза между сигналами будет равна длительности(предыдущего сигнала)
    beep_scenario_t list = {BD_SHORT, BD_LONG, BD_SHORT, BD_SHORT};
    buzzer.beep(list,3);

    // воспроизвести сценарий из 5-х сигналов, повторить 1 раз
    // пауза между сигналами будет равна длительности(предыдущего сигнала)
    // отрицательное значение длительности *-500* создает эффект "паузы/тишины" между сигналами
    beep_scenario_t list = {BD_SHORT, BD_LONG, BD_SHORT, -500, BD_SHORT, BD_SHORT};
    buzzer.beep(list);

    // воспроизводить непрерывно сигнал длительностью 250 мсек.
    // пауза между сигналами будет равна длительности
    buzzer.circulate(250);

    // воспроизводить непрерывно сценарий из 4-х сигналов
    // пауза между сигналами будет равна длительности(предыдущего сигнала)
    beep_scenario_t list = {200, 500, 250, 500};
    buzzer.circulate(list);

    // остановить воспроизведение, если оно запущено
    if(buzzer.isBeep()){
        // остановить воспроизведение
        buzzer.stop();
    }
}

```

<a id="feedback"></a>

## Обратная связь

При обнаружении ошибок или багов создавайте **Issue**

При сообщении о багах или некорректной работе библиотеки обязательно указывайте:

- Версия библиотеки
- Какой используется контроллер
- Версия Arduino IDE/PLatformIO
- Корректно ли работают ли встроенные примеры
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- Приложите (минимальный) код, в котором обнаружен баг.
