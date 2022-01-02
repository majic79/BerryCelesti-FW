/*
 * RTC.cpp
 *
 * Created: 01/02/2021 00:12:42
 *  Author: Chris
 */ 

#include "Hardware.h"

#include "Clock.h"
#include "Program.h"

ISR(RTC_CNT_vect)
{
	program.clock.ISR_CNT();
	printf("RTC_CNT\r\n");
}

ISR(RTC_PIT_vect)
{
	program.clock.ISR_PIT();
	program.clock.tick++;
	
	// Calc error
	uint16_t us = program.timer.Micros();
	uint16_t ms = program.timer.Millis();
	program.timer.CalculateError(ms, us);
	//printf("%u %03u %03u\r\n", program.clock.tick, ms, us);

	program.timer.Reset();
	//printf("RTC_PIT\r\n");
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
