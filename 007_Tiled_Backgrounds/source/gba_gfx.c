#include "gba_gfx.h"

u16* vid_page = vid_page_back;

u16* page_flip()
{
	vid_page = (u16*)((u32)vid_page ^ VRAM_PAGE_SIZE);
	REG_DISPCNT ^= VIDEO_PAGE;	// update control register	
	return vid_page;
}

SpriteObject obj_buffer[128] = {};

//functionality to set attribute 0 properties
u16 SetSpriteObjectAttrib0(u8 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosiac, u8 a_colorMode, u8 a_shape)
{
	u16 attrib0 = (a_y & A0_YMASK) | ((a_objectMode & 0x3) << 8) | ((a_gfxMode & 0x3) << 10) |
		((a_mosiac & 0x1) << 12) | ((a_colorMode & 0x1) << 13) | ((a_shape & 0x3) << 14);
	return attrib0;
}
//functionality to set attribute 1 properties
u16 SetSpriteObjectAttrib1(u16 a_x, u8 a_flip, u8 a_size)
{
	u16 attrib1 = (a_x & A1_XMASK) | ((a_flip & A1_FLIPMASK) << 12) | ((a_size & 3) << 14);
	return attrib1;
}

//functionality to set attribute 2 properties
u16 SetSpriteObjectAttrib2(u16 a_tileIndex, u8 a_priority, u8 a_paletteBank)
{
	u16 attrib2 = (a_tileIndex & 0x3FF) | ((a_priority & 3) << 10) | ((a_paletteBank & 0xF) << 12);
	return attrib2;
}
//functionality to set position properties for an object
void SetSpriteObjectPosition(SpriteObject* a_object, u8 a_x, u8 a_y)
{
	a_object->attr0 = (a_object->attr0 & 0xFF00) | (a_y & 0x00FF);
	a_object->attr1 = (a_object->attr1 & 0xFE00) | (a_x & 0x01FF);
}

void HideSpriteObject(SpriteObject* a_obj)
{
	a_obj->attr0 = (a_obj->attr0 & A0_MODE_MASK) | (A0_MODE_HIDE << 8);
}

//! Unhide an object.
void UnhideSpriteObject(SpriteObject* a_obj, u8 mode)
{
	a_obj->attr0 = (a_obj->attr0 & A0_MODE_MASK) | ((mode& 0x3) << 8);
}

void oam_init(SpriteObject* obj, u8 count)
{
	u32 nn = count;
	SpriteObject *dst = obj;

	// Hide each object
	while (nn--)
	{
		dst->attr0	= 0;
		dst->attr1	= 0;
		dst->attr2	= 0;
		dst->pad	= 0;
		HideSpriteObject(dst);
		++dst;
	}
	// init oam
	oam_copy(MEM_OAM, obj, count);
}

void oam_copy( SpriteObject* a_dst, SpriteObject* a_src, u8 a_count)
{

	u32 *dstw = (u32*)a_dst, *srcw = (u32*)a_src;
	while (a_count--)
	{
		*dstw++ = *srcw++;
		*dstw++ = *srcw++;
	}

}