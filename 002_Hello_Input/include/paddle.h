#ifndef __PONG_PADDLE_H__
#define __PONG_PADDLE_H__

#include "gba_types.h"

typedef struct Paddle
{
	int32 x, y, width, height;
	uint16 color;
}Paddle;

extern void InitPaddle(Paddle* a_paddle, int32 a_x, int32 a_y, int32 a_width, int32 a_height, int16 a_color);
extern void MovePaddle(Paddle* a_paddle, int32 a_val);
extern void ClearPaddle(Paddle* a_paddle);
extern void DrawPaddle(Paddle* a_paddle);

#endif //__PONG_PADDLE_H__