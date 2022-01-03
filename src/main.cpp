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