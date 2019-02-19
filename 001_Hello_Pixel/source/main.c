#include "Intellisense.h"


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

#define REG_DISPLAYCONTROL *((v_uint32*)(0x04000000))
#define VIDEOMODE_3 0x0003
#define BGMODE_2	0x0400


#define SCREENBUFFER ((v_int16*)(0x06000000))
#define SCREEN_W 240
#define SCREEN_H 160

#define REG_VCOUNT (*(v_uint16*)(0x04000006))
void vsync()
{
	while (REG_VCOUNT >= 160);
	while (REG_VCOUNT < 160);
}

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

int qran_range(int32 a_min, int32 a_max)
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

uint16 setColor(uint8 a_red, uint8 a_green, uint8 a_blue)
{
	return (a_red & 0x1F) | (a_green & 0x1F) << 5 | (a_blue & 0x1f) << 10;
}

void drawRect(int32 a_left, int32 a_top, int32 a_width, int32 a_height, uint16 a_color)
{
	for (uint32 y = 0; y < a_height; ++y)
	{
		for (uint32 x = 0; x < a_width; ++x)
		{
			SCREENBUFFER[(a_top + y) * SCREEN_W + a_left + x] = a_color;
		}
	}
}

void drawLine(int32 a_x, int32 a_y, int32 a_x2, int32 a_y2, uint16 a_color)
{
	int32 w = a_x2 - a_x;
	int32 h = a_y2 - a_y;
	int32 dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (w<0) dx1 = -1; else if (w>0) dx1 = 1;
	if (h<0) dy1 = -1; else if (h>0) dy1 = 1;
	if (w<0) dx2 = -1; else if (w>0) dx2 = 1;
	int32 longest = abs(w);
	int32 shortest = abs(h);
	if (!(longest>shortest)) {
		longest = abs(h);
		shortest = abs(w);
		if (h<0) dy2 = -1; else if (h>0) dy2 = 1;
		dx2 = 0;
	}
	int32 numerator = longest >> 1;
	for (int32 i = 0; i <= longest; i++) {
		SCREENBUFFER[a_y * SCREEN_W + a_x] = a_color;
		numerator += shortest;
		if (!(numerator<longest)) {
			numerator -= longest;
			a_x += dx1;
			a_y += dy1;
		}
		else {
			a_x += dx2;
			a_y += dy2;
		}
	}
}


typedef struct Ball
{
	int32 x, y, xDir, yDir, size;
	uint16 color;
}Ball;

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



typedef struct Paddle
{
	int32 x, y, width, height;
	uint16 color;
}Paddle;

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

int main()
{
	//set GBA rendering context to MODE 3 Bitmap Rendering
	REG_DISPLAYCONTROL = VIDEOMODE_3 | BGMODE_2;

	sqran(14);
	Paddle p1;
	InitPaddle(&p1,10, 60, 8, 40, setColor(0, 0, 31));
	Paddle p2;
	InitPaddle(&p2, SCREEN_W - 18, 60, 8, 40, setColor(31, 0, 0));

	Ball ball;
	InitBall(&ball, 115, 75, 10, setColor(31, 31, 31));

	while (1) { //loop forever
		vsync();
		ClearBall(&ball);
		ClearPaddle(&p1);
		ClearPaddle(&p2);

		MoveBall(&ball);
		
		drawLine(10, 4, 230, 4, setColor(31, 31, 31));
		drawLine(230, 156, 10, 156, setColor(31, 31, 31));
		DrawBall(&ball);
		DrawPaddle(&p1);
		DrawPaddle(&p2);
	}
	
	return 0;
}