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
typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;
//\===========================================================================================================
//\ Volatile variables
//\ The volatile key word indicates to the compiler, and the programmer. That these variables may have their values
//\ altered by an external factor, this could be a hardware switch, or some external program that can access the 
//\ memory location of the variable.
//\===========================================================================================================
typedef volatile uint8_t		v_uint8;
typedef volatile uint16_t		v_uint16;
typedef volatile uint32_t		v_uint32;
typedef volatile int8_t			v_int8;
typedef volatile int16_t		v_int16;
typedef volatile int32_t		v_int32;
//\===========================================================================================================
//\ Set up defines for making sense of some memory addresses
#define REG_DISPLAYCONTROL *((v_uint32*)(0x04000000))
#define VIDEOMODE_3 0x0003
#define BGMODE_2	0x0400

#define SCREENBUFFER ((v_int16*)(0x06000000))
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
int32 abs(int32 a_val)
{
	int32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}
//\===========================================================================================================
//\ Function takes three 8 bit values and uses these to set a 16 bit value, the & 0x1F ensures that the values are
//\ all less than 5 bits in size, any number higher than 31 will be truncated.
uint16 setColor(uint8 a_red, uint8 a_green, uint8 a_blue)
{
	return (a_red & 0x1F) | (a_green & 0x1F) << 5 | (a_blue & 0x1f) << 10;
}
//\===========================================================================================================
//\function to fill a rectangular area of the screen with the colour value provided to it as the 5th argument.
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
//\===========================================================================================================
//\ an implementation of bresenham's line drawing algorithm, allows lines to be drawn in any direction
//\ from positive to negative direction, vertically or horizontally
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


int main()
{
	//set GBA rendering context to MODE 3 Bitmap Rendering and enable BG 2
	REG_DISPLAYCONTROL = VIDEOMODE_3 | BGMODE_2;


	while (1) { //loop forever
		drawLine(10, 4, 230, 4, setColor(31, 31, 31));
		drawLine(230, 156, 10, 156, setColor(31, 31, 31));

		drawLine(10, 4, 230, 156, setColor(2, 31, 15));
		drawLine(10, 156, 230, 4, setColor(2, 15, 31));

		drawRect(100, 60, 40, 40, setColor(31,12,3));
	}
	
	return 0;
}