#include "gba.h"
#include <string.h>

#include "rocket4bpp.h"
#include "particles.h"

fixed g_gravity = -0x9CC; // gravity as -9.8 mps
fixed g_frameTime = 0x04;
fixed g_pixels2Meter = 0x500;

typedef struct Emitter{
	fixed x, y;
}Emitter;

typedef struct Particle{
	fixed x, y; //position
	fixed vx, vy; //velocity
	u32 life; //lifespan
}Particle;

void InitParticle( Particle* a_p )
{
	a_p->x = 0; a_p->y = 0; a_p->vx = 0; a_p->vy = 0; a_p->life = 0x1FF;
}

void EmitParticle( Particle* a_p, Emitter* a_e )
{
	a_p->x = a_e->x; a_p->y = a_e->y; // set position of particle to emitter
	a_p->vx = int2fix(gba_rand_range(-5, 5)); //x velocity between -5 & 5
	a_p->vy = int2fix(20 + gba_rand_range( 0, 5)); //y velocity 20 -> 25
	a_p->life = 0; //start this particle off at 0
}

void UpdateParticle( Particle* a_p, Emitter* a_e )
{
	a_p->x = fixAdd( a_p->x, fixMul(fixMul(a_p->vx, g_frameTime), g_pixels2Meter));
	a_p->y = fixAdd( a_p->y, fixMul(fixMul(a_p->vy, g_frameTime), g_pixels2Meter));
	a_p->vy -= fixMul( g_gravity, g_frameTime);
	a_p->life += 16;

	if( (a_p->y - a_e->y) > int2fix(40) )
	{
		EmitParticle( a_p, a_e);
	}
}

int main()
{

	register_vblank_isr();
	//set GBA rendering context to MODE 0 Tile Rendering and enable 1D sprites
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D;
	//copy sprite palette into sprite palette memory
	memcpy( PAL_SP_BLOCK(0), rocket4bppPal, rocket4bppPalLen);
	memcpy( PAL_SP_BLOCK(1), particlesPal, particlesPalLen);
	//copy sprite tiles into memory
	memcpy( sprite_tile_address(0), rocket4bppTiles, rocket4bppTilesLen);
	memcpy( sprite_tile_address(32), particlesTiles, particlesTilesLen);

	oam_init(obj_buffer, 128);

	// Set position to center of screen
	int x = (SCREEN_W >> 1) - 16, y = (SCREEN_H >> 1) - 32;
	//set up our rocket sprite
	SpriteObject* rocketSprite = &obj_buffer[0];
	rocketSprite->attr0 = SetSpriteObjectAttrib0( y, A0_MODE_REG, A0_GFX_MODE_REG, 0, A0_COLOUR_MODE_4BPP, A0_SHAPE_TALL);
	rocketSprite->attr1 = SetSpriteObjectAttrib1( x, 0, A1_SIZE_3);
	rocketSprite->attr2 = SetSpriteObjectAttrib2( 0, 0, 0);
	
	//configure our emiter
	Emitter emitter;
	emitter.x = int2fix( x + 12);
	emitter.y = int2fix( y + 32 );

	//set up particle
	Particle particles[64];
	SpriteObject particle;
	particle.attr0 = SetSpriteObjectAttrib0( 0, A0_MODE_REG, A0_GFX_MODE_REG, 0, A0_COLOUR_MODE_4BPP, A0_SHAPE_SQUARE);
	particle.attr1 = SetSpriteObjectAttrib1( 0, 0, A1_SIZE_0);
	particle.attr2 = SetSpriteObjectAttrib2(32, 0, 1);
	//copy particle into OAM buffer
	SpriteObject* particleOAM = &obj_buffer[1];
	for( s32 i = 0; i < 64; ++i)
	{
		EmitParticle( &particles[i], &emitter);
		particleOAM[i] = particle;
	}

	while (1) //loop forever
	{ 
		vblank_intr_wait();
		PollKeys();
		for( s32 i = 0; i < 64; ++i)
		{
			//update each particle
			UpdateParticle( &particles[i], &emitter);
			//update oam buffer with new particles positions
			SetSpriteObjectPosition( &particleOAM[i], fix2int(particles[i].x), fix2int(particles[i].y));
			u32 frameID = 0x200 - particles[i].life;
			frameID = frameID << 4 >> 9;
			particleOAM[i].attr2 = SetSpriteObjectAttrib2( 32 + frameID, 0, 1);
		}
				
		oam_copy(MEM_OAM, obj_buffer, 128);
	}
	return 0;
}
