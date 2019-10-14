#include "gba_gfx.h"

u16* vid_page = vid_page_back;

u16* page_flip()
{
	vid_page = (u16*)((u32)vid_page ^ VRAM_PAGE_SIZE);
	REG_DISPCNT ^= VIDEO_PAGE;	// update control register	
	return vid_page;
}

//We will keep a WRAM side copy of Sprite Attributes
//OAM access is locked during VDraw so this can be used to create a 
//double buffer style of Sprite Manipulation, that we copy the buffer over during VBLANK
SpriteObject obj_buffer[128] = { 0 };
//Function to copy sprites from obj_buffer to OAM memory (0x07000000)
void oam_copy( SpriteObject* a_dst, SpriteObject* a_src, u8 a_count )
{
	//cast source and destination to u32 pointers to improve copy speed
	//Sprites are 4 * 16 bit and we have 128 of them so copy 1K (potentially)
	u32* dstw = (u32*)a_dst;  u32* srcw = (u32*)a_src;
	while(a_count--)
	{
		*dstw++ = *srcw++; //here we take advantage of post increment after assignment
		*dstw++ = *srcw++; //Two 32 bit assignemnts is one sprite
	}
}

u16 SetSpriteObjectAttrib0( u8 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosaic, u8 a_colourMode
, u8 a_shape)
{
	u16 attrib0 = (a_y & A0_YPOS_MASK) | A0_MODE(a_objectMode) | A0_GFX_MODE(a_gfxMode)
	| A0_MOSAIC(a_mosaic) | A0_COLOUR_MODE(a_colourMode) | A0_SHAPE(a_shape);
	return attrib0;
}

u16 SetSpriteObjectAttrib1( u16 a_x, u8 a_flip, u8 a_size )
{
	u16 attrib1 = (a_x & A1_XPOS_MASK) | A1_FLIP(a_flip) | A1_SIZE(a_size);
	return attrib1;
}

u16 SetSpriteObjectAttrib2( u16 a_tileIndex, u8 a_priority, u8 a_paletteBank)
{
	u16 attrib2 = A2_TILE(a_tileIndex) | A2_PRIORITY(a_priority) | A2_PALETTE(a_paletteBank);
	return attrib2;
}
