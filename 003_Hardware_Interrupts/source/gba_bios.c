
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

void cpu_zero_memory(void* const dst, u32 len)
{
  // Use static data for our source since this function fills in 0s every time.
  static volatile uint32_t zero_value = 0;
  volatile uint32_t* src = &zero_value;
  // Set the fill bit flag to 1.
  len |= 1 << 24;
  __asm__ volatile(
    "ldr r0, %[src]\n" // Load address that points to 0 into first argument of CpuFastSet.
    "ldr r1, %[dst]\n" // Load the destination address into second argument of CpuFastSet.
    "mov r2, %[len]\n" // The length is the third argument to CpuFastSet.
    "swi 0xC" // 0xC is the index for CpuFastSet in the function table.
    : 
		: [src] "m" (src), [dst] "m" (dst), [len] "r" (len)
    : "r0", "r1", "r2", "cc", "memory"
  );
}

void cpu_fast_cpy(void* const dst, void* const src, u32 len)
{
  // Set the fill bit flag to 1.
  len &= 0xFFFFFF;
  __asm__ volatile(
    "ldr r0, %[src]\n" // Load address that points to 0 into first argument of CpuFastSet.
    "ldr r1, %[dst]\n" // Load the destination address into second argument of CpuFastSet.
    "mov r2, %[len]\n" // The length is the third argument to CpuFastSet.
    "swi 0xC" // 0xC is the index for CpuFastSet in the function table.
    : 
		: [src] "m" (src), [dst] "m" (dst), [len] "r" (len)
    : "r0", "r1", "r2", "cc", "memory"
  );
}