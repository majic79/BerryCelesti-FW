// This file is part of BerryCelesti-FW.
//
// BerryCelesti-FW is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// BerryCelesti-FW is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// BerryCelesti-FW. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright (C) 2022 Chris Marr - Nerdsmith UK (www.nerdsmith.co.uk)


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

struct Timer {
		TCA_t &_timer;

		uint16_t millis;
		TimerError Error;

		Timer(TCA_t &timer);

		void Setup();
		
		inline void ISRHandler() {
			_timer.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
			millis++;
		}
		
		inline void Reset() {
			millis = 0;
			_timer.SINGLE.CNT = 0;
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
