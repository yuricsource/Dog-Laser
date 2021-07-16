#include "CodeReceiver.h"
#include "Hardware.h"

namespace Hal
{
void CodeReceiver::Stop()
{
    _timer->Stop();
    _waitCounter = 0;
    _state = CodeLearnerState::None;
    _tOnCounter = 0;
}

void CodeReceiver::Start()
{
    _state = CodeLearnerState::None;
    _gpio->ConfigInput(_pin);
    _bufferIndex = 0;
    memset(_data, 0, sizeof(_data));
    _codeReceived = false;
    _codeIndex = 0;
    memset(_codes, 0, sizeof(_codes));
    _waitCounter = 0;
    _tOnCounter = 0;

    _timer->SetTimer(SamplingFrequency);
    _timer->Start();
}

void CodeReceiver::Reset()
{
    Stop();
    Start();
}

void CodeReceiver::PrintResult()
{
    printf("\n");
    printf("_bufferIndex:%d, MinimunBitsAllowed:%d, WaitCount:%d, HighOrLowCount:%d",
            _bufferIndex, _minimunBitsAllowed, _waitCount, _highOrLowCount);

    memset(_codes, 0, sizeof(_codes));
    // printf("Code in bits:\n");
    for(uint8_t i = 0; i < _bufferIndex; i++)
    {
        if (i%32 == 0)
            printf("\n");

        printf("%d", _data[i]);

        uint8_t buffer32Index = i / 32;

        if (_data[i] != 0)
            _codes[buffer32Index] += (1 << (31 - (i % 32)));
    }
    printf("\n");
    // printf("Code in 32 bits Hex\n");
    for (uint8_t i = 0; i < 3; i++)
        printf("0x%08X ", _codes[i]);
        
    printf("\n");
}

void CodeReceiver::PrintCode()
{
    uint32_t code[3] = {};

    for(uint8_t i = 0; i < _bufferIndex; i++)
    {
        uint8_t buffer32Index = i / 32;

        if (_data[i] != 0)
            code[buffer32Index] += (1 << (31 - (i % 32)));
    }
    printf("\n");
    for (uint8_t i = 0; i < 3; i++)
        if (i == 2)
            printf("0x%08X};", code[i]);
        else
            printf("0x%08X, ", code[i]);
    
}

void IRAM_ATTR CodeReceiver::TimerCallback()
{
    switch(_state)
    {
        case CodeLearnerState::None:
        {
            if (_gpio->Get(_pin) == _logicLevel)
            {
                _waitCounter = 0;
            }
            else
            {
                _waitCounter++;
                if (_waitCounter > _waitCount || _skipPreparation)
                {
                    if (_skipPreparation)
                        _state = CodeLearnerState::Logging;
                    else
                        _state = CodeLearnerState::Ready;
                }
            }
        }
        break;
        case CodeLearnerState::Ready:
        {
            if (_gpio->Get(_pin) == _logicLevel)
            {
                _state = CodeLearnerState::Logging;
                _tOnCounter++;
            }
        }
        break;
        case CodeLearnerState::Logging:
        {
            if (_gpio->Get(_pin) == _logicLevel)
            {
                _tOnCounter++;
            }
            else
            {
                if (_tOnCounter != 0)
                {
                    if (_tOnCounter > _highOrLowCount)
                    {
                        _data[_bufferIndex++] = _logicLevel;
                    }
                    else
                    {
                        _data[_bufferIndex++] = !_logicLevel;
                    }
                    _tOnCounter = 0;
                    _waitCounter = 0;
                }
                else
                {
                    _waitCounter++;
                    if (_waitCounter > _waitCount)
                    {
                        if (_bufferIndex < _minimunBitsAllowed)
                        {
                            _bufferIndex = 0;
                            _waitCounter = 0;
                            _tOnCounter = 0;
                            _state = CodeLearnerState::None;
                        }
                        else
                        {
                            _state = CodeLearnerState::Finished;
                        }
                    }
                }
            }
        }
        break;
        case CodeLearnerState::Finished:
        {
            if (_codeReceived == false)
            {
                _codeReceived = true;
                // for(uint8_t i = 0; i < _bufferIndex; i++)
                // {
                //     uint8_t buffer32Index = i / 32;
                //     assert(buffer32Index <= 1);
                //     if (_data[i] != 0)
                //     _codes[_codeIndext] += (1 << (31 - (i % 32)));
                // }
                // if (_codeIndext < 3)
                //     _codeIndext++;
                // else
                //     _codeIndext = 0; 
            }
        }
        break;
        default:
            _state = CodeLearnerState::None;
    }
}
}