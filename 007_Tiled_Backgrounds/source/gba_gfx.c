#include "gba_gfx.h"

u16* vid_page = vid_page_back;

u16* page_flip()
{
	vid_page = (u16*)((u32)vid_page ^ VRAM_PAGE_SIZE);
	REG_DISPCNT ^= VIDEO_PAGE;	// update control register	
	return vid_page;
}

//background control functions
u16* getBGTileBlock(u8 a_tileblock)
{
  //each block/page is 16Kb
  return  (u16*)(TILE_MEM[(a_tileblock & BG_TILEBLOCK_MASK)]);
}

u16* getBGMapBlock(u8 a_mapBlock /*numbers 0-31 accepted - masked otherwise*/ )
{
  return (u16*)(VRAM + ((a_mapBlock & BG_MAPBLOCK_MASK) * TILEMAP_BLOCK_SIZE));
}

u16 setBGControlRegister(u8 a_priority, u8 a_tileblock, u8 a_mosaic, u8 a_colourMode,
  u8 a_mapblock, u8 a_affineWrap, u8 a_bgSize )
{
  u16 control = BGCNT_PRIORITY(a_priority) | BGCNT_TILEBLOCK(a_tileblock) | 
  BGCNT_MOSAIC(a_mosaic) | BGCNT_COLOUR_MODE(a_colourMode) |
  BGCNT_TILEMAP_LOC(a_mapblock) | BGCNT_AFFINE_WRAP(a_affineWrap) |
  BGCNT_SIZE(a_bgSize);
  return control;
}

SpriteObject obj_buffer[128] = {0};

//functionality to set attribute 0 properties
u16 SetSpriteObjectAttrib0(u8 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosaic, u8 a_colourMode, u8 a_shape)
{
	u16 attrib0 = (a_y & A0_YPOS_MASK) | A0_MODE(a_objectMode) | A0_GFX_MODE(a_gfxMode)
	| A0_MOSAIC(a_mosaic) | A0_COLOUR_MODE(a_colourMode) | A0_SHAPE(a_shape);
	return attrib0;
}
//functionality to set attribute 1 properties
u16 SetSpriteObjectAttrib1(u16 a_x, u8 a_flip, u8 a_size)
{
	u16 attrib1 = (a_x & A1_XPOS_MASK) | A1_FLIP(a_flip) | A1_SIZE(a_size);
	return attrib1;
}

//functionality to set attribute 2 properties
u16 SetSpriteObjectAttrib2(u16 a_tileIndex, u8 a_priority, u8 a_paletteBank)
{
	u16 attrib2 = A2_TILE(a_tileIndex) | A2_PRIORITY(a_priority) | A2_PALETTE(a_paletteBank);
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
	a_obj->attr0 = (a_obj->attr0 & ~A0_MODE_MASK) | A0_MODE_DISABLE;
}

//! Unhide an object.
void UnhideSpriteObject(SpriteObject* a_obj, u8 mode)
{
	a_obj->attr0 = (a_obj->attr0 & ~A0_MODE_MASK) | A0_MODE(mode);
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
		dst->padding= 0;
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