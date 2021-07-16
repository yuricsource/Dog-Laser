#include "LearnerCode.h"

void LearnerCode::Stop()
{
    _timer->Stop();
    _waitCounter = 0;
    _state = CodeLearnerState::None;
    _tOnCounter = 0;
}

void LearnerCode::Start()
{
    _state = CodeLearnerState::None;
    _gpio->ConfigInput(_pin);
    _bufferIndex = 0;
    memset(_data, 0, sizeof(_data));
    _timer->Start();
}

void LearnerCode::PrintResult()
{
    printf("\n");
    printf("_bufferIndex:%d, MinimunBitsAllowed:%d, WaitCount:%d, HighOrLowCount:%d\n",
        _bufferIndex, _minimunBitsAllowed, _waitCount, _highOrLowCount);
        uint32_t code[3] = {};

    printf("Code in bits:\n");
    for(uint8_t i = 0; i < _bufferIndex; i++)
    {
        if (i%32 == 0)
            printf("\n");

        printf("%d", _data[i]);

        uint8_t buffer32Index = i / 32;

        if (_data[i] != 0)
            code[buffer32Index] += (1 << (31 - (i % 32)));
    }
    printf("\n\n");
    printf("Code in 32 bits Hex\n");
    for (uint8_t i = 0; i < 3; i++)
        printf("0x%08X ", code[i]);
        
}

void LearnerCode::PrintCode()
{
    uint32_t code[3] = {};

    for(uint8_t i = 0; i < _bufferIndex; i++)
    {
        uint8_t buffer32Index = i / 32;

        if (_data[i] != 0)
            code[buffer32Index] += (1 << (31 - (i % 32)));
    }
    printf("\n{");
    for (uint8_t i = 0; i < 3; i++)
        if (i == 2)
            printf("0x%08X};", code[i]);
        else
            printf("0x%08X, ", code[i]);
    
}

void LearnerCode::TimerCallback()
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

                _waitCounter++;
                if (_waitCounter > _waitCount)
                {
                    if (_bufferIndex < _minimunBitsAllowed)
                    {
                        _bufferIndex = 0;
                        _waitCounter = 0;
                        _state = CodeLearnerState::Ready;
                    }
                    else
                        _state = CodeLearnerState::Finished;
                }

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
            }
        }
        break;
        case CodeLearnerState::Finished:
        {
        }
        break;
        default:
            _state = CodeLearnerState::None;
    }
}