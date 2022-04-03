#include "common.h"

//����������õ�����
u16 seedForRand=0;

//��ʱ����
u8 buff[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// #pragma optimize=none �Ǹ�IAR���ģ����ⱻ�������Ż���������ʱ��������
// ��ʹ��iar����ʱ������Ҫɾ�����߸ĳ�keil֧�ֵ��﷨
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

	// ����û�б����ù������Ի�ȡ����
	if (seedForRand == 0)
	{
		// ���������ADC���裬��ôʹ�����սŵ�ADC������Ϊ����
		#if PER_LIB_GPIO_CTRL == 1
			seedForRand = ADC_Get_Sample(ADC_Channel_P04_8G2K64S2_S4, ADC_Acc_10Bits);
		#else
			// �������ʱ��ģ�飬ʹ������ʱ����Ϊ����
			#if HAS_REAL_TIME_DEV == 1
				// TODO:
			#else
				// �����������������ʱ�����û����°���ʱ����ô�����ϵ���û�����֮�侭����ʱ����Ϊ����
				#if USE_KEY_PRESS_TIME == 1
					// TODO:
				#else
					// ɶ��û�У�ֻҪʹ�ù̶���������
				#endif
			#endif
		#endif

		// �������Ӳ����������
		srand(seedForRand);
	}

	return rand();
}
