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

#include "Clock.h"
#include "Program.h"

ISR(RTC_CNT_vect)
{
	program.clock.ISR_CNT();
}

ISR(RTC_PIT_vect)
{
	program.clock.ISR_PIT(program.timer);
}

Clock::Clock(RTC_t& rtc) : _rtc(rtc)
{
	tick = 0;
}

/*
uint16_t Clock::GetTick()
{
	return _rtc.CNT ;
}
*/

void Clock::SetPeriod(uint16_t period)
{
	//rtc_set_period(0x0400);

	while(_rtc.STATUS & RTC_PERBUSY_bp) { /* wait */ ;}
	_rtc.PER = period;
	RTC.INTCTRL |= RTC_OVF_bm;
}

void Clock::SetPIT() {
	while(_rtc.PITSTATUS & RTC_CTRLBUSY_bm) { /* wait */ ;}
	_rtc.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;
	_rtc.PITINTCTRL |= RTC_PI_bm;
	_rtc.PITDBGCTRL = RTC_DBGRUN_bm;
}

void Clock::Setup()
{
	// Setup a 1s Clock
	
	// http://ww1.microchip.com/downloads/en/AppNotes/TB3213-Getting-Started-with-RTC-90003213A.pdf
	
	// Clear Enable bit
	uint8_t t = CLKCTRL.XOSC32KCTRLA & ~CLKCTRL_ENABLE_bm;
	_PROTECTED_WRITE(CLKCTRL.XOSC32KCTRLA,t);
	
	// Wait for it...
	while(CLKCTRL.MCLKCTRLA & CLKCTRL_XOSC32KS_bm) { /* Wait */ ; }
	
	// Source Select (External Crystal)
	t = CLKCTRL.XOSC32KCTRLA & ~CLKCTRL_SEL_bm;
	_PROTECTED_WRITE(CLKCTRL.XOSC32KCTRLA,t);
	
	// Enable oscillator
	t = CLKCTRL.XOSC32KCTRLA | CLKCTRL_ENABLE_bm;
	_PROTECTED_WRITE(CLKCTRL.XOSC32KCTRLA,t);
	
	while (_rtc.STATUS) { /* Wait */ ; }

	_rtc.CLKSEL = RTC_CLKSEL_TOSC32K_gc;
	_rtc.DBGCTRL = RTC_DBGRUN_bm;
	
	_rtc.CTRLA = RTC_PRESCALER_DIV32768_gc | RTC_RTCEN_bm | RTC_RUNSTDBY_bm;
}
