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

#include "ring_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
};
#endif


class UARTDriver {
	private:
		uint16_t _baud;
		RingBuffer* _ringRX;
		RingBuffer* _ringTX;
	public:
		USART_t &_usart;
        PORT_t &_port;
		UARTDriver(USART_t &usart, PORT_t &port, uint16_t baud, RingBuffer* ringRx, RingBuffer* ringTX);
		void SetupUART();
		void sendChar(char c);
		void sendString(const char* str);
};

#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)