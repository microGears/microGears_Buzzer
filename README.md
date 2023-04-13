## microGears Buzzer Library for Arduino

**microGears_Buzzer** - the library is designed in such a way that the buzzer beeps or a series of beeps of different duration and / or repeats many times without blocking other code.

## Содержание

- [Установка](#install)
- [Инициализация](#init)
- [Функции](#functions)

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
#include <Buzzer.h>
microGears_Buzzer buzzer(PIN);
```

<a id="functions"></a>

## Функции

```cpp
void init(uint8_t pin);                             // инициализация;
void setCallbackHandler(beep_callback_t callback);  // назначение своего обработчика для воспроизведения звука
void beep(uint16_t duration, uint32_t repeat = 1);
void beep(beep_scenario_t scenario, uint32_t repeat = 1);
void circulate(beep_scenario_t scenario);
void circulate(uint16_t duration);
void stop(); // остановить вопроизведение
void tick();
```
