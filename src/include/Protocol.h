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
#include "StepperDriver.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t bufSize;
	timestamp_t tick;
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
		void UpdateProtocolInfo(timestamp_t tick, int16_t tick_error, StepperSettings raMotor, StepperSettings decMotor);
		void PrintBuffer();
		inline void RestartDebug() {
			_debug_state = 0;
		}
		inline ProtocolInfo *GetProtocolBuffer() {
			return &info;
		}
};
