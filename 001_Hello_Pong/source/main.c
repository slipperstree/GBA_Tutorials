//\===========================================================================================================
//\ Title: Main.c 
//\ Author: Jamie Stewart
//\ About: 
//\        This is a simple implementation of Pong for the GBA that does not feature any user input and draws 
//\   it's content to the screenbuffer using bitmap mode rendering. This implementation has the code for ball
//\   movement and basic collision to keep the ball inside the bouds of the screen. A simple pseudo random 
//\   number generator is introduced in this code too.
//\===========================================================================================================

#include "Intellisense.h"
#include <stdint.h>
#include <stdbool.h>

#pragma region Tutorial_000_Code
//\===========================================================================================================
//\TYPEDEF our variables to indicate clearly what level of bit precision each variable has
//\===========================================================================================================
typedef uint8_t		u8;		typedef int8_t		s8;
typedef uint16_t	u16;	typedef int16_t		s16;
typedef uint32_t	u32;	typedef int32_t		s32;
//\===========================================================================================================
//\ Volatile variables
//\ The volatile key word indicates to the compiler, and the programmer. That these variables may have their values
//\ altered by an external factor, this could be a hardware switch, or some external program that can access the 
//\ memory location of the variable.
//\===========================================================================================================
typedef volatile uint8_t		v_u8;	typedef volatile int8_t			v_s8;
typedef volatile uint16_t		v_u16;	typedef volatile int16_t		v_s16;
typedef volatile uint32_t		v_u32;	typedef volatile int32_t		v_s32;
//\===========================================================================================================
//\ Set up defines for making sense of some memory addresses
#define REG_DISPCNT *((v_u32*)(0x04000000))
#define VIDEOMODE_3 0x0003
#define BGMODE_2	0x0400

#define SCREENBUFFER ((u16*)(0x06000000))
#define SCREEN_W 240
#define SCREEN_H 160
//\===========================================================================================================
//\ Function to get the absolute value of a variable passed into it. This function uses bitwise operators
//\ The value sent in is bitshifted right 31 bits, this will leave only the most significant bit the sign bit.
//\ if the sign bit is on then this is a negative integer value, shifting it 31 spaces right leaves us with a 
//\ value of 0 or all 1's for a -tve number (this is really implementation independent, but works on the GBA arm 
//\ implementation). We then XOR this back into the original value which will remove the negative (this is two's 
//\ compliment at work) and give us a positive value if the mask was negative we add one (mask is either 0 or -1)
//\ and get our absolute number, no if statments involved. GBA code has to be quick.
s32 abs(s32 a_val)
{
	s32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}
//\===========================================================================================================
//\ Function takes three 8 bit values and uses these to set a 16 bit value, the & 0x1F ensures that the values are
//\ all less than 5 bits in size, any number higher than 31 will be truncated.
u16 setColor(u8 a_red, u8 a_green, u8 a_blue)
{
	return (a_red & 0x1F) | (a_green & 0x1F) << 5 | (a_blue & 0x1f) << 10;
}
//\===========================================================================================================
void plotPixel( u32 a_x, u32 a_y, u16 a_colour)
{
	SCREENBUFFER[a_y * SCREEN_W + a_x] = a_colour;
}

//\===========================================================================================================
//\function to fill a rectangular area of the screen with the colour value provided to it as the 5th argument.
void drawRect(u32 a_left, u32 a_top, u32 a_width, u32 a_height, u16 a_color)
{
	for (u32 y = 0; y < a_height; ++y)
	{
		for (u32 x = 0; x < a_width; ++x)
		{
			SCREENBUFFER[ (a_top + y) * SCREEN_W + a_left + x ] = a_color;
		}
	}
}
//\===========================================================================================================
//\ an implementation of bresenham's line drawing algorithm, allows lines to be drawn in any direction
//\ from positive to negative direction, vertically or horizontally
void drawLine(u32 a_x, u32 a_y, u32 a_x2, u32 a_y2, u16 a_colour)
{
	//Get the horizontal and vertical displacement of the line
	s32 w = a_x2 - a_x; //w is width or horizontal distance
	s32 h = a_y2 - a_y; //h is the height or vertical displacement
	//work out what the change in x and y is with the d in these variables stands for delta
	s32 dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

	if (w<0) dx1 = dx2 = -1; else if (w>0) dx1 = dx2 = 1;
	if (h<0) dy1 = -1; else if (h>0) dy1 = 1;
	//which is the longest the horizontal or vertical step
	s32 longest = abs(w); //assume that width is the longest displacement
	s32 shortest = abs(h);
	if ( shortest > longest )	//oops it's the other way around reverse it
	{
		//use xor to swap longest and shortest around
		longest ^= shortest; shortest ^= longest; longest ^= shortest;
		if (h<0) dy2 = -1; else if (h>0) dy2 = 1;
		dx2 = 0;
	}
	//geta  value that is half the longest displacement
	s32 numerator = longest >> 1;
	//for each pixel across the longest span
	for (s32 i = 0; i <= longest; ++i)
	{
		//fill the pixel we're currently at
		plotPixel( a_x, a_y, a_colour);
		//increase the numerator by the shortest span
		numerator += shortest;
		if (numerator>longest)  
		{
			//if the numerator is now larger than the longest span
			//subtract the longest value from the numerator
			numerator -= longest;
			//increment x & y by their delta1 values
			a_x += dx1;
			a_y += dy1;
		}
		else 
		{
			//numerator is smaller than the longst side
			//increment x & y by their delta 2 values
			a_x += dx2;
			a_y += dy2;
		}
	}
}

#pragma endregion

//\===========================================================================================================
//\ Set up a simple random number generator
//\ Set the seed for the random number generator - for now hardcode this value.
s32 __rand_seed = 42;
//\ Function to allow modification of the random seed 
s32 seed_gba_rand(s32 a_val)
{
	s32 old = __rand_seed;
	__rand_seed = a_val;
	return old;
}
//\===========================================================================================================
//\Function to generate a random number
//\ This is a basic Linear Congruential Generator (LCG) (Wikipedia: https://en.wikipedia.org/wiki/Linear_congruential_generator)
//\ This version uses the numerical values from the Numerical Recipies book
//\ The equation for LCG is (a * x + c) mod m, as the m value is chosed to be 2^32 then simply bitwise & with 0x7FFF has the 
//\ same effect as a modulus operation, but is a cheaper operation to perform. The bit shift is simply done as there is not enough
//\ variance in the lower 16 bits of the value to be of much interest and would limit the periodicity of the pseudo random value returned
s32 gba_rand()
{
	__rand_seed = 1664525 * __rand_seed + 1013904223;
	return (__rand_seed >> 16) & 0x7FFF;
}
//\===========================================================================================================
//\ Get a random number that is within a certain range a_min -> a_max
s32 gba_rand_range(s32 a_min, s32 a_max)
{
	return (gba_rand()*(a_max - a_min) >> 15) + a_min;
}

//\===========================================================================================================
//\ Gets the sign of the variable passed in by looking at the most significant bit which if on suggest the value is negative
s32 sign(s32 a_val)
{
	return (!(a_val & 0x80000000) && !a_val) ? 1 : -1;
}

//\===========================================================================================================
//\ Structure for a ball object for Pong
//\ Has position, direction, size and colour
typedef struct Ball
{
	s32 x, y, xDir, yDir, size;
	u16 color;
}Ball;

//\===========================================================================================================
//\ Start Ball - function to get the ball rolling, ball starts at the centre of screen and will choose a random 
//\ x direction of travel, the ball has to move in the x direction hence the while loop when xDir = 0
//\ y direction is not as important.
void StartBall(Ball* a_ball) {
	while (a_ball->xDir == 0)
	{
		a_ball->xDir = gba_rand_range(-1, 2);
	}
	a_ball->yDir = gba_rand_range(-1, 2);
}
//\ Function to initialise the variables in the ball structure, put into a function to keep things as tidy as possible
void InitBall(Ball* a_ball, s32 a_x, s32 a_y, s32 a_size, u16 a_color)
{
	a_ball->x = a_x;
	a_ball->y = a_y;
	a_ball->size = a_size;
	a_ball->color = a_color;
	a_ball->xDir = a_ball->yDir = 0;
	StartBall(a_ball);
}
//Function to control/update ball movement, this function will keep the ball inside of the y bounds of the screen
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
		a_ball->x = SCREEN_W/2 - a_ball->size/2; 
		a_ball->y = SCREEN_H/2 - a_ball->size/2;
		a_ball->xDir = 0;
		StartBall(a_ball);
	}

}
//We need to clear the screen buffer at the location of the ball
void ClearBall(Ball* a_ball)
{
	drawRect(a_ball->x, a_ball->y, a_ball->size, a_ball->size, setColor(0,0,0));
}
//We need to draw the ball at it's current location
void DrawBall(Ball* a_ball)
{
	drawRect(a_ball->x, a_ball->y, a_ball->size, a_ball->size, a_ball->color);
}

//\===========================================================================================================
//\ Structre to hold the data for a paddle object
//\ Paddles will have a position, a width and height, and a colour
typedef struct Paddle
{
	s32 x, y, width, height;
	u16 color;
}Paddle;
//\===========================================================================================================
//\ Initialise the paddle variables
void InitPaddle(Paddle* a_paddle, s32 a_x, s32 a_y, s32 a_width, s32 a_height, u16 a_color)
{
	a_paddle->x = a_x;
	a_paddle->y = a_y;
	a_paddle->width = a_width;
	a_paddle->height = a_height;
	a_paddle->color = a_color;
}
//\===========================================================================================================
//\ move the paddle - this function alters the paddles y position by a value amount passed in.
void MovePaddle(Paddle* a_paddle, s32 a_val)
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
//\Clear the paddle location on the screen buffer
void ClearPaddle(Paddle* a_paddle)
{
	drawRect(a_paddle->x, a_paddle->y, a_paddle->width, a_paddle->height, setColor(0, 0, 0));
}
//\Draw the paddle at it's current location
void DrawPaddle(Paddle* a_paddle)
{
	drawRect(a_paddle->x, a_paddle->y, a_paddle->width, a_paddle->height, a_paddle->color);
}

//In order to only draw everything once per screen refresh or once per frame without tearing
//we will make use of the VCOUNT register, this area of hardware lets us know where the Screen's
//current horizontal draw is up to and while this value is still within the dimensions of the screen
//we will not draw and will instead spin our wheels within the vsync function until this counter 
//is up to a value that greater than the screen height.
#define REG_VCOUNT (*(v_u16*)(0x04000006))
void vsync()
{
	while (REG_VCOUNT >= SCREEN_H);
	while (REG_VCOUNT < SCREEN_H);
}


int main()
{
	//set GBA rendering context to MODE 3 Bitmap Rendering
	REG_DISPCNT = VIDEOMODE_3 | BGMODE_2;

	seed_gba_rand(14);
	Paddle p1;
	InitPaddle(&p1,10, 60, 8, 40, setColor(0, 0, 31));
	Paddle p2;
	InitPaddle(&p2, SCREEN_W - 18, 60, 8, 40, setColor(31, 0, 0));

	Ball ball;
	InitBall(&ball,  SCREEN_W/2 - 5, SCREEN_H/2 - 5, 10, setColor(31, 31, 31));

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