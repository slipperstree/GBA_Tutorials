// 使用TONC库的时候放开这一行定义
// 使用devkitPro自带gba库的话注释掉这一行定义
#define USE_TONC

#ifdef USE_TONC
	#include "tonc.h"
#else
	#include "gba.h"
#endif

#include "maxmod.h"

#include "soundbank.h"
#include "soundbank_bin.h"

int main() {

	#ifdef USE_TONC
		irq_init(NULL);
		irq_add( II_VBLANK, mmVBlank);
		irq_enable(II_VBLANK);
	#else
		irqInit();
		irqSet( IRQ_VBLANK, mmVBlank );
		irqEnable(IRQ_VBLANK);

		// Show some hint on the screen
		consoleDemoInit();
		// ansi escape sequence to clear screen and home cursor
		// /x1b[line;columnH
		iprintf("\x1b[2J");

		// ansi escape sequence to clear screen and home cursor
		// /x1b[line;columnH
		iprintf("\x1b[2J");
		// ansi escape sequence to set print co-ordinates
		// /x1b[line;columnH
		iprintf("\x1b[0;8HMaxMod Audio demo");
		iprintf("\x1b[3;0HHold A for ambulance sound");
		iprintf("\x1b[4;0HPress B for boom sound");
	#endif

	// initialise maxmod with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 8 );

	// Start playing module 这个播放有点问题会破音？
	mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );

	mm_sound_effect ambulance = {
		{ SFX_AMBULANCE } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	mm_sound_effect boom = {
		{ SFX_BOOM } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		255,	// panning
	};
	
	// sound effect handle (for cancelling it later)
	mm_sfxhand amb = 0;

	do {

		VBlankIntrWait();
		mmFrame();

		#ifdef USE_TONC
			key_poll();

			// Play looping ambulance sound effect out of left speaker if A button is pressed
			if ( key_hit(KEY_A) ) {
				amb = mmEffectEx(&ambulance);
			}

			// stop ambulance sound when A button is released
			if ( key_released(KEY_A) ) {
				mmEffectCancel(amb);
			}

			// Play explosion sound effect out of right speaker if B button is pressed
			if ( key_hit(KEY_B)) {
				mmEffectEx(&boom);
			}
		#else
			int keys_pressed, keys_released;

			scanKeys();
			keys_pressed = keysDown();
			keys_released = keysUp();

			if ( keys_pressed & KEY_A ) {
				amb = mmEffectEx(&ambulance);
			}

			if ( keys_released & KEY_A ) {
				mmEffectCancel(amb);
			}

			if ( keys_pressed & KEY_B ) {
				mmEffectEx(&boom);
			}
		#endif

	} while( 1 );
}
