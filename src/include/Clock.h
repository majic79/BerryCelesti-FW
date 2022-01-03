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
