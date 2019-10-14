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
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D;
	//copy sprite palette into memory
	memcpy( PAL_SP_MEM, sp_food_8bPal, sp_food_8bPalLen);
	//There should be enough sprite memory to load all tiles into sprite tile mem
	memcpy( &TILE8_MEM[4][0], sp_food_8bTiles, sp_food_8bTilesLen);
	//Initialise the first sprite in our obj_buffer to be the first sprite in out tiles
	//Set sprite position to be center of the screen
	s16 x = (SCREEN_W>>1) - 8;
	s16 y = (SCREEN_H>>1) - 8;
	//set the tile ID
	u16 tileID = 0;	
	//Set the sprite object up
	SpriteObject* sprite = &obj_buffer[0];
	sprite->attr0 = SetSpriteObjectAttrib0( y, A0_MODE_REG, A0_GFX_MODE_REG, 0, 1, A0_SHAPE_SQUARE);
	sprite->attr1 = SetSpriteObjectAttrib1( x, 0, 1);
	sprite->attr2 = SetSpriteObjectAttrib2( tileID, A2_PRIORITY_0, 0);

	while (1) { //loop forever
		vblank_intr_wait();

		PollKeys();
		if( keyReleased(A) )
		{
			tileID += 4<<1;
			tileID = tileID & 0x1FF;
		}
		sprite->attr2 = SetSpriteObjectAttrib2( tileID, A2_PRIORITY_0, 0);
		oam_copy(MEM_OAM, obj_buffer, 1);
	}
	return 0;
}
