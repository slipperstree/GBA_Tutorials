#ifndef __GBA_GFX_H__
#define __GBA_GFX_H__

#include "gba_macros.h"
#include "gba_reg.h"
#include "gba_types.h"

//base address pointer for base register
#define REG_DISPCNT *((v_u32*)(REG_BASE))

//Defines for settng up different video modes
#define VIDEOMODE_0 0x0000  //Sprite Mode 0
#define VIDEOMODE_1 0x0001  //Sprite Mode 1
#define VIDEOMODE_2 0x0002  //Sprite Mode 2
#define VIDEOMODE_3 0x0003	// < Mode 3; 240x160 @16bpp
#define VIDEOMODE_4	0x0004	// < Mode 4; 240x160 @8 bpp
#define VIDEOMODE_5 0x0005  // < Mode 5; 160x128 @16bpp

//Defines for enabling different backgrounds
#define BGMODE_0	0x0100
#define BGMODE_1	0x0200
#define BGMODE_2	0x0400
#define BGMODE_3	0x0800

//vcount is used for testing for vertical blank
#define REG_VCOUNT (*(v_u16*)(REG_BASE + 0x06))

#define MEM_PALETTE			0x05000000 
#define pal_bg_mem		((u16*)MEM_PALETTE)
#define pal_sp_mem		((u16*)(MEM_PALETTE + 0x200))

#define VIDEO_PAGE			0x0010	//!< Page indicator

#define VRAM_PAGE_SIZE		0x0A000
#define VRAM_BACK	(VRAM + VRAM_PAGE_SIZE)

#define vid_mem			((u16*)VRAM)
#define vid_page_front	((u16*)VRAM)
#define vid_page_back	((u16*)VRAM_BACK)



extern u16* vid_page;
extern u16* page_flip();

//Defines and structures to map tiles in to memory blocks
typedef struct TILE { u32 data[8]; }__attribute__((packed, aligned(4)))TILE, TILE4;
typedef struct TILE8 { u32 data[16]; }__attribute__((packed, aligned(4)))TILE8;

typedef TILE TILEBLOCK[512];
typedef TILE8 TILEBLOCK8[256];

#define tile_mem ((TILEBLOCK*)0x06000000)
#define tile8_mem ((TILEBLOCK8*)0x06000000)

#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040

typedef struct SpriteObject {
	u16 attr0;
	u16 attr1;
	u16 attr2;
	s16 pad;
}__attribute__((packed, aligned(4)))SpriteObject;

extern SpriteObject obj_buffer[128];

#define MEM_OAM ((SpriteObject*) 0x07000000)

#define A0_YMASK 0xFF
#define A0_MODE_MASK 0xFCFF
//defines for Object mode - affine or regular, or double size affine
#define A0_MODE_REG		0x00
#define A0_MODE_AFF		0x01
#define A0_MODE_HIDE	0x02
#define A0_MODE_AFF_DBL 0x03
//colour mode defines
#define A0_4BPP 0x0
#define A0_8BPP 0x1
//sprite shape
#define A0_SQUARE 0x00
#define A0_WIDE   0x01
#define A0_TALL   0x02
//x position mask
#define A1_XMASK 0x1FF
#define A1_FLIPMASK 0x3
#define A1_SIZE_0 0x00
#define A1_SIZE_1 0x01
#define A1_SIZE_2 0x02
#define A1_SIZE_3 0x03
#define A1_AFF_INDEX_MASK 0x1F

extern u16 SetSpriteObjectAttrib0(u8 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosiac, u8 a_colorMode, u8 a_shape);
extern u16 SetSpriteObjectAttrib1(u16 a_x, u8 a_flip, u8 a_size);
extern u16 SetSpriteObjectAttrib2(u16 a_tileIndex, u8 a_priority, u8 a_paletteBank);
//set the position of a sprite object
extern void SetSpriteObjectPosition(SpriteObject* a_object, u8 a_x, u8 a_y);
//! Hide an object.
extern void HideSpriteObject(SpriteObject* a_obj);
//! Unhide an object.
extern void UnhideSpriteObject(SpriteObject* a_obj, u8 a_mode);

//Initialise Object Attribute Memory
extern void oam_init(SpriteObject* a_obj, u8 a_count);
//Copy Spirite Object Attribute Memory
extern void oam_copy(SpriteObject* a_dst, SpriteObject* a_src, u8 a_count);

#endif //__GBA_GFX_H__