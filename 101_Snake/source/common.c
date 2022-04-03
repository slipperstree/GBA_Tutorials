#include "common.h"

//生成随机数用的种子
u16 seedForRand=0;

//临时变量
u8 buff[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// #pragma optimize=none 是给IAR看的，避免被编译器优化掉导致延时被跳过。
// 不使用iar环境时可能需要删掉或者改成keil支持的语法
//#pragma optimize=none
void My_delay_ms(u16 n)
{
	u16 x,y;
	for(x=n;x>0;x--)
	{
		for(y=110;y>0;y--);
	}
}

u8 My_strlen(char* x)
{
	u8 count = 0;
	while(*x != 0x00)
	{
		count++;
		x++;
	}
	return count;
}

u16 My_real_rand(){

	// 种子没有被设置过，尝试获取种子
	if (seedForRand == 0)
	{
		// 如果开启了ADC外设，那么使用悬空脚的ADC噪声作为种子
		#if PER_LIB_GPIO_CTRL == 1
			seedForRand = ADC_Get_Sample(ADC_Channel_P04_8G2K64S2_S4, ADC_Acc_10Bits);
		#else
			// 如果存在时钟模块，使用日期时间作为种子
			#if HAS_REAL_TIME_DEV == 1
				// TODO:
			#else
				// 最后，如果产生随机数的时机是用户按下按键时，那么根据上电后到用户按键之间经过的时间作为种子
				#if USE_KEY_PRESS_TIME == 1
					// TODO:
				#else
					// 啥都没有，只要使用固定的种子了
				#endif
			#endif
		#endif

		// 利用种子产生随机序列
		srand(seedForRand);
	}

	return rand();
}
