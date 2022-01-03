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
#include "UART.h"
#include "ring_buffer.h"
#include <string.h>

#include "Program.h"

ISR(USART0_RXC_vect) {
	// Handle interrupt
	program.rRX.buffer_put(program.uart._usart.RXDATAL);
	program.uart._usart.STATUS |= USART_RXCIF_bm;
}

ISR(USART0_TXC_vect) {
	// Handle interrupt
	program.uart._usart.STATUS |= USART_TXCIF_bm;
}

ISR(USART0_DRE_vect) {
	// Handle interrupt
	if(!program.rTX.ring_is_empty()) {
		program.uart._usart.TXDATAL = program.rTX.buffer_get();
	} else {
		program.uart._usart.CTRLA &= ~USART_DREIE_bm;
	}
}

UARTDriver::UARTDriver(USART_t &usart, PORT_t &port, uint16_t baud, RingBuffer* ringRX, RingBuffer* ringTX) : _usart(usart), _port(port) {
	_baud = baud;
	_ringRX = ringRX;
	_ringTX = ringTX;
}

void UARTDriver::SetupUART() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		_usart.BAUD = this->_baud;

		_port.DIR &= ~PIN1_bm;
		_port.DIR |= PIN0_bm;

		//_usart.CTRLA |= USART_TXCIE_bm;
		_usart.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc | USART_SBMODE_1BIT_gc;
		_usart.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	}
}

void UARTDriver::sendChar(char c)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if(_ringTX->ring_is_empty()) {
			// Enable DR interrupt
			_usart.CTRLA |= USART_DREIE_bm;
		}
		_ringTX->buffer_put(c);
	}
}

void UARTDriver::sendString(const char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		sendChar(str[i]);
	}
}

