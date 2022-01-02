/*
 * Protocol.h
 *
 * Created: 07/02/2021 20:49:35
 *  Author: Chris
 */ 

#pragma once

#include "Hardware.h"
#include "StepperDriver.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t bufSize;
	uint16_t tick;
	int16_t tick_error;
	StepperSettings raMotor;
	StepperSettings decMotor;	
} ProtocolInfo;

#ifdef __cplusplus
};
#endif

class ProtocolDriver {
	protected:
		uint8_t _debug_state;
		ProtocolInfo info;
		void PrintStepperSettings(StepperSettings &settings, uint8_t debug_offset);
	public:
		ProtocolDriver() { };
		void UpdateProtocolInfo(uint16_t tick, int16_t tick_error, StepperSettings raMotor, StepperSettings decMotor);
		void PrintBuffer();
		inline void RestartDebug() {
			_debug_state = 0;
		}
		inline ProtocolInfo *GetProtocolBuffer() {
			return &info;
		}
};
