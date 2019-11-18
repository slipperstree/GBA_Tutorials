#include "gba_timers.h"

void InitialiseTimer( u8 a_timer, u8 a_prescaler, u8 a_mode, u8 a_irq, u8 a_enable, u16 a_start_val )
{
    s8 id = a_timer & TIMER_MASK;
    TIMER[id].start_val = a_start_val;
    TIMER[id].control = TIMER_PRESCALER(a_prescaler) | TIMER_MODE(a_mode) | TIMER_IRQ(a_irq) | TIMER_OPERATE(a_enable);
}

void StartTimer(u8 a_timer)
{
    s8 id = a_timer & TIMER_MASK;
    TIMER[id].control |= TIMER_OPERATE(TIMER_ENABLE);
}

void StopTimer(u8 a_timer)
{
    s8 id = a_timer & TIMER_MASK;
    TIMER[id].control &= ~TIMER_OPERATE(TIMER_ENABLE);
}

void TimerToggleCascade( u8 a_timer )
{
    s8 id = a_timer & TIMER_MASK;
    TIMER[id].control ^= ~TIMER_MODE(TIMER_CASCADE);
}

u16 GetTimerValue(u8 a_timer)
{
    s8 id = a_timer & TIMER_MASK;
    return TIMER[id].count;    
}