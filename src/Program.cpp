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

#include "Program.h"
#include "StepperDriver.h"
#include "Timer.h"
#include "SPIPort.h"
#include "UART.h"

// Right Ascension Clock period for continuous rotation as microseconds period between pulses (full step - needs to be corrected for microstepping)
#define CLOCK_PERIOD ((1000000L / STEPS_PER_REV))

uint8_t rxbuf[BUFFER_SIZE];
uint8_t txbuf[BUFFER_SIZE];

Program::Program() : rRX(rxbuf, BUFFER_SIZE), rTX(txbuf, BUFFER_SIZE),
	uart(USART0, PORTA, (uint16_t)USART_BAUD_RATE(115200), &rRX, &rTX),
	timer(TCA0), clock(RTC),
	raStepper(PORTC, pBuf.raMotor), decStepper(PORTD, pBuf.decMotor), spi(SPI0)
{
	// Initialisation
	_sysMillisDebug = 0;
	spiBuf.outBuf = (uint8_t*)pDriver.GetProtocolBuffer();
	spiBuf.inBuf = (uint8_t*)&pBuf;
	_tick_err = 0;
}

void Program::SetupUnusedPins()
{
	// Unused pins on Port C & D
	PORTC.DIRSET = PIN7_bm;
	PORTC.OUTSET = PIN7_bm;
	PORTD.DIRSET = PIN7_bm;
	PORTD.OUTSET = PIN7_bm;

	// PORTA: SPI/USART + PWM output for motor control

	// Set Outs:   UART_TX |  F IN1  |  F IN2
//	PORTA.DIRSET = PIN0_bm | PIN2_bm | PIN3_bm;
	// Set Ins:    UART_RX
//	PORTA.DIRCLR = PIN1_bm;
	
}

void Program::Setup()
{
	clock.Setup();
	clock.SetPIT();
	timer.Setup();
	
	spi.StartSPISlave();
	uart.SetupUART();

	raStepper.SetupStepperPort();
	raStepper.SetStepping(STEP_1_32);
	raStepper.SetStepMode(STEP_MODE_CONTINUOUS);
	uint32_t period = CLOCK_PERIOD << 2;
	uint16_t pm = 0;
	uint16_t perr = 0;
	uint8_t s = static_cast<uint8_t>(raStepper.GetStepping());
	for(uint8_t a=0; a<s;a++) {
		pm <<= 1;
		pm |= 1;
	}
	perr = period | pm;
	period >>= s;
	raStepper.SetPeriod(period,perr);
	//raStepper.SetPeriod(raStepper.MinPeriod(),0);
	raStepper.SetDir(STEP_DIR0);

	decStepper.SetupStepperPort();
	decStepper.SetStepping(STEP_1_4);
	decStepper.SetStepMode(STEP_MODE_DISCRETE);
	decStepper.SetPeriod(decStepper.MinPeriod(),0);
	decStepper.SetDir(STEP_DIR0);

	raStepper.Enable();
	decStepper.Enable();

	_rtcTick = clock.GetTick();
	uint16_t ms = timer.Millis();
	uint16_t us = timer.Micros();
	
	timestamp_t ts = GetTimeStamp(_rtcTick, ms, us);

    pDriver.UpdateProtocolInfo(ts, 0, pBuf.raMotor, pBuf.decMotor);

	SetupUnusedPins();
	
	_delay_ms(2);

}

void Program::HandleSerialPort(SPIBuffer& buf)
{
	if(buf.state & 0x80) // Have we got a full frame?
	{
		buf.state = 0;
	}
}

timestamp_t Program::GetTimeStamp(uint16_t rtc, uint16_t ms, uint16_t us) {
	uint32_t ts = rtc;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		ts *= 1000;
		ts += ms;
		ts *= 1000;
		ts += us;
	}
	return ts;
}

void Program::MainLoop()
{
	uint16_t ms = timer.Millis();
	uint16_t us = timer.Micros();
	uint16_t rtc = clock.GetTick();

	timestamp_t ts = GetTimeStamp(rtc, ms, us);
	
	// Handle SPI stuff...
	//SPIBuffer& spiBuf = spi.GetBuffer();
	//HandleSerialPort(spiBuf);
	
	raStepper.DoSteps(ts);
	decStepper.DoSteps(ts);
	
	pDriver.PrintBuffer();
	if(_rtcTick != rtc) {
		_tick_err += timer.Error.Microseconds;
		_rtcTick = rtc;

        pDriver.UpdateProtocolInfo(ts, timer.Error.Microseconds, pBuf.raMotor, pBuf.decMotor);
		pDriver.RestartDebug();
		//steps = stepsPerRev;
		// Adjust RA Stepper Speed
		//raStepper.SetPeriod(raStepper.MinPeriod() + (_rtcTick & 0xFF), timer.Error.Microseconds);
		//raStepper.Enable();
		// Request a revolution at full speed
		int16_t steps = (STEPS_PER_REV << static_cast<uint8_t>(decStepper.GetStepping()));
		decStepper.AddSteps(steps);
		//decStepper.Enable();
	}
}