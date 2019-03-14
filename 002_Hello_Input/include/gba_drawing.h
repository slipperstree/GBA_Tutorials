#ifndef __GBA_DRAWING_H__
#define __GBA_DRAWING_H__

#include "gba_reg.h"
#include "gba_types.h"

#define SCREENBUFFER ((v_uint16*)(VRAM))

#define SCREEN_W 240
#define SCREEN_H 160

extern uint16 setColor(uint8 a_red, uint8 a_green, uint8 a_blue);
extern void drawRect(uint32 a_left, uint32 a_top, uint32 a_width, uint32 a_height, uint16 a_color);
extern void drawLine(int32 a_x, int32 a_y, int32 a_x2, int32 a_y2, uint16 a_color);

#endif //__GBA_DRAWING_H__