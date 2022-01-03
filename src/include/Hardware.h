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

// #define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <avr/xmega.h>

#include <util/delay.h>
#include <util/atomic.h>

// iostream library not present, so we're relying on the old C printf functions, instead of proper stream handlers
#include <stdio.h>

typedef uint32_t timestamp_t;