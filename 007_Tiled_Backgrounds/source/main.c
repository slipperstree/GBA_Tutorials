#include "gba_macros.h"
#include "gba_types.h"
#include "gba_bios.h"
#include "gba_gfx.h"
#include "gba_drawing.h"
#include "gba_mathUtil.h"
#include "gba_input.h"


#include <string.h>

//The tileset used in this tutorial is avaiable from 
//https://bakudas.itch.io/generic-rpg-pack
//This tileset has been modified slightly to accommodate a 16 colour palette per layer    and not all tiles have been used.
#include "BG_Externs.h"

//MapED exports things in the wrong structure for GBA it's fine for 32x32 maps
//but anything larger and it breaks a wee bit

//lets adjust the map to better suit the hardware
//for ease create a structure to hold data for a row block (screen row of tiles)
//holds 16 tiles or half a row
//map blocks are a series of 16 bit entries per tile
//10 bits for index
//1 bit for h flip
//1 bit for v flip
//4 bits for palette index
typedef struct {u32 data[8]; } HALF_ROW;

void copy64x32MapIntoMemory( const u16* a_mapData, u16 a_mapBlockAddress )
{
	//get a pointer to the map Ed data we want to convert pages for
	HALF_ROW* src = (HALF_ROW*)a_mapData;
	//a 32x32 map occupies one address space (32x16 bits no$gba demo's this well)
	//a 64x32 map occupies two address spaces, 64x64 occupies four address spaces
	HALF_ROW* dst0 = (HALF_ROW*)getBGMapBlock(a_mapBlockAddress);
	HALF_ROW* dst1 = (HALF_ROW*)getBGMapBlock(a_mapBlockAddress+1);
	//as there are 32 tiles per page row the following loop can be used.
	//I am taking advantage of post increment here. The assignment operator happens
	//before the increment operator
	for( u32 i = 0; i < 32; ++i)
	{
		//copy row i of the left page
		*dst0++ = *src++;   *dst0++ = *src++;
		//copy row i of the right page
		*dst1++ = *src++;   *dst1++ = *src++;
	}
}

int main()
{

	register_vblank_isr();
	//set GBA rendering context to MODE 0 Tile Rendering and enable 1D sprites enable backgrounds 0 & 1
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D | BGMODE_0 | BGMODE_1;
	//copy sprite palette into sprite palette memory
	memcpy( PAL_BG_MEM, bgPalette, bgPaletteSize);
	//There is enough memory to copy all tiles into memory for this exercise
	u16* tileblock = getBGTileBlock(0);
	//copy the BG tiles into tile memory
	memcpy( tileblock, bgTiles, bgTilesSize);
	//copy maps
	copy64x32MapIntoMemory( bgMapLayer0, 16);
	copy64x32MapIntoMemory( bgMapLayer1, 18);

	//set up Background controllers for each layer
	REG_BGCNT[0] = setBGControlRegister( 1, 0, 0, 0, 16, 0, BG_REG_SIZE_64x32);
	REG_BGCNT[1] = setBGControlRegister( 0, 0, 0, 0, 18, 0, BG_REG_SIZE_64x32);

	s32 x = 0;
	s32 y = 0;
	while (1) { //loop forever
		vblank_intr_wait();
		PollKeys();

		x += getAxis(HORIZONTAL);
		y -= getAxis(VERTICAL);
		REG_BG_OFFSET[0].x = x;
		REG_BG_OFFSET[0].y = y;
		REG_BG_OFFSET[1].x = x;
		REG_BG_OFFSET[1].y = y;
	}
	return 0;
}
