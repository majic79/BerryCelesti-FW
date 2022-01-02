#include "Protocol.h"

void ProtocolDriver::UpdateProtocolInfo(uint16_t tick, int16_t tick_error, StepperSettings raMotor, StepperSettings decMotor) {
    info.bufSize = sizeof(ProtocolInfo);
    info.tick = tick;
    info.tick_error = tick_error;
    info.raMotor = raMotor;
    info.decMotor = decMotor;
}

void ProtocolDriver::PrintBuffer() {
    switch(_debug_state) {
        case 0:
            printf("P: %u\r\n", info.bufSize);
            break;
        case 1:
            printf("\tr: %u\r\n", info.tick);
            break;
        case 2:
            printf("\trt Err: %d\r\n", info.tick_error);
            break;
        case 3:
            printf("\tRA:\r\n");
            break;
        case 19:
            printf("\tDec:\r\n");
            break;
        case 100:
            _debug_state = 99;
            break;
    }
    PrintStepperSettings(info.raMotor, 10);
    PrintStepperSettings(info.decMotor, 20);
    _debug_state++;
}

void ProtocolDriver::PrintStepperSettings(StepperSettings &s, uint8_t debug_state_offset) {
    uint8_t ds = _debug_state - debug_state_offset;
    switch(ds) {
        case 0:
            // printf("\t\tr: %0x\r\n", s.reserved);
            printf("\t\tc: %0x, %0x %0x %0x\r\n", s.control, s.Enabled(), s.StepDir() , s.StepMode());
            break;
        case 1:
            printf("\t\tt: %0d\r\n", s.tick);
            break;
        case 2:
            printf("\t\ts: %0x\r\n", s.stepState);
            break;
        case 3:
            printf("\t\ta: %0d\r\n", s.accel);
            break;
        case 4:
            printf("\t\tT: %0d\r\n", s.stepTarget);
            break;
        case 5:
            printf("\t\tA: %0d\r\n", s.stepActual);
            break;
        case 6:
            printf("\t\tV: %0d\r\n", s.speed);
            break;
        case 7:
            printf("\t\tp: %0d\r\n", s.period);
            break;
    }
}