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

#include "SPIPort.h"

#include "Program.h"

SPIBuffer spiBuf;

/*
struct PTComm {
	uint8_t command;
	uint32_t 
};
*/

ISR(SPI0_INT_vect) {
	// Handle interrupt
	spiBuf.inBuf[spiBuf.state & SPI_BM] = SPI0.DATA;
	SPI0.DATA = spiBuf.outBuf[spiBuf.state & SPI_BM];
	spiBuf.state++;
	if(spiBuf.state> SPI_BUFFER_SIZE) {
		spiBuf.state = 0;
	}
	SPI0.INTFLAGS = SPI_RXCIF_bm;
}


SPIPort::SPIPort(SPI_t& spi) : _spi(spi) {
	// Initialisation
	_spiBuf.state = 0;
}

void SPIPort::StartSPISlave() {
	
    PORTA.DIR &= ~PIN4_bm; /* Set MOSI pin direction to input */
    PORTA.DIR |= PIN5_bm; /* Set MISO pin direction to output */
    PORTA.DIR &= ~PIN6_bm; /* Set SCK pin direction to input */
    PORTA.DIR &= ~PIN7_bm; /* Set SS pin direction to input */

    SPI0.CTRLA = SPI_DORD_bm        /* LSB is transmitted first */
    | SPI_ENABLE_bm      /* Enable module */
    & (~SPI_MASTER_bm);     /* SPI module in Slave mode */

    SPI0.INTCTRL = SPI_IE_bm; /* SPI Interrupt enable */	
}
