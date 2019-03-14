#include "paddle.h"
#include "gba_mathUtil.h"
#include"gba_drawing.h"

void InitPaddle(Paddle* a_paddle, int32 a_x, int32 a_y, int32 a_width, int32 a_height, int16 a_color)
{
	a_paddle->x = a_x;
	a_paddle->y = a_y;
	a_paddle->width = a_width;
	a_paddle->height = a_height;
	a_paddle->color = a_color;
}

void MovePaddle(Paddle* a_paddle, int32 a_val)
{
	a_paddle->y += a_val;
	if (a_paddle->y < 0)
	{
		a_paddle->y = 0;
	}
	if (a_paddle->y > SCREEN_H - a_paddle->height)
	{
		a_paddle->y = SCREEN_H - a_paddle->height;
	}
}

void ClearPaddle(Paddle* a_paddle)
{
	drawRect(a_paddle->x, a_paddle->y, a_paddle->width, a_paddle->height, setColor(0, 0, 0));
}

void DrawPaddle(Paddle* a_paddle)
{
	drawRect(a_paddle->x, a_paddle->y, a_paddle->width, a_paddle->height, a_paddle->color);
}