#ifndef _microGears_Buzzer_h
#define _microGears_Buzzer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define BD_SHORT 75
#define BD_LONG 750                 // длиный сигнал
#define BD_LONG_ALT BD_LONG * -1    // пауза
#define BD_SCENARIO_LENGTH 8        // размер сценария
#define _fill(a, n) a[0] = n, memcpy(((char*)a) + sizeof(a[0]), a, sizeof(a) - sizeof(a[0]));

typedef void (*beep_callback_t)(uint8_t _val);
typedef int16_t beep_scenario_t[BD_SCENARIO_LENGTH];    // сценарий

#define BS_short4x \
    { BD_SHORT, BD_SHORT, BD_SHORT, BD_SHORT }

class microGears_Buzzer {
    public:
    microGears_Buzzer(){};

    void init(uint8_t _pin) {
        pin = _pin;
        pinMode(pin, OUTPUT);
    }

    void setCallbackHandler(beep_callback_t _callback) {
        callback = _callback;
    }

    void beep(beep_scenario_t scenario, uint32_t repeat = 1) {
        int     size       = 0;
        int16_t durability = 0;

        noBeep();
        _fill(_scenario, 0);
        while ((durability = scenario[size]) && size <= BD_SCENARIO_LENGTH) {
            _scenario[size] = constrain(durability, BD_LONG * -1, BD_LONG);
            size++;
        }

        _scenario_key = 0;
        _repeat       = ((_scenario_length = size) * 2) * repeat;
    }

    void noBeep() {
        _repeat = 0;
        _loop   = false;
        setState(LOW);
    }
    
    void tick() {
        static unsigned long last_request = 0;
        unsigned long        now          = millis();

        int32_t duration = getDuration();
        bool    rising   = false;
        if ((now - last_request) > abs(duration) && _repeat > 0) {

            last_request = now;
            uint8_t val  = (rising = --_repeat % 2) && duration > 0 ? HIGH : LOW;
            setState(val);

            if (!rising) {
                _scenario_key = (_scenario_key + 1) % _scenario_length;
            }

            if (_repeat == 0 && _loop) {
                _repeat = 2;
            }
        }
    }

    private:
    uint8_t         pin;
    beep_callback_t callback;
    uint32_t        _repeat          = 0;
    bool            _loop            = false;
    beep_scenario_t _scenario        = {0};
    byte            _scenario_length = 0;
    byte            _scenario_key    = 0;

    void setState(uint8_t _val) {
        if (callback != nullptr) {
            callback(_val);
            return;
        }

        /** else self apply */
        digitalWrite(pin, _val);
    }

    int32_t getDuration() { return _scenario[_scenario_key]; }
};
#endif