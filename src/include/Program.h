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

#include "Timer.h"
#include "Clock.h"
#include "StepperDriver.h"
#include "SPIPort.h"
#include "UART.h"

#include "Protocol.h"

struct Program {
	FILE uart_stream;
	uint16_t _rtcTick;
	uint16_t _tick_err;
	uint16_t _sysMillisDebug;
	ProtocolInfo pBuf;
	ProtocolDriver pDriver;
	Timer timer;
	Clock clock;
	SPIPort spi;
	StepperDriver raStepper;
	StepperDriver decStepper;

	TimerError timer_error;
	
	RingBuffer rRX;
	RingBuffer rTX;
	UARTDriver uart;

	Program();
	void SetupUnusedPins();
	void Setup();
	void HandleSerialPort(SPIBuffer& buf);
	void PrintDebug(uint16_t sysMillis, char* debugString);
	void MainLoop();
	timestamp_t GetTimeStamp(uint16_t rtc, uint16_t ms, uint16_t us);
};

extern Program program;