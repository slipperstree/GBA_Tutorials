#ifndef __PONG_PADDLE_H__
#define __PONG_PADDLE_H__

#include "gba_types.h"

typedef struct Paddle
{
	s32 x, y, width, height;
	u16 color;
}Paddle;

extern void InitPaddle(Paddle* a_paddle, s32 a_x, s32 a_y, s32 a_width, s32 a_height, u16 a_color);
extern void MovePaddle(Paddle* a_paddle, s32 a_val);
extern void ClearPaddle(Paddle* a_paddle);
extern void DrawPaddle(Paddle* a_paddle);

#endif //__PONG_PADDLE_H__