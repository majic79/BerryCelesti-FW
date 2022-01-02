#pragma once
#include "Hardware.h"

//#define STEP_DIR1 0xFF
//#define STEP_DIR0 0xBF

// https://www.daycounter.com/Calculators/Stepper-Motor-Calculator.phtml
#define STEPS_PER_REV	200

//#define STEP_PERIOD		1900 // uS delay between steps (full steps)
// Above definition for 0.4A/34mH/12V stepper motor
// Part: https://www.omc-stepperonline.com/download/17HS13-0404S-PG27.pdf

#define STEP_PERIOD		750 // uS delay between steps (full steps)
// Above definition for 1A/4.5mH/12V stepper motor
// PArt: https://www.omc-stepperonline.com/download/14HS13-0804S-PG5.pdf

//#define STEP_PERIOD		5000 // uS delay between steps (full steps)
// Failsafe testing Period: 5ms = 200 pulses per second = 60RPM max speed

#define MIN_PULSE		2 // 1.9uS delay required for minimum Step pulse according to DRV8825 datasheet

#define MAX_FREQUENCY  250000	// Max drive frequency

#define STEP_EN 0x20

enum STEP_DIR {
	STEP_DIR1=0x40,
	STEP_DIR0=0x00
};

enum STEPPING_FLAGS {
	STEP_NOFLAG = 0x00,
	STEP_M0 = 0x02,
	STEP_M1 = 0x04,
	STEP_M2 = 0x08,
};

enum STEPPING_STEPS {
	STEP_FULL = 0x00,
	STEP_1_2 = 0x01,
	STEP_1_4 = 0x02,
	STEP_1_8 = 0x03,
	STEP_1_16 = 0x04,
	STEP_1_32 = 0x05,
};

enum STEP_STATE {
	STEP_STATE_WAITHIGH,
	STEP_STATE_WAITLOW,
	STEP_STATE_IDLE,
};

enum STEP_MODE {
	STEP_MODE_DISCRETE = 0,
	STEP_MODE_CONTINUOUS = 1
};

inline STEPPING_FLAGS operator|(STEPPING_FLAGS a, STEPPING_FLAGS b) {
	return static_cast<STEPPING_FLAGS>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

#define STEPPING_1_1 STEP_NOFLAG
#define STEPPING_1_2 (STEP_M0)
#define STEPPING_1_4 (STEP_M1)
#define STEPPING_1_8 (STEP_M1 | STEP_M0)
#define STEPPING_1_16 (STEP_M2)
#define STEPPING_1_32 (STEP_M2| STEP_M0)

// 16 byte setting buffer
typedef struct {
	uint8_t control;	// Enabled, Dir, reserved, Step/Cont [4 bits] | microstepping [4 bits]
	uint8_t reserved;
	uint8_t stepState;
	uint16_t tick;		// Current "tick" (in microseconds)
	uint16_t accel;		// Steps/Sec squared
	int32_t stepTarget;		// target steps	- target position expressed in full steps
	int32_t stepActual;		// Actual steps - actual position expressed in full steps
	uint32_t speed;		// microseconds/step target
	uint16_t period;	// Period delay between steps

	inline STEP_MODE StepMode() {
		return static_cast<STEP_MODE>((control & 0x30) >> 4);
	}

	inline STEP_DIR StepDir() {
		return static_cast<STEP_DIR>(control & 0x40);
	}

	inline bool Enabled() {
		return !((control & 0x80) == 0);
	}

	inline STEPPING_STEPS Stepping() {
		return static_cast<STEPPING_STEPS>(control & 0x0F);
	}


} StepperSettings;


class StepperDriver {
	protected:
		PORT_t &_port;
		
		StepperSettings& _settings;
		//STEPPING_STEPS _stepping;
		//uint16_t _period;	// Period between steps (in microseconds)
		uint16_t _period_error;
		//uint32_t _steps;
		//uint32_t _stepTarget;
		//uint8_t _stepState;
		//uint16_t _tick;		// Current "tick" (in microseconds)
		uint16_t _tick_err;
		uint16_t _minPeriod;
		//STEP_MODE _mode;
	public:
		StepperDriver(PORT_t &port, StepperSettings &settings);
		void SetupStepperPort();
		void Enable();
		void Disable();
		void SetStepping(STEPPING_STEPS stepping);
		void SetDir(STEP_DIR dir);
		void SetPeriod(uint16_t period, uint16_t period_error);
		void SetSteps(uint32_t steps);
		void AddSteps(int16_t steps);
		void DoSteps(uint16_t sys_us);
		void SetStepMode(STEP_MODE mode);
		
		inline uint16_t MinPeriod() {
			return _minPeriod;
		}

		inline STEPPING_STEPS GetStepping()
		{
			return _settings.Stepping();
		}

		inline bool Enabled() {
			return (_port.OUT & PIN0_bm) == 0;
		}

		inline int32_t CurrentSteps() {
			return _settings.stepActual;
		}

};