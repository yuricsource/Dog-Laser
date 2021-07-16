
#include "Timer.h"
//#define DEBUG_TIMER
namespace Hal
{

	Timer::Timer(TimerInterruptHandler *itrpHandler, TimerSelect timerSelected) : interruptHandler(itrpHandler)
	{
		switch (timerSelected)
		{
		case TimerSelect::Timer0:
			this->Preemption = Preemption::TIMER0;
			break;
		case TimerSelect::Timer1:
			this->Preemption = Preemption::TIMER1;
			break;
		}
	}

	Timer::~Timer()
	{
	}

	void Timer::Initlialize()
	{
#ifdef DEBUG_TIMER
		printf("Setting timer callback\n");
#endif
		// Initialize timer with 1 Hz
		this->Frequency = 1;

		if (this->Preemption == Preemption::TIMER0)
			this->AutoReload = false;
		else
			this->AutoReload = true;

		for (uint8_t i = 0; i < MaxTimerCallBack; i++)
		{
			callbackList[i] = nullptr;
		}

		interruptHandler->SetCallback(this);
	}

	void IRAM_ATTR Timer::InterruptCallback()
	{
		// Call all interruptions registered in this timer
#ifdef DEBUG_TIMER
		printf("Timer::InterruptCallback()\n");
#endif

		for (uint8_t i = 0; i < MaxTimerCallBack; i++)
		{
			if (callbackList[i] != nullptr)
			{
				callbackList[i]->TimerCallback();
#ifdef DEBUG_TIMER
				printf("CallbackList %d: OKAY\n", i);
			}
			else
				printf("CallbackList %d: NULL\n", i);
#else
			}
#endif
		}
	}

	void Timer::Start()
	{
	#ifdef DEBUG_TIMER
		printf("\n\nStart Timer %d, Freq: %d\n\n", (this->Preemption == Preemption::TIMER0) ? 0 : 1, this->Frequency);
	#endif
		if (Frequency != 0)
			interruptHandler->Enable(this);
	}

	void Timer::Stop()
	{
		interruptHandler->Disable(this->Preemption);
	}

	void Timer::SetTimer(uint32_t frequency)
	{
		if (frequency == 0)
			return;

		this->Frequency = (frequency + frequency / 25);
		interruptHandler->SetFrequency(this);
	}

	bool Timer::AddCallback(Timer::Callback *timerCallback)
	{

#ifdef DEBUG_TIMER
		printf("Timer::AddCallback()\n");
#endif
printf("Timer::AddCallback()\n");
		if (timerCallback == nullptr)
			return false;

		for (uint8_t i = 0; i < MaxTimerCallBack; i++)
		{
			if (callbackList[i] == nullptr)
			{
				callbackList[i] = timerCallback;
				printf("Timer::AddCallback() OK\n");
				return true;
			}
		}
		printf("Timer::AddCallback() Failed\n");
		return false;
	}

} // namespace Hal
