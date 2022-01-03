/*
 * Timer.h
 *
 * Created: 31/01/2021 21:48:51
 *  Author: Chris
 */ 

#pragma once

#include "Hardware.h"

#ifdef __cplusplus
extern "C" {
#endif

void TCA0_init(void);
uint16_t SysMillis();
uint16_t SysMicros();

#ifdef __cplusplus
};
#endif

#define TIMER_PERIOD 0x03E8 // 1000 ticks = 1ms
// If I could get a Divide by 7 on the clock cycles, then I could us a base2 factor for timing which would save a few multiplies
//#define TIMER_PERIOD 0x0400 // 1024 ticks = 1ms

struct TimerError {
	int16_t Milliseconds;
	int16_t Microseconds;
};

class Timer {
	private:
		uint16_t millis;
	public:
		TimerError Error;
		TCA_t &_timer;

		Timer(TCA_t &timer);
		void Setup();
		
		inline void ISRHandler() {
			_timer.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
			millis++;
		}
		
		inline void Reset() {
			millis = 0;
			//_timer.SINGLE.CNT = 0;
		}
		
		inline uint16_t Millis() {
			return millis;
		}

		inline uint16_t Micros() {
			return (uint16_t)_timer.SINGLE.CNT;
		}
		
		inline void CalculateError(uint16_t ms, uint16_t us) {
			Error.Milliseconds = ms - 1000;
			Error.Microseconds = (Error.Milliseconds * 1000) + us;
		}
	};
