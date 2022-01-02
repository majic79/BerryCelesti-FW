/*
 * GccApplication1.cpp
 *
 * Created: 30/01/2021 22:23:38
 * Author : Chris
 */ 

#include "Hardware.h"
#include "Timer.h"
#include "Program.h"

Program program;

int UART_SendByte(char data, FILE *stream) {
	program.uart.sendChar(data);
	return 0;
}

FILE uart_stream;

// Fuses set for 16MHz clock

void avr_clock_init() {
	// 2x prescaler (8MHz Clock speed)
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc| CLKCTRL_PEN_bm);
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, !CLKCTRL_CLKOUT_bm | CLKCTRL_CLKSEL_OSC20M_gc);
	//_PROTECTED_WRITE(CLKCTRL.OSC20MCALIBA, 190);
}

int main(void) {
	fdev_setup_stream(&uart_stream, UART_SendByte, NULL, __SWR);
	stdout = &uart_stream;
	
	avr_clock_init();
	program.Setup();
	
	printf("Cal A: %u", CLKCTRL.OSC20MCALIBA);
	printf("Cal B: %u", CLKCTRL.OSC20MCALIBB);

	// Enable global interrupts
	sei();

	//program.uart.sendString("Initialised\r\n");
	printf("Init\r\n");
	//cout << "Init++";
	
	while(1) {
		program.MainLoop();
	}
	return 0;
}