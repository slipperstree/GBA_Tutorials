#include "ball.h"
#include "gba_mathUtil.h"
#include "gba_drawing.h"

void StartBall(Ball* a_ball) {
	while (a_ball->xDir == 0)
	{
		a_ball->xDir = qran_range(-1, 2);
	}
	a_ball->yDir = qran_range(-1, 2);
}

void InitBall(Ball* a_ball, int32 a_x, int32 a_y, int32 a_size, int16 a_color)
{
	a_ball->x = a_x;
	a_ball->y = a_y;
	a_ball->size = a_size;
	a_ball->color = a_color;
	a_ball->xDir = a_ball->yDir = 0;
	StartBall(a_ball);
}

void MoveBall(Ball* a_ball)
{
	bool reset = false;
	a_ball->y += a_ball->yDir;
	if (a_ball->y < 0)
	{
		a_ball->y = 0;
		a_ball->yDir *= -1;
	}
	if (a_ball->y > SCREEN_H - a_ball->size)
	{
		a_ball->y = SCREEN_H - a_ball->size;
		a_ball->yDir *= -1;
	}

	a_ball->x += a_ball->xDir;
	if (a_ball->x < 0 || a_ball->x > SCREEN_W - a_ball->size)
	{
		reset = true;
	}

	if (reset)
	{
		a_ball->x = 115; a_ball->y = 75;
		a_ball->xDir = 0;
		StartBall(a_ball);
	}

}

void ClearBall(Ball* a_ball)
{
	drawRect(a_ball->x, a_ball->y, a_ball->size, a_ball->size, setColor(0,0,0));
}

void DrawBall(Ball* a_ball)
{
	drawRect(a_ball->x, a_ball->y, a_ball->size, a_ball->size, a_ball->color);
}
