//---------------------------------------------------------------------------------
// This is a small header file used to define types that are used for GBA development
// these types are named according to their signedness and their bit length
// u8 represents an unsigned integer that is 8 bits in size, this would be an unsigned
// char on 0x86 systems. 
// These are defined like this for simplicity and to keep code in appropriate spaces
//---------------------------------------------------------------------------------
#pragma once

#ifndef __GBA_TYPES_H__
#define __GBA_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;

typedef volatile uint8_t		v_uint8;
typedef volatile uint16_t		v_uint16;
typedef volatile uint32_t		v_uint32;
typedef volatile int8_t			v_int8;
typedef volatile int16_t		v_int16;
typedef volatile int32_t		v_int32;

#endif //__GBA_TYPES_H__