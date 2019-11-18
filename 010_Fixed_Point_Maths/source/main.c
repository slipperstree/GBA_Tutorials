#include "gba_macros.h"
#include "gba_types.h"
#include "gba_bios.h"
#include "gba_math.h"
#include "gba_gfx.h"
#include "gba_timers.h"
#include "gba_drawing.h"
#include "gba_mathUtil.h"
#include "gba_input.h"


#include <string.h>

#include "rocket4bpp.h"

int main()
{

	register_vblank_isr();
	//set GBA rendering context to MODE 0 Tile Rendering and enable 1D sprites
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D;
	//copy sprite palette into sprite palette memory
	memcpy( PAL_SP_MEM, rocket4bppPal, rocket4bppPalLen);
	//copy sprite tiles into memory
	memcpy( &TILE8_MEM[4][0], rocket4bppTiles, rocket4bppTilesLen);
	oam_init(obj_buffer, 128);

	// Set position to center of screen
	int x = (SCREEN_W >> 1) - 16, y = (SCREEN_H >> 2) - 32;
	
	// Angle		
	int alpha = 0;
	// Offset for angle, 1 binary radian..
	int alpha_ofs = 1 << 8;

	// Scale, init to 1x.
	fixed scale = 1 << 8;
	// Ofset for scale 
	int scale_ofs = 4;


	
	SpriteObject* sprite = &obj_buffer[0];

	sprite->attr0 = SetSpriteObjectAttrib0( x, A0_MODE_AFF_DBL, A0_GFX_MODE_REG, 0, A0_COLOUR_MODE_4BPP, A0_SHAPE_TALL);
	sprite->attr1 = SetSpriteObjectAttrib1( y, 0, A1_SIZE_3);
	sprite->attr2 = SetSpriteObjectAttrib2( 0, 0, 0);
	SetAttrib1AffineIndex(sprite, 0);
	sprite->attr2 = 0;

	// Get our AFFINE for this sprite
	SpriteAffine* spriteAff = &obj_affine_buffer[0];
	obj_aff_identity(spriteAff);

	while (1) { //loop forever
		vblank_intr_wait();
		PollKeys();

		--y;
		SetSpriteObjectPosition(sprite, x, y);
		
		// Rotate by 1 binary radian
		alpha += alpha_ofs;

		// Reset to zero brads if we go over 256
		if (alpha > 256 << 8)
			alpha = 0;

		// Scale by +/- 4/256ths
		scale += scale_ofs;

		// Bounce scale if it reaches 3x (64) or 0.25x (512)
		if (scale > 2 << 9 || scale < 1 << 6) {
			scale_ofs = -scale_ofs;
		}

		// Rotate and scale sprite.
		obj_aff_rotscale(spriteAff, scale, scale, alpha);

		oam_copy(MEM_OAM, obj_buffer, 128);
	}
	return 0;
}
