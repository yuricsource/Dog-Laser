#pragma once

#include <cstring>
#include <string>
#include "stdio.h"
#include <cstdint>
#include <cstdarg>
#include "stdio.h"
#include "wchar.h"
#include "Hardware.h"
#include "Timer.h"

class LearnerCode : Hal::Timer::Callback
{

public:
    LearnerCode(Hal::Gpio* gpio, Hal::Gpio::GpioIndex pin, Hal::Timer* timer, bool infraredLearner = false) : _gpio(gpio), _pin(pin), _timer(timer)
    {
        _timer->AddCallback(this);
        _gpio->ConfigInput(_pin);

        if(infraredLearner)
        { 
            // This profile is set to use a fan of 3M
            _logicLevel = true;
            _minimunBitsAllowed = 10;
            _delayLowInMiliseconds = 2.5;
            _tOnLowInMiliseconds = 0.4;
            _tOnHighInMiliseconds = 1.3;
            _tOnMiddleLevel = ((_tOnHighInMiliseconds + _tOnLowInMiliseconds) / 2);
            _waitCount = (_delayLowInMiliseconds * SamplingFrequency / 1000);
            _highOrLowCount = (_tOnMiddleLevel * SamplingFrequency / 1000);
            _skipPreparation = true;
        }
        // RF Learner
        else
        {
            _logicLevel = true;
            _minimunBitsAllowed = 60;
            _delayLowInMiliseconds = 3;
            _tOnLowInMiliseconds = 0.35;
            _tOnHighInMiliseconds = 0.7;
            _tOnMiddleLevel = ((_tOnHighInMiliseconds + _tOnLowInMiliseconds) / 2);
            _waitCount = (_delayLowInMiliseconds * SamplingFrequency / 1000);
            _highOrLowCount = (_tOnMiddleLevel * SamplingFrequency / 1000);
            _skipPreparation = false;
        }
    }
    
    enum class CodeLearnerState : uint8_t
    {
        None,
        Ready,
        Logging,
        Finished
    };

    void Stop();

    void Start();

    void TimerCallback() override;

    void PrintResult();
    void PrintCode();

    CodeLearnerState GetState()
    {
        return _state;
    }
    
    // Sampling 16k per second => period of 62.5 us
    static constexpr uint32_t SamplingFrequency = 16000; 

private:
    uint32_t _minimunBitsAllowed;
    float _delayLowInMiliseconds;
    float _tOnLowInMiliseconds;
    float _tOnHighInMiliseconds;
    float _tOnMiddleLevel;
    uint32_t _waitCount;
    uint32_t _highOrLowCount;
    bool _logicLevel;
    bool _skipPreparation;
    Hal::Gpio* _gpio;
    Hal::Gpio::GpioIndex _pin;
    Hal::Timer* _timer;
    CodeLearnerState _state = CodeLearnerState::None;
    uint32_t _waitCounter = 0;
    uint8_t _data[200] = {};
    uint16_t _bufferIndex = 0;
    uint32_t _tOnCounter = 0;
};


