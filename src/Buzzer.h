#ifndef _microGears_Buzzer_h
#define _microGears_Buzzer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define BD_SHORT 75             // длительность короткого сигнала
#define BD_LONG 750             // длительность длинного сигнала
#define BD_SCENARIO_LENGTH 8    // максимальный размер сценария сигналов

#define FILLIN(a, n) a[0] = n, memcpy(((char*)a) + sizeof(a[0]), a, sizeof(a) - sizeof(a[0]));

typedef void (*beep_callback_t)(uint8_t val);           // (тип) функция-обработчик
typedef int16_t beep_scenario_t[BD_SCENARIO_LENGTH];    // (тип) сценария

class microGears_Buzzer {
    public:
    microGears_Buzzer(){};
    microGears_Buzzer(uint8_t pin) {
        init(pin);
    };

    void init(uint8_t pin) {
        __pin = pin;
        pinMode(__pin, OUTPUT);
    }

    void setCallbackHandler(beep_callback_t callback) {
        __callback = callback;
    }

    void beep(beep_scenario_t scenario, uint32_t repeat = 1) {
        int     size       = 0;
        int16_t durability = 0;

        stop();
        FILLIN(__scenario, 0);
        while ((durability = scenario[size]) && size <= BD_SCENARIO_LENGTH) {
            __scenario[size] = constrain(durability, BD_LONG * -1, BD_LONG);
            size++;
        }

        __scenario_key = 0;
        __repeat       = ((__scenario_length = size) * 2) * repeat;
    }

    void beep(uint16_t duration, uint32_t repeat = 1) {
        stop();
        duration = constrain(duration, BD_SHORT, BD_LONG);
        FILLIN(__scenario, duration);
        __scenario_key = 0;
        __repeat       = (__scenario_length = repeat) * 2;
    }

    void circulate(beep_scenario_t scenario) {
        beep(scenario, 1);
        __loop = true;
    }

    void circulate(uint16_t duration) {
        beep(duration, 1);
        __loop = true;
    }

    void stop() {
        __repeat = 0;
        __loop   = false;
        setState(LOW);
    }

    void tick() {
        static unsigned long last_request = 0;
        unsigned long        now          = millis();

        int32_t duration = getDuration();
        bool    rising   = false;
        if ((now - last_request) > abs(duration) && __repeat > 0) {

            last_request = now;
            uint8_t val  = (rising = --__repeat % 2) && duration > 0 ? HIGH : LOW;
            setState(val);

            if (!rising) {
                __scenario_key = (__scenario_key + 1) % __scenario_length;
            }

            if (__repeat == 0 && __loop) {
                __repeat = 2;
            }
        }
    }

    bool isBeep() { return __repeat > 0; }

    private:
    uint8_t         __pin;
    beep_callback_t __callback;
    uint32_t        __repeat          = 0;
    bool            __loop            = false;
    beep_scenario_t __scenario        = {0};
    byte            __scenario_length = 0;
    byte            __scenario_key    = 0;

    void setState(uint8_t val) {
        if (__callback != nullptr) {
            __callback(val);
            return;
        }

        /** else self apply */
        digitalWrite(__pin, val);
    }

    int32_t getDuration() { return __scenario[__scenario_key]; }
};
#endif