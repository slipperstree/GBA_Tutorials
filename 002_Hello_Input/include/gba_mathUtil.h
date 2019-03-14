#ifndef __GBA_MATHUTIL_H__
#define __GBA_MATHUTIL_H__

#include "gba_types.h"

extern int32 __qran_seed;
extern int32 sqran(int32 a_val);
extern int32 qran();
extern int32 qran_range(int32 a_min, int32 a_max);
extern int32 sign(int32 a_val);
extern int32 abs(int32 a_val);

#endif //__GBA_MATHUTIL_H__
