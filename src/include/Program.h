/*
 * Program.h
 *
 * Created: 06/02/2021 22:27:13
 *  Author: Chris
 */ 


#pragma once

#include "Hardware.h"

#include "Timer.h"
#include "Clock.h"
#include "StepperDriver.h"
#include "SPIPort.h"
#include "UART.h"

#include "Protocol.h"

class Program {
	protected:
		FILE uart_stream;
		uint16_t _rtcTick;
		uint16_t _tick_err;
		uint16_t _sysMillisDebug;
		ProtocolInfo pBuf;
		ProtocolDriver pDriver;
		void SetupUnusedPins();
	public:
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

		void Setup();
		void HandleSerialPort(SPIBuffer& buf);
		void PrintDebug(uint16_t sysMillis, char* debugString);
		void MainLoop();
		timestamp_t GetTimeStamp(uint16_t rtc, uint16_t ms, uint16_t us);
};

extern Program program;