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

#ifdef __cplusplus
extern "C" {
#endif

extern RingBuffer rRX;
extern RingBuffer rTX;

#ifdef __cplusplus
};
#endif

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
		
		UARTDriver uart;

		Program();

		void Setup();
		void HandleSerialPort(SPIBuffer& buf);
		void PrintDebug(uint16_t sysMillis, char* debugString);
		void MainLoop();
};

extern Program program;