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
//The GBA has two areas of palette memory
//Background palette memory which consists of 256 colours 
#define PAL_BG_MEM			(u16*)(MEM_PALETTE)
//Sprite Palette memory which again consists to 256 colours
#define PAL_SP_MEM			(u16*)(MEM_PALETTE + 0x200) //0x200 = 512

#define VIDEO_PAGE			0x0010	//!< Page indicator

#define VRAM_PAGE_SIZE		0x0A000
#define VRAM_BACK	(VRAM + VRAM_PAGE_SIZE)

#define vid_mem			((u16*)VRAM)
#define vid_page_front	((u16*)VRAM)
#define vid_page_back	((u16*)VRAM_BACK)



extern u16* vid_page;
extern u16* page_flip();

//GBA Uses tiled rendering in Modes 0-2
//A tile consists of 8x8 pixels
//There are two variations on bitdepth 4bpp or 8bpp
//So we have two tile types 8x8@4bpp or 8x8@8bpp
typedef struct TILE { u32 data[8]; } ALIGN(4) TILE, TILE4;
typedef struct TILE8 { u32 data[16]; } ALIGN(4) TILE8;

//GBA Tile memory is split up into different pages
//There are six pages in total, four for background tiles and two for sprite tiles
//Each page can hold a total of 512 4bpp tiles or 256 8bpp tiles
//Defines for TILE blocks for standard tiles and double size tiles
typedef TILE TILEBLOCK[512];
typedef TILE8 TILEBLOCK8[256];
#define TILEBLOCK_SIZE      0x4000 //This is the size of a page of tiles in memory
#define TILEMAP_BLOCK_SIZE  0x0800 //Background tile maps are addressable in 2Kb Chunks
//The following define allows us to access title memory pages the way we would access an array
#define TILE_MEM ((TILEBLOCK*)VRAM)
#define TILE8_MEM ((TILEBLOCK8*)VRAM)

//Defines for Background Tile Rendering
//This could be put into an additional gba_ file
//I'll leave that up to you.
//define a base registry address for the background controller
//accessing this as an array will provide us with the 4 BG controllers
//EG REG_BGCNT[1] = BG controller 1
#define REG_BGCNT ((u16*)(REG_BASE+0x0008))
/*
 Bit   Expl.
  0-1   BG Priority           (0-3, 0=Highest)*/
#define BGCNT_PRIORITY_MASK       0x3
#define BGCNT_PRIORITY_SHIFT      0
#define BGCNT_PRIORITY(n)         (( n << BGCNT_PRIORITY_SHIFT) & BGCNT_PRIORITY_MASK)
/*  2-3   Character Base Block  (0-3, in units of 16 KBytes) (=BG Tile Data)*/
#define BGCNT_TILEBLOCK_MASK      0xC
#define BGCNT_TILEBLOCK_SHIFT     2
#define BGCNT_TILEBLOCK(n)        ((n << BGCNT_TILEBLOCK_SHIFT) & BGCNT_TILEBLOCK_MASK)
/*  4-5   Not used (must be zero) (except in NDS mode: MSBs of char base)*/

/*  6     Mosaic                (0=Disable, 1=Enable)*/
#define BGCNT_MOSAIC_MASK         0x40
#define BGCNT_MOSAIC_SHIFT        6
#define BGCNT_MOSAIC(n)           ((n << BGCNT_MOSAIC_SHIFT) & BGCNT_MOSAIC_MASK)
/*  7     Colors/Palettes       (0=16/16, 1=256/1)*/
#define BGCNT_COLOUR_MODE_MASK    0x80
#define BGCNT_COLOUR_MODE_SHIFT   7
#define BGCNT_COLOUR_MODE(n)      ((n << BGCNT_COLOUR_MODE_SHIFT) & BGCNT_COLOUR_MODE_MASK)
/*8-12  Screen Base Block     (0-31, in units of 2 KBytes) (=BG Map Data)*/
#define BGCNT_TILEMAP_LOC_MASK    0x1F00
#define BGCNT_TILEMAP_LOC_SHIFT   8
#define BGCNT_TILEMAP_LOC(n)      ((n << BGCNT_TILEMAP_LOC_SHIFT) & BGCNT_TILEMAP_LOC_MASK)
/*  13    BG0/BG1: Not used (except in NDS mode: Ext Palette Slot for BG0/BG1)
  13    BG2/BG3: Display Area Overflow (0=Transparent, 1=Wraparound)*/
#define BGCNT_AFFINE_WRAP_MASK    0x2000
#define BGCNT_AFFINE_WRAP_SHIFT   13
#define BGCNT_AFFINE_WRAP(n)      ((n << BGCNT_AFFINE_WRAP_SHIFT) & BGCNT_AFFINE_WRAP_MASK)
/*  14-15 Screen Size (0-3)
Internal Screen Size (dots) and size of BG Map (bytes):
  Value  Text Mode      Rotation/Scaling Mode
  0      256x256 (2K)   128x128   (256 bytes)
  1      512x256 (4K)   256x256   (1K)
  2      256x512 (4K)   512x512   (4K)
  3      512x512 (8K)   1024x1024 (16K)
  */
 #define BGCNT_SIZE_MASK        0xC000
 #define BGCNT_SIZE_SHIFT       14
 #define BGCNT_SIZE(n)          ((n << BGCNT_SIZE_SHIFT) & BGCNT_SIZE_MASK)
 //Regular Background sizes
 #define BG_REG_SIZE_32x32      0x0
 #define BG_REG_SIZE_64x32      0x1
 #define BG_REG_SIZE_32x64      0x2
 #define BG_REG_SIZE_64x64      0x3
 //Affine Background sizes
 #define BG_AFF_SIZE_16x16      0x0
 #define BG_AFF_SIZE_32x32      0x1
 #define BG_AFF_SIZE_64x64      0x2
 #define BG_AFF_SIZE_128x128    0x3

//Structure to control background offsets
//Background scrolls on x & y
typedef struct BackgroundOffset
{
  s16 x;
  s16 y;
}ALIGN(4) BGOffset;
//define for background offset regiter(s)
#define REG_BG_OFFSET ((BGOffset*)(REG_BASE+0x0010))

#define BG_TILEBLOCK_MASK     0x3 //There are only four tileblocks for BG tiles
//Get tile block/page by number (0-3)
extern u16* getBGTileBlock(u8 a_tileblock);

#define BG_MAPBLOCK_MASK 0x1F
//Function to get a pointer to the tile map block address (0-31)
extern u16* getBGMapBlock(u8 a_mapBlock /*numbers 0-31 accepted - masked otherwise*/ );
//Function to setup/configure BG_Control register
extern u16 setBGControlRegister(u8 a_priority, u8 a_tileblock, u8 a_mosaic, u8 a_colourMode,
  u8 a_mapblock, u8 a_affineWrap, u8 a_bgSize );
//To enable sprites we need to set a bit in our REG_DISPC
#define ENABLE_OBJECTS 0x1000
//There are two modes of mapping for Sprites 1D & 2D
//I prefer to use 1D as this makes more logical sense as a programmer
#define MAPPING_MODE_1D 0x0040

//Define Sprite Structure
//GBATek shows sprites are a collection of 4 16bit attributes
//attribute 0-2 are utilised attribute 3 is padding for affine sprites
typedef struct SpriteObject {
	u16 attr0;
	u16 attr1;
	u16 attr2;
	s16 padding;
} ALIGN(4) SpriteObject;
//there are 128 sprites available in OAM we may want to create a buffer for these
extern SpriteObject obj_buffer[128];
//Sprite memory in GBA terms is called OBject Attribute Memory (OAM)
#define MEM_OAM ((SpriteObject*)0x07000000)
//Define Sprite Attributes
//Copy Attribute defintions from GBATEX
/*OBJ Attribute 0 (R/W)
  Bit   Expl.
  0-7   Y-Coordinate           (0-255)*/
#define A0_YPOS_MASK          0xFF

 /* 8     Rotation/Scaling Flag  (0=Off, 1=On)
  When Rotation/Scaling used (Attribute 0, bit 8 set):
    9     Double-Size Flag     (0=Normal, 1=Double)
  When Rotation/Scaling not used (Attribute 0, bit 8 cleared):
    9     OBJ Disable          (0=Normal, 1=Not displayed)
*/
#define A0_MODE_MASK          0x0300
#define A0_MODE_REG           0x0
#define A0_MODE_AFFINE        0x100
#define A0_MODE_DISABLE       0x200
#define A0_MODE_AFF_DBL       0x300
#define A0_MODE_SHIFT         8
#define A0_MODE(n)            ((n << A0_MODE_SHIFT) & A0_MODE_MASK)
/*  10-11 OBJ Mode  (0=Normal, 1=Semi-Transparent, 2=OBJ Window, 3=Prohibited)*/
#define A0_GFX_MODE_MASK      0x0C00
#define A0_GFX_MODE_REG       0x0
#define A0_GFX_MODE_BLEND     0x400
#define A0_GFX_MODE_WINDOW    0x800
#define A0_GFX_MODE_SHIFT     10
#define A0_GFX_MODE(n)        ((n << A0_GFX_MODE_SHIFT) & A0_GFX_MODE_MASK)
/*  12    OBJ Mosaic             (0=Off, 1=On)*/
#define A0_MOSAIC_MASK        0x1000
#define A0_MOSAIC_SHIFT       12
#define A0_MOSAIC(n)          ((n << A0_MOSAIC_SHIFT) & A0_MOSAIC_MASK)
/*  13    Colors/Palettes        (0=16/16, 1=256/1)*/
#define A0_COLOUR_MODE_MASK   0x2000
#define A0_COLOUR_MODE_4BPP   0x0
#define A0_COLOUR_MODE_8BPP   0x2000
#define A0_COLOUR_MODE_SHIFT  13
#define A0_COLOUR_MODE(n)     ((n << A0_COLOUR_MODE_SHIFT) & A0_COLOUR_MODE_MASK)
/*  14-15 OBJ Shape              (0=Square,1=Horizontal,2=Vertical,3=Prohibited)*/
#define A0_SHAPE_MASK         0xC000
#define A0_SHAPE_SQUARE       0x0
#define A0_SHAPE_WIDE         0x4000
#define A0_SHAPE_TALL         0x8000
#define A0_SHAPE_SHIFT        14
#define A0_SHAPE(n)           ((n << A0_SHAPE_SHIFT) & A0_SHAPE_MASK)  

/*
OBJ Attribute 1 (R/W)
  Bit   Expl.
  0-8   X-Coordinate           (0-511)*/
#define A1_XPOS_MASK          0x1FF
  /*When Rotation/Scaling used (Attribute 0, bit 8 set):
    9-13  Rotation/Scaling Parameter Selection (0-31)
          (Selects one of the 32 Rotation/Scaling Parameters that
          can be defined in OAM, for details read next chapter.)
  When Rotation/Scaling not used (Attribute 0, bit 8 cleared):
    9-11  Not used
    12    Horizontal Flip      (0=Normal, 1=Mirrored)
    13    Vertical Flip        (0=Normal, 1=Mirrored)*/
#define A1_AFFINE_INDEX_MASK    0x3E00
#define A1_FLIP_MASK            0x3000
#define A1_H_FLIP               0x1000
#define A1_V_FLIP               0x2000
#define A1_FLIP_SHIFT           9
#define A1_FLIP(n)              ((n << A1_FLIP_SHIFT) & A1_FLIP_MASK)
/*  14-15 OBJ Size               (0..3, depends on OBJ Shape, see Attr 0)
          Size  Square   Horizontal  Vertical
          0     8x8      16x8        8x16
          1     16x16    32x8        8x32
          2     32x32    32x16       16x32
          3     64x64    64x32       32x64
*/
#define A1_SIZE_MASK            0xC000
#define A1_SIZE_0               0x0
#define A1_SIZE_1               0x4000
#define A1_SIZE_2               0x8000
#define A1_SIZE_3               0xC000
#define A1_SIZE_SHIFT           14
#define A1_SIZE(n)              ((n << A1_SIZE_SHIFT) & A1_SIZE_MASK)
/*
OBJ Attribute 2 (R/W)
  Bit   Expl.
  0-9   Character Name          (0-1023=Tile Number)*/
#define A2_TILE_MASK            0x3FF
#define A2_TILE_SHIFT           0
#define A2_TILE(n)              ((n << A2_TILE_SHIFT) & A2_TILE_MASK)
/*  10-11 Priority relative to BG (0-3; 0=Highest)*/
#define A2_PRIORITY_MASK        0xC00
#define A2_PRIORITY_0           0x0
#define A2_PRIORITY_1           0x400
#define A2_PRIORITY_2           0x800
#define A2_PRIORITY_3           0xC00
#define A2_PRIORITY_SHIFT       10
#define A2_PRIORITY(n)          ((n << A2_PRIORITY_SHIFT) & A2_PRIORITY_MASK)
/*  12-15 Palette Number   (0-15) (Not used in 256 color/1 palette mode)*/
#define A2_PALETTE_MASK         0xF000
#define A2_PALETTE_SHIFT        12
#define A2_PALETTE(n)           ((n << A2_PALETTE_SHIFT) & A2_PALETTE_MASK)

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