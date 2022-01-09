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

#include "StepperDriver.h"
#include "Timer.h"

StepperDriver::StepperDriver(PORT_t &port, StepperSettings& settings) : _port(port), _settings(settings) {
	//_stepsPerRev = STEPS_PER_REV;
	// this->port = port;
	settings.stepState = STEP_STATE_IDLE;
}

void StepperDriver::SetupStepperPort() {
	_port.DIR = 0x7F;
	_port.OUT = 0x01;
}

void StepperDriver::SetStepping(STEPPING_STEPS stepping) {
	uint8_t p = (_port.OUT & 0x71);
	switch(stepping) {
		case STEP_1_2:
			p |= STEPPING_1_2;
			break;
		case STEP_1_4:
			p |= STEPPING_1_4;
			break;
		case STEP_1_8:
			p |= STEPPING_1_8;
			break;
		case STEP_1_16:
			p |= STEPPING_1_16;
			break;
		case STEP_1_32:
			p |= STEPPING_1_32;
			break;
		default:
			stepping = STEP_FULL;
			break;
	}
	_port.OUT = p;

	_minPeriod = ((uint16_t)STEP_PERIOD >> static_cast<uint8_t>(stepping));
	if(_minPeriod < 4) _minPeriod = 4;
	_settings.control = (_settings.control & 0xF0) | static_cast<uint8_t>(stepping) & 0x0F;
}

void StepperDriver::SetPeriod(uint16_t period, uint16_t period_error) {
	uint16_t mp = MinPeriod();
	if(period >= mp)
	{
		_settings.period = period;
		_period_error = period_error;
	}
	else
	{
		_settings.period = mp;
	}
}

void StepperDriver::Disable() {
	_port.OUTSET = PIN0_bm;
	_port.OUTCLR = PIN4_bm;
	_settings.control &= ~STEP_EN;
}

void StepperDriver::Enable() {
	// Only enable if period is set
	if(_settings.period > 0) {
		_port.OUTCLR = PIN0_bm;
		_port.OUTSET = PIN4_bm;
		uint8_t c = _settings.control & ~0x80;
		_settings.control = c | 0x80;
	}
}

void StepperDriver::SetDir(STEP_DIR dir) {
	uint8_t p = _port.OUT & ~PIN6_bm;
	_port.OUT = p | static_cast<uint8_t>(dir);
	p = _settings.control & ~0x40;
	_settings.control = p | static_cast<uint8_t>(dir);
}

void StepperDriver::SetSteps(uint32_t steps) {
	_settings.stepTarget = steps;
}

void StepperDriver::AddSteps(int16_t steps) {
	STEP_DIR dir = static_cast<STEP_DIR>(_settings.control & 0x40);
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if(dir == STEP_DIR0) {
			_settings.stepTarget += steps;
		} else {
			_settings.stepTarget -= steps;
		}
	}
}

void StepperDriver::SetStepMode(STEP_MODE mode) {
	uint8_t m = _settings.control & ~0x30;
	m |= (static_cast<uint8_t>(mode) << 4);
	_settings.control = m;
}

void StepperDriver::DoSteps(timestamp_t ts) {
	timestamp_t t;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		t = ts - _settings.tick;
	}

	switch(_settings.stepState) {
		case STEP_STATE_WAITHIGH:
			if(t >= (_settings.period >> 1)){
				_port.OUTCLR = STEP_EN;
				_settings.stepState = STEP_STATE_WAITLOW;
			}
			break;
		case STEP_STATE_WAITLOW:
			if(t >= _settings.period) {
				_settings.stepState = STEP_STATE_IDLE;
			}
			break;
		default:
			// Do nothing if we are disabled
			if( !_settings.Enabled() ) return;

			STEP_MODE mode = _settings.StepMode();
			STEP_DIR dir = _settings.StepDir();
			if((mode == STEP_MODE_CONTINUOUS) || (_settings.stepActual != _settings.stepTarget)) {
				if(t < _settings.period ) {
					// Too soon, junior..
					return;
				}

				_port.OUTSET = STEP_EN;
				_settings.stepState = STEP_STATE_WAITHIGH;

				_tick_err += _period_error;
				uint8_t s = static_cast<uint8_t>(GetStepping());
				uint16_t te = _tick_err >> s;
				if(te > 0) {
					_settings.tick++;
					_tick_err -= te << s;
				}
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
					_settings.tick += (timestamp_t)_settings.period;	// Set next tick setpoint
					if(dir == STEP_DIR0)
						_settings.stepActual++;
					else
						_settings.stepActual--;

					if(mode == STEP_MODE_CONTINUOUS)
						_settings.stepTarget = _settings.stepActual;
				}
			} else {
				// truly, we are idle
				_settings.tick = ts - _settings.period;
			}
	}
}
