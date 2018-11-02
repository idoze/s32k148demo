#include "delay.h"
#include "device_registers.h"

static uint8_t  fac_us=0;							//us��ʱ������
static uint16_t fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��

//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(uint8_t SYSCLK)
{
    fac_us=SYSCLK/2;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	fac_ms=(uint16_t)fac_us*1000;				//��OS��,����ÿ��ms��Ҫ��systickʱ����
}

//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ,(���ֵ��2^24/fac_us@fac_us=1)
void delay_us(uint32_t nus)
{		
	uint32_t temp;
	S32_SysTick->RVR=nus*fac_us; 				//ʱ�����
	S32_SysTick->CVR=0x00;        				//��ռ�����
	S32_SysTick->CSR=0x01 ;      				//��ʼ����
	do
	{
		temp=S32_SysTick->CSR;
	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
	S32_SysTick->CSR=0x00;      	 			//�رռ�����
	S32_SysTick->CVR =0X00;       				//��ռ�����
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��168M������,nms<=798ms 
void delay_xms(uint16_t nms)
{	 		  	  
	uint32_t temp;
	S32_SysTick->RVR=(uint32_t)nms*fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
	S32_SysTick->CVR =0x00;           			//��ռ�����
	S32_SysTick->CSR=0x01 ;          			//��ʼ����
	do
	{
		temp=S32_SysTick->CSR;
	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
	S32_SysTick->CSR=0x00;       				//�رռ�����
	S32_SysTick->CVR =0X00;     		  		//��ռ�����
} 
//��ʱnms 
//nms:0~65535
void delay_ms(uint16_t nms)
{	 	 
	uint8_t repeat=nms/540;						//������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
											//���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
	uint16_t remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
