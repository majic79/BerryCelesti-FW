/*
 * ring_buffer.h
 *
 * Created: 02/12/2021 23:13:08
 *  Author: Chris
 */ 


#pragma once

#include <assert.h>
// Ring Buffer implementation

#include "Hardware.h"

#define BUFFER_SIZE 255

class RingBuffer {
	private:
	volatile uint8_t _write_offset;
	volatile uint8_t _read_offset;
	uint8_t _size;
	uint8_t *_buffer;
	public:
	RingBuffer(uint8_t *buffer, uint8_t size) {
		_write_offset = 0;
		_read_offset = 0;
		_buffer = buffer;
		_size = size;
	}

	inline uint8_t get_next(uint8_t cur_offset) {
		return (cur_offset >= (_size - 1) ? 0 : cur_offset + 1);
	}

	inline uint8_t get_next_write() {
		return get_next(_write_offset);
	}

	inline uint8_t get_next_read() {
		return get_next(_read_offset);
	}

	inline bool ring_is_full() {
		return (_read_offset == get_next_write());
	}

	inline bool ring_is_empty() {
		return (_read_offset == _write_offset);
	}

	inline uint8_t buffer_get() {
		assert(!ring_is_empty());

		uint8_t data = _buffer[_read_offset];
		_read_offset = get_next_read();
		return data;
	}

	inline void buffer_put(uint8_t data) {
		assert(!ring_is_full());
		_buffer[_write_offset] = data;
		_write_offset = get_next_write();
	}
};
