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

#include "Hardware.h"

#include "Timer.h"
#include "Program.h"

ISR(TCA0_OVF_vect) {
	program.timer.ISRHandler();
}

Timer::Timer(TCA_t &timer) : _timer(timer) {
	millis = 0;
	Error.Microseconds = 0;
	Error.Milliseconds = 0;
}

void Timer::Setup() {
	/* don't enable enable overflow interrupt - free running timer*/
	_timer.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	
	/* set Normal mode */
	_timer.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
	
	/* disable event counting */
	_timer.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
	
	/* set the period */
	_timer.SINGLE.PER = TIMER_PERIOD;
	
	_timer.SINGLE.CNT = 0;
	
	_timer.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV8_gc         /* set clock source (sys_clk/256) */
	| TCA_SINGLE_ENABLE_bm;                /* start timer */	
}
