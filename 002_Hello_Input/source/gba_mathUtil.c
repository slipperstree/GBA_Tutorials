#include "gba_mathUtil.h"

int32 __qran_seed = 42;

int32 sqran(int32 a_val)
{
	int32 old = __qran_seed;
	__qran_seed = a_val;
	return old;
}

int32 qran()
{
	__qran_seed = 1664525 * __qran_seed + 1013904223;
	return (__qran_seed >> 16) & 0x7FFF;
}

int32 qran_range(int32 a_min, int32 a_max)
{
	return (qran()*(a_max - a_min) >> 15) + a_min;
}

int32 sign(int32 a_val)
{
	return (!(a_val & 0x80000000) && !a_val) ? 1 : -1;
}

int32 abs(int32 a_val)
{
	int32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}