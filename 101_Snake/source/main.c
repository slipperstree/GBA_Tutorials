#include "gba_macros.h"
#include "gba_types.h"
#include "gba_bios.h"
#include "gba_gfx.h"
#include "gba_drawing.h"
#include "gba_mathUtil.h"
#include "gba_input.h"

//include Pong related structures and functions
#include "ball.h"
#include "paddle.h"

#include "control.h"

int main()
{
	//set GBA rendering context to MODE 3 Bitmap Rendering
	REG_DISPCNT = VIDEOMODE_3 | BGMODE_2;

	register_vblank_isr();

	gba_seed_rand(14);
	Paddle p1;
	InitPaddle(&p1,10, 60, 8, 40, setColor(0, 0, 31));
	Paddle p2;
	InitPaddle(&p2, SCREEN_W - 18, 60, 8, 40, setColor(31, 0, 0));

	// Ball ball;
	// InitBall(&ball, 115, 75, 10, setColor(31, 31, 31));

	CTL_init();

	while (1) { //loop forever
		//wait until v-sync occurs
		//vblank_intr_wait();
		//PollKeys();

		CTL_run();


		// ClearBall(&ball);
		// ClearPaddle(&p1);
		// ClearPaddle(&p2);

		// MoveBall(&ball);

		// s16 pDir = 0;
		// if( keyDown(UP) )
		// {
		// 	pDir = -2;
		// }
		// if( keyDown(DOWN) )
		// {
		// 	pDir = 2;
		// }
		// MovePaddle(&p1, pDir);

		// pDir = 0;
		// if( keyDown(A) )
		// {
		// 	pDir = -2;
		// }
		// if( keyDown(B) )
		// {
		// 	pDir = 2;
		// }
		// MovePaddle(&p2, pDir);

		// drawLine(10, 4, 230, 4, setColor(31, 31, 31));
		// drawLine(230, 156, 10, 156, setColor(31, 31, 31));
		// DrawBall(&ball);
		// DrawPaddle(&p1);
		// DrawPaddle(&p2);
	}
	
	return 0;
}