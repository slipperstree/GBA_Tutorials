//\===========================================================================================================
//\ Title: Main.c 
//\ Author: Jamie Stewart
//\ About: 
//\        This is a simple Pixel Plotter for the GBA that does not feature any user input and draws 
//\   it's content to the screenbuffer using bitmap mode rendering.
//\===========================================================================================================

#include "Intellisense.h"
#include <stdint.h>
#include <stdbool.h>

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

#define SCREENBUFFER ((v_u16*)(0x06000000))
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


int main()
{
	//set GBA rendering context to MODE 3 Bitmap Rendering and enable BG 2
	REG_DISPCNT = VIDEOMODE_3 | BGMODE_2;


	while (1) { //loop forever
		drawLine(10, 4, 230, 4, setColor(31, 31, 31));
		drawLine(230, 156, 10, 156, setColor(31, 31, 31));

		drawLine(10, 4, 230, 156, setColor(2, 31, 15));
		drawLine(10, 156, 230, 4, setColor(2, 15, 31));
		
		drawRect(100, 60, 40, 40, setColor(31,12,3));
	}
	
	return 0;
}