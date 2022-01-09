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

#include <assert.h>
// Ring Buffer implementation

#include "Hardware.h"

#define BUFFER_SIZE 255

struct RingBuffer {
	volatile uint8_t _write_offset;
	volatile uint8_t _read_offset;
	uint8_t _size;
	uint8_t *_buffer;

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
