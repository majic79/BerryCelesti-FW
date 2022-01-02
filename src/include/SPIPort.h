/*
 * SPI.h
 *
 * Created: 05/02/2021 21:18:12
 *  Author: Chris
 */ 

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
