// 板级支持头文件

#ifndef __EMB_SNAKE_BORAD_SUPPORT_H_
#define __EMB_SNAKE_BORAD_SUPPORT_H_

#include "common.h"

// 学电学习板
#define XUEDIAN

#ifdef XUEDIAN
    // LED
    #define LED_R_ON()              ;//TODO:GBA? EAXFR_ENABLE();setBitPWMniEnableOut(PWM_Port_0,PWM_Channel_5);EAXFR_DISABLE()
    #define LED_Y_ON()              ;//TODO:GBA?EAXFR_ENABLE();setBitPWMniEnableOut(PWM_Port_0,PWM_Channel_6);EAXFR_DISABLE()
    #define LED_G_ON()              ;//TODO:GBA?EAXFR_ENABLE();setBitPWMniEnableOut(PWM_Port_0,PWM_Channel_7);EAXFR_DISABLE()
    #define LED_R_OFF()             ;//TODO:GBA?EAXFR_ENABLE();clrBitPWMniEnableOut(PWM_Port_0,PWM_Channel_5);EAXFR_DISABLE()
    #define LED_Y_OFF()             ;//TODO:GBA?EAXFR_ENABLE();clrBitPWMniEnableOut(PWM_Port_0,PWM_Channel_6);EAXFR_DISABLE()
    #define LED_G_OFF()             ;//TODO:GBA?EAXFR_ENABLE();clrBitPWMniEnableOut(PWM_Port_0,PWM_Channel_7);EAXFR_DISABLE()
    #define LED_ALL_ON()            LED_R_ON();LED_Y_ON();LED_G_ON()
    #define LED_ALL_OFF()           LED_R_OFF();LED_Y_OFF();LED_G_OFF()

    // 蜂鸣器
    #define BEEP(tone,time)         ;//TODO:GBA? EAXFR_ENABLE();                  \
                                    setRegPWMnC(PWM_Port_5, tone);   \
                                    setRegPWMniT1(PWM_Port_5, PWM_Channel_3, tone/2);        \
                                    setBitPWMnCEN(PWM_Port_5);       \
                                    delay_ms(time);                  \
                                    clrBitPWMnCEN(PWM_Port_5);       \
                                    EAXFR_DISABLE();                  

    #define BEEP_SHORT()            ;//TODO:GBA? EAXFR_ENABLE();                  \
                                    setRegPWMnC(PWM_Port_5, 0x2000); \
                                    setRegPWMniT1(PWM_Port_5, PWM_Channel_3, 1000);          \
                                    setBitPWMnCEN(PWM_Port_5);       \
                                    delay_ms(50);                    \
                                    clrBitPWMnCEN(PWM_Port_5);       \
                                    EAXFR_DISABLE();
    

#endif

#endif
