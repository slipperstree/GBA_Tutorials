#include "gba_macros.h"
#include "gba_types.h"
#include "gba_bios.h"
#include "gba_gfx.h"
#include "gba_drawing.h"
#include "gba_mathUtil.h"
#include "gba_input.h"


#include <string.h>

//The tileset used in this tutorial is avaiable from https://bakudas.itch.io/generic-rpg-pack
//This tileset has been modified slightly to accommodate a 16 colour palette per layer and not all tiles have been used.
#include "BG_Externs.h"

//define a base registry address for the background controllers
//accessing this as REG_BGCNT[0] is controller 0
//accessing this as REG_BGCNT[1] is controller 1 or memory address REG_BGCNT + (sizeof(vu16) * 1) = 0x0400:000A
//accessing this as REG_BGCNT[2] is controller 2 or memory address REG_BGCNT + (sizeof(vu16) * 2) = 0x0400:000C
//accessing this as REG_BGCNT[3] is controller 3 or memory address REG_BGCNT + (sizeof(vu16) * 3) = 0x0400:000E
#define REG_BGCNT ((u16*)(REG_BASE+0x0008))

#define BGCNT_PRIORITY_MASK 0x3
#define BGCNT_TBID_MASK		0x3
#define BGCNT_MOS_MASK		0x1
#define BGCNT_COLOR_MASK	0x1
#define BGCNT_TMID_MASK		0x1F
#define BGCNT_AFW_MASK		0x1
#define BGCNT_SIZE_MASK		0x3

#define BG_REG_SIZE_32x32 0x0
#define BG_REG_SIZE_64x32 0x1
#define BG_REG_SIZE_32x64 0x2
#define BG_REG_SIZE_64x64 0x3

//structure to control background offset x & y flags
typedef struct BackgroundOffsets
{
	s16 x, y;
}__attribute__((packed, aligned(4))) BGOffset;
//set up define for REG_BG_OFS accessed as an array to access offset data for bg 0-3 
//REG_BG_OFS[1].x accesses x .y accesses y value
#define REG_BG_OFS  ((BGOffset*)(REG_BASE+0x0010))

#define TILE_BLOCK_SIZE 0x4000
#define TILEMAP_BLOCK_SIZE 0x0800
//function to return the memory address (pointer) to one of the four tile block locations (0-3)
u16* tile_block_address(u32 a_blockNum)
{
	//each block is 16KB in size
	return (u16*)(VRAM + (a_blockNum * TILE_BLOCK_SIZE));
}
//function to return a pointer to address space for the tile map blocks location (0-31)
u16* tileMap_block_address(u32 a_blockNum)
{
	//each tilemap block is 2KB in size
	return (u16*)(VRAM + (a_blockNum * TILEMAP_BLOCK_SIZE));
}

u16 setBG_Control_Register(u8 a_priority, u8 a_tileBlockID, u8 a_mosaic, u8 a_colourMode, u8 a_mapBlockID, u8 a_affineWrap, u8 a_bgSize)
{
	u16 control = (a_priority & BGCNT_PRIORITY_MASK) | ((a_tileBlockID & BGCNT_TBID_MASK) << 2) | ((a_mosaic & BGCNT_MOS_MASK) << 6) | ((a_colourMode & BGCNT_COLOR_MASK) << 7) |
		((a_mapBlockID & BGCNT_TMID_MASK) << 8) | ((a_affineWrap & BGCNT_AFW_MASK) << 13) | ((a_bgSize & BGCNT_SIZE_MASK) << 14);
	return control;
}

//MapEd is an asshole for the way it exports map blocks here's a fix for that.

//strucure to hold data for a row block, holds 16 tiles or half a row
//A map is a series of 16 bit entries per tile, 
// 10 bits for tile index
// 1 bit for horizontal flip
// 1 bit for vertical flip
// 4 bits for palette index (in 4bpp mode)
typedef struct { u32 data[8]; } HALF_ROW;

void copy64x32MapIntoMemory( const u16* a_mapData, u16 a_mapBlockAddress )
{
	//get a pointer to the mapEd data that we want to convert pages for
	HALF_ROW* src = (HALF_ROW*)a_mapData;
	//a 32x32 map occupies one address space (32x16bits, if you look at this in an emulator like no$gba you can see this is two rows of tile data space)
	//a 64x32 map occupies two address spaces, a 64x64 map occupies four address spaces, get two pointers to page location 0 and page location 1.
	HALF_ROW* dst0 = (HALF_ROW*)tileMap_block_address(a_mapBlockAddress);
	HALF_ROW* dst1 = (HALF_ROW*)tileMap_block_address(a_mapBlockAddress+1);

	//as there are 32 tiles per page row the following loop can be used. I am making use of post increment order here in C
	//this means that the assignment operator will take place first before the increment operator.
	//This could have been written in a longer form for the sake of readability but the compactness of this function is quite nice
	for( u32 i = 0; i < 32; ++i )
	{
		//copy row i of the left page
		*dst0++ = *src++;	*dst0++ = *src++;
		//copy row i of the right page
		*dst1++ = *src++;	*dst1++ = *src++;
	}
}

int main()
{

	register_vblank_isr();
	//set GBA rendering context to MODE 0 Tile Rendering and enable 1D sprites enable backgrounds 0 & 1
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPINGMODE_1D | BGMODE_0 | BGMODE_1;
	//copy sprite palette into sprite palette memory
	memcpy( pal_bg_mem, bgPalette, bgPaletteSize);
	//there should be enough sprite tile memory to load all food sprite tiles into memory
	//get our tileMemory location let's use block 0
	u16* tileMemLoc = tile_block_address(0);
	//copy the background data into the tile memory location
	memcpy(tileMemLoc, bgTiles, bgTilesSize);

	//get the tilemap memory location and then copy our map data into that memory
	//u16* tilemapMemLoc = tileMap_block_address(16);
	copy64x32MapIntoMemory( bgMapLayer0, 16);
	copy64x32MapIntoMemory( bgMapLayer1, 18);
	//memcpy(tilemapMemLoc, bgMapLayer0, bgMapLayer0Size);

	REG_BGCNT[0] = setBG_Control_Register( 1, 0, 0, 0, 16, 0, BG_REG_SIZE_64x32);
	REG_BGCNT[1] = setBG_Control_Register( 0, 0, 0, 0, 18, 0, BG_REG_SIZE_64x32);

	s32 x = 0;
	s32 y = 0;
	while (1) { //loop forever
		vblank_intr_wait();
		PollKeys();

		
		x += getAxis(HORIZONTAL);
		y -= getAxis(VERTICAL);
		REG_BG_OFS[0].x = x;
		REG_BG_OFS[0].y = y;
		REG_BG_OFS[1].x = x;
		REG_BG_OFS[1].y = y;
	}
	return 0;
}
