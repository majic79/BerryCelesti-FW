/*
 * Clock.h - RTC Clock interface
 *
 * Created: 01/02/2021 00:12:53
 *  Author: Chris
 */ 


#pragma once

#include "Hardware.h"

class Clock {
	protected:
	
	public:
		RTC_t& _rtc;
		uint16_t tick;
		Clock(RTC_t& rtc);
		void Setup();
		void SetPeriod(uint16_t period);
		void SetPIT();
		inline uint16_t GetTick() {
			return tick;
		}
		
		inline void ISR_PIT() {
			// Programmable Interrupt Timer
			_rtc.PITINTFLAGS = RTC_PI_bm;
		}
		
		inline void ISR_CNT() {
			// CNT increment interrupt
			_rtc.INTFLAGS = RTC_OVF_bm;
		}
};
