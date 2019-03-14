#ifndef __PONG_BALL_H__
#define __PONG_BALL_H__

#include "gba_types.h"

typedef struct Ball
{
	int32 x, y, xDir, yDir, size;
	uint16 color;
}Ball;

extern void StartBall(Ball* a_ball);
extern void InitBall(Ball* a_ball, int32 a_x, int32 a_y, int32 a_size, int16 a_color);
extern void MoveBall(Ball* a_ball);
extern void ClearBall(Ball* a_ball);
extern void DrawBall(Ball* a_ball);

#endif //__PONG_BALL_H__