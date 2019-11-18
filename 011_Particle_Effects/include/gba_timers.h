#ifndef __GBA_TIMERS_H__
#define __GBA_TIMERS_H__

#include "gba_reg.h"
#include "gba_types.h"
#include "gba_macros.h"

typedef struct Timer 
{
    union{
        u16 count;
        u16 start_val;
    }PACKED;
    u16 control;
}ALIGN(4) Timer;

//There are four timers which can be accessed as an array
#define TIMER       ((volatile Timer*)(REG_BASE + 0x100))
#define TIMER_MASK                  0x3
/*Timer Controller Breakdown
 Bit   Expl.
  0-1   Prescaler Selection (0=F/1, 1=F/64, 2=F/256, 3=F/1024)
  2     Count-up Timing   (0=Normal, 1=Overflow)
  3-5   Not used
  6     Timer IRQ Enable  (0=Disable, 1=IRQ on Timer overflow)
  7     Timer Start/Stop  (0=Stop, 1=Operate)
  8-15  Not used
  */
//Prescaler - timer frequency
#define TIMER_PRESCALER_MASK        0x3
#define TIMER_PRESCALE_1            0x0
#define TIMER_PRESCALE_64           0x1
#define TIMER_PRESCALE_256          0x2
#define TIMER_PRESCALE_1024         0x3
#define TIMER_PRESCALER_SHIFT       0
#define TIMER_PRESCALER(n)          ((n << TIMER_PRESCALER_SHIFT) & TIMER_PRESCALER_MASK)
//Counting Up - This is the overflow mechanic timers that set this bit only increment when the
//previous timer overflows, naming for this is cascade timer
#define TIMER_MODE_MASK             0x4
#define TIMER_NORMAL                0x0
#define TIMER_CASCADE               0x1
#define TIMER_MODE_SHIFT            2
#define TIMER_MODE(n)               ((n << TIMER_MODE_SHIFT) & TIMER_MODE_MASK)
//Timer IRQ - if enabled an interrupt is triggered in timer overflow
#define TIMER_IRQ_MASK              0x40
#define TIMER_IRQ_DISABLE           0x0
#define TIMER_IRQ_ENABLE            0x1
#define TIMER_IRQ_SHIFT             6
#define TIMER_IRQ(n)                ((n << TIMER_IRQ_SHIFT) & TIMER_IRQ_MASK)
//timer start/stop - enable/disable stopping a timer cause it to revert to it's reload value on restart
#define TIMER_ENABLE_MASK           0x80
#define TIMER_DISABLE               0x0
#define TIMER_ENABLE                0x1
#define TIMER_ENABLE_SHIFT          7
#define TIMER_OPERATE(n)            ((n<<TIMER_ENABLE_SHIFT) & TIMER_ENABLE_MASK)

extern void InitialiseTimer( u8 a_timer, u8 a_prescaler, u8 a_mode, u8 a_irq, u8 a_enable, u16 a_start_val );
extern void StartTimer(u8 a_timer);
extern void StopTimer(u8 a_timer);
extern void TimerToggleCascade( u8 a_timer );
extern u16 GetTimerValue(u8 a_timer);

#endif //__GBA_TIMERS_H__