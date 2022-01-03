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

#include "Protocol.h"

#define SPI_BUFFER_SIZE sizeof(ProtocolInfo)
#define SPI_BM 0x1F

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t *inBuf;
	uint8_t *outBuf;
	uint8_t state;
} SPIBuffer;

// Command: Position/Focus/Clock/Configure:
// Configure:
//	RA/Dec
//		Enable/Disable
//		Reset
//		Microstepping
//		Accel
// Position:
//	RA Steps	- 32 bits
//	Dec Steps	- 32 bits
// Clock:
//	RA/Dec
//		Full Step period	- 32 bits

extern SPIBuffer spiBuf;

#ifdef __cplusplus
};
#endif

class SPIPort {
	protected:
		SPIBuffer _spiBuf;
	public:
		SPI_t& _spi;
		SPIPort(SPI_t& spi);
		void StartSPISlave();
		inline SPIBuffer& GetBuffer() {
			return _spiBuf;
		}
};
