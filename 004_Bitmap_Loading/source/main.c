#include "gba_macros.h"
#include "gba_types.h"
#include "gba_gfx.h"
#include "gba_drawing.h"
#include "gba_mathUtil.h"
#include "gba_input.h"

//include the header file for the bitmap image
#include "img_gba_m3.h"
#include "img_gba_m4.h"
#include "img_gba_m5.h"

#include <string.h>

int main()
{

	u16 vidMode = VIDEOMODE_3;
	memcpy(SCREENBUFFER, img_gbaM3Bitmap, img_gbaM3BitmapLen);
	//set GBA rendering context to MODE 3 Bitmap Rendering
	REG_DISPCNT = VIDEOMODE_3 | BGMODE_2;
	while (1) { //loop forever
		vsync();

		PollKeys();

		if( keyReleased(A) )
		{
			vidMode += 1;
			if( vidMode > VIDEOMODE_5)
			{
				vidMode = VIDEOMODE_3;
			}

			switch (vidMode)
			{
				case VIDEOMODE_3:
				{
					memcpy(SCREENBUFFER, img_gbaM3Bitmap, img_gbaM3BitmapLen);
					break;
				}
				case VIDEOMODE_4:
				{
					memcpy(MEM_TILE, img_gba_m4_bitmap, img_gba_m4BitmapLen);
					memcpy(MEM_PALETTE, img_gba_m4_pal, img_gba_m4PalLen);
					break;
				}
				case VIDEOMODE_5:
				{
					memcpy(SCREENBUFFER, img_gba_m5Bitmap, img_gba_m5BitmapLen);
					break;
				}
				default:
					break;
			}
			REG_DISPCNT = vidMode | BGMODE_2;
		}
		
	}
	
	return 0;
}