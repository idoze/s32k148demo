#include "w25qxx.h" 
#include "flashread.h"
#include "delay.h"



uint16_t W25QXX_TYPE=0;

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
//W25Q32
//����Ϊ4M�ֽڣ�����64��Block,2048��Sector													 
//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{ 
  
	
	W25QXX_TYPE=W25QXX_ReadID();	//��ȡFLASH ID.
  #ifdef DEBUG
  Log("W25QXX_TYPE = %d",W25QXX_TYPE);
  #endif
	
  
}  

//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t W25QXX_ReadSR(void)   
{  
	uint8_t byte=0;   
	PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
	LPSI1_WriteRead_8bits(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=LPSI1_WriteRead_8bits(0Xff);             //��ȡһ���ֽ�  
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(uint8_t sr)   
{   
	PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
	LPSI1_WriteRead_8bits(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	LPSI1_WriteRead_8bits(sr);               //д��һ���ֽ�  
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_WriteEnable);      //����дʹ��  
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_WriteDisable);     //����д��ָֹ��    
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     	      
} 		
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);			    
	LPSI1_WriteRead_8bits(0x90);//���Ͷ�ȡID����	    
	LPSI1_WriteRead_8bits(0x00); 	    
	LPSI1_WriteRead_8bits(0x00); 	    
	LPSI1_WriteRead_8bits(0x00); 	 			   
	Temp|=LPSI1_WriteRead_8bits(0xFF)<<8;  
	Temp|=LPSI1_WriteRead_8bits(0xFF);	 
	
  PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);			
	return Temp;
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_ReadData);         //���Ͷ�ȡ����   
  LPSI1_WriteRead_8bits((uint8_t)((ReadAddr)>>16));  //����24bit��ַ    
  LPSI1_WriteRead_8bits((uint8_t)((ReadAddr)>>8));   
  LPSI1_WriteRead_8bits((uint8_t)ReadAddr);   
  for(i=0;i<NumByteToRead;i++)
	{ 
    pBuffer[i]=LPSI1_WriteRead_8bits(0XFF);   //ѭ������  
  }
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  W25QXX_Write_Enable();                  //SET WEL 
  PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_PageProgram);      //����дҳ����   
  LPSI1_WriteRead_8bits((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
  LPSI1_WriteRead_8bits((uint8_t)((WriteAddr)>>8));   
  LPSI1_WriteRead_8bits((uint8_t)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)LPSI1_WriteRead_8bits(pBuffer[i]);//ѭ��д��  
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
      
			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
static uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  
      
		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 
      
      pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
      NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{                                   
  W25QXX_Write_Enable();                  //SET WEL 
  W25QXX_Wait_Busy();   
  PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_ChipErase);        //����Ƭ��������  
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	//����falsh�������,������   
  //	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
  W25QXX_Write_Enable();                  //SET WEL 	 
  W25QXX_Wait_Busy();   
  PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_SectorErase);      //������������ָ�� 
  LPSI1_WriteRead_8bits((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
  LPSI1_WriteRead_8bits((uint8_t)((Dst_Addr)>>8));   
  LPSI1_WriteRead_8bits((uint8_t)Dst_Addr);  
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     	      
  W25QXX_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_PowerDown);        //���͵�������  
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     	      
  delay_xms(3);                               //�ȴ�TPD  
}   
//����
void W25QXX_WAKEUP(void)   
{  
  PINS_DRV_ClearPins(W25GPIO_PORT, 1 << W25_CS);                            //ʹ������   
  LPSI1_WriteRead_8bits(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	PINS_DRV_SetPins(W25GPIO_PORT, 1 << W25_CS);	                            //ȡ��Ƭѡ     	      
  delay_xms(3);                               //�ȴ�TRES1
}   


























