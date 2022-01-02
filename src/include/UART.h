/*
 * uart.h
 *
 * Created: 02/12/2021 23:13:43
 *  Author: Chris
 */ 

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