/*
 * Hardware.h
 *
 * Created: 06/02/2021 23:01:41
 *  Author: Chris
 */ 


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
