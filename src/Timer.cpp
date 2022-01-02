/*
 * Timer.cpp
 *
 * Created: 31/01/2021 21:49:28
 *  Author: Chris
 */ 

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
