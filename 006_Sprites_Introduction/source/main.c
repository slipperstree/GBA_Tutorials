#include "gba_macros.h"
#include "gba_types.h"
#include "gba_bios.h"
#include "gba_gfx.h"
#include "gba_drawing.h"
#include "gba_mathUtil.h"
#include "gba_input.h"


#include <string.h>
//include the header file for the sprite food pixels
//Original Sprite Image obtained from itch.io free sprites
//https://henrysoftware.itch.io/pixel-food
#include "sp_food_8b.h"

int main()
{

	register_vblank_isr();
	//set GBA rendering context to MODE 0 Tile Rendering and enable 1D sprites
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPINGMODE_1D;
	//copy sprite palette into sprite palette memory
	memcpy( pal_sp_mem, sp_food_8bPal, sp_food_8bPalLen);
	//there should be enough sprite tile memory to load all food sprite tiles into memory
	memcpy(&tile8_mem[4][0], sp_food_8bTiles, sp_food_8bTilesLen);

	//initialise OAM/Sprite object memory
	oam_init(obj_buffer, 128);
	
	//Initialise the first sprite in our object buffer to be the first sprite in our tiles
	//Set spirte position to be the center of the screen
	s16 x = (SCREEN_W>>1) - 8;
	s16 y = (SCREEN_H>>1) - 8;
	//set the tile id
	u16 tileID = 0;

	//Set the sprite object up
	SpriteObject* sprite = &obj_buffer[0];
	sprite->attr0 = SetSpriteObjectAttrib0( y, A0_MODE_REG, 0, 1, A0_8BPP, A0_SQUARE);
	sprite->attr1 = SetSpriteObjectAttrib1( x, 0, A1_SIZE_1);
	sprite->attr2 = SetSpriteObjectAttrib2(tileID, 0, 0);

	while (1) { //loop forever
		vblank_intr_wait();

		PollKeys();
		if( keyReleased(A) )
		{
			tileID += 4<<1;
			tileID = tileID & 0x1FF;
		}
		sprite->attr2 = SetSpriteObjectAttrib2(tileID, 0, 0);
		oam_copy(MEM_OAM, obj_buffer, 1);
	}
	return 0;
}
