
#include "gba_bios.h"

void vblank_intr_wait()
{
  // 0x5 is VBlankIntrWait in the function table.
  asm volatile("swi 0x05");
}

ARM void interruptHandler()
{
	//get the interrupts that we're expecting to be called
	//u32 ie = REG_IE;
	//and the expected interrupts with the one being fired
 //   u32 ieif= ie & REG_IF;
	//This will tell us which interrupt callback function we should call
	
	REG_IF = INT_VBLANK;
	REG_IFBIOS |= INT_VBLANK;
}

void register_vblank_isr()
{
	REG_IME = 0x00;
	REG_INTERRUPT = (fnptr)interruptHandler;
	REG_DISPSTAT |= DSTAT_VBL_IRQ;
	REG_IE |= INT_VBLANK;
	REG_IME = 0x01;
}

