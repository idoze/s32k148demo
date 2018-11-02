/**
  ******************************************************************************
  * File Name          : ShowModule.c
  * Description        : Code for 3-dragon ��ʾģ�� 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#include "ShowModule.h"
#include "LinkList.h"
#include "PicDraw.h"
#include "SendimageData.h"
#include "UnicodeFont.h"
#include "delay.h"
#include "Flash_Handle.h"
#include "LCD_Basic_Elements.h"
/* ---------------------------------------------------------------------------*/
/*
*������ʾ��ģ�� ���Viewlist ��CanList ��ʾ���� 
*���ܵ㣺
*		2.���յ�CAN���ݶ���ID��(ͼ��ֻ��ҪID����ģ����������ţ�������ʾ ��Ҫ��������ֵ ������ʾ����Ҫ��������unicode�ռ�)
*		3.���Ͷ��������� ͼ������ ���ֶ��� 
*------------------------------------------------------------------------------*/
/**
  * @brief UpMachine_CANData_Analyses 
  * @param Viewlist:��ʾģ��Ƕ������
  * @param Canlist: Canģ��Ƕ������
  * @retval NULL
  */
 /* -----*/


static uint32_t Mult10(uint32_t n);

extern SPISendBuf SSendBuf;


uint32_t Mult10(uint32_t n)
{
  uint32_t sum=10;
  uint32_t m;
  if(n==0)
  {
    return 1;
  }
  for(m=1;m<n;m++)
  {
    sum*=10;
  }
  return sum;

}

void ViewShow(User_Header * UHead,CanModuleHeader * CHead,uint32_t IDItemValue)
{
   //����CANģ����ܵ��������ݲ��������շ�ʽ�����н���CANlistID�ţ�����ֱ��set������
   //ͼ����ʾ
   //��������
  
  uint16_t CAN_Rece_ID=0;//CAN������ģ���ID
  uint16_t CAN_Rece_Item=0;//CAN������ģ�������������ݴ�С
  
  uint16_t   Tdata=0;
  uint8_t   Tcount=0;
  
  
  CAN_Rece_ID=IDItemValue>>16;
  CAN_Rece_Item=IDItemValue;
  
  
 	
  SubCanModuleHeader *PSCMHead =  (SubCanModuleHeader *)GetPosValue(CHead->Sublist, CAN_Rece_ID);//�õ��˽ڵ���CAN���������
 	if (PSCMHead->DataMode==1)//ͼ����ʾ
 	{
  SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//�õ��˽ڵ���view���������

 		SubModuleShowPicDataContent *PSSPDCont=(SubModuleShowPicDataContent *)GetPosValue(PSSMHead->Contlist,CAN_Rece_Item);
 		SubModuleCANPicDataContent *PSCPDCont=(SubModuleCANPicDataContent *)GetPosValue(PSCMHead->Contlist,0);
      	#ifdef DEBUG
      	Log("show_flag is %d\r\n",PSSPDCont->show_flag);
      	#endif
 		Show_pic_Handle(PSSMHead,PSSPDCont);//��ʾ����

 	}
 	else if (PSCMHead->DataMode==2)//������ʾ
 	{
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//�õ��˽ڵ���view���������
   
    SubModuleCANNumDataContent *PSCNDCont=(SubModuleCANNumDataContent *)GetPosValue(PSCMHead->Contlist,0);
    
    Tdata=PSCNDCont->Data;//��������λ��
    if(Tdata==0)
    {
      Tcount=1;
    }
    while(Tdata!=0)
    {
      Tdata/=10;
      Tcount++;
    }
    //����λ����ѡ����ʾ������������
 		SubModuleShowNumFontDataContent *PSSNFDCont=(SubModuleShowNumFontDataContent *)GetPosValue(PSSMHead->Contlist,Tcount);
	    #ifdef DEBUG
	    Log("Num_Kind is %d\r\n",PSSNFDCont->Num_Kind);
	    Log("x is %d\r\n",PSSNFDCont->x);
	    Log("y is %d\r\n",PSSNFDCont->y);         
	    #endif
 		Show_Num_Handle(PSSMHead,PSSNFDCont,PSCNDCont,Tcount,UHead);
 	}
 	else if (PSCMHead->DataMode==3)//������ʾ
 	{
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//�õ��˽ڵ���view���������
     
 		SubModuleShowNumFontDataContent *PSSNFDCont=(SubModuleShowNumFontDataContent *)GetPosValue(PSSMHead->Contlist,CAN_Rece_Item);
 		SubModuleCANFontDataContent *PSCFDCont=(SubModuleCANFontDataContent *)GetPosValue(PSCMHead->Contlist,0);
	    #ifdef DEBUG
	    Log("Unicode_Kind is %d\r\n",PSSNFDCont->Unicode_Kind);
	    Log("x is %d\r\n",PSSNFDCont->x);
	    Log("y is %d\r\n",PSSNFDCont->y);      
	    #endif    
 		Show_Font_Handle(PSSMHead,PSCMHead,PSSNFDCont,PSCFDCont,UHead);
 	}
  else if (PSCMHead->DataMode==4)//������ʾ
  {
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//�õ��˽ڵ���view���������
   
    SubModuleShowAnimatedDataContent *PSSMHeadADCont=(SubModuleShowAnimatedDataContent *)GetPosValue(PSSMHead->Contlist,CAN_Rece_Item);
    SubModuleCANNumDataContent *PSCNDCont=(SubModuleCANNumDataContent *)GetPosValue(PSCMHead->Contlist,0);
    #ifdef DEBUG   
    Log("y is %d\r\n",PSSMHeadADCont->X);
    #endif  

    /*������ʾ */   
  }
  else if(PSCMHead->DataMode==5)//��������ʾ
  {
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//�õ��˽ڵ���view���������
 
    SubModuleShowBarDataContent *PSSMHeadBDCont=(SubModuleShowBarDataContent *)GetPosValue(PSSMHead->Contlist,0);
    SubModuleCANNumDataContent *PSCNDCont=(SubModuleCANNumDataContent *)GetPosValue(PSCMHead->Contlist,0);

    Show_Bar_Handle(PSSMHead,PSCMHead,PSSMHeadBDCont,PSCNDCont);
  }
  else if(PSCMHead->DataMode==99)//�л���Ļ
  {
      
    SubCanModuleHeader *PSCMHead =  (SubCanModuleHeader *)GetPosValue(CHead->Sublist, CAN_Rece_ID);//�õ��˽ڵ���CAN���������
    SubModuleCANPicDataContent *PSCPDCont=(SubModuleCANPicDataContent *)GetPosValue(PSCMHead->Contlist,0);
    AHead.ShowLayer=PSCPDCont->Data_ItemNumber;
    //�������� Ȼ���ʼ����һ�����������Ԫ��
    
  }
 	else
 	{
 		Log("ViewShow have no DataMode  error\r\n");
 	}

   	 
  
   //������ʾ
   //������ʾ
   

}

/* ---------------------------------------------------------------------------*/
/*
*ͼ�� ���� ������ʾ
*���ܵ㣺
*		1.������ģ�����ݽṹ ������
*		
*------------------------------------------------------------------------------*/
/**
  * @brief View_pic_Handle 
  * @param submodhead:Can��ģ��ָ��
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void Show_pic_Handle(SubShowModuleHeader *PSSMHead,\
					 SubModuleShowPicDataContent *PSSPDCont)//ͼ����ʾ������
{
//	//������ʾ����Ĳ���
    Clear_Region(PSSMHead);

	//��ȡ��Ҫ��ʾ����ģ��������ĵ�ַ ����
	SendimageHeader sendimageheader;

	sendimageheader.Start_Addr=PSSPDCont->Fpga_Addr;//��ʼ��ַ
	sendimageheader.pic_X=PSSPDCont->StartX;//��ʼx
	sendimageheader.pic_Y=PSSPDCont->StartY;//��ʼy
	sendimageheader.pic_W=PSSPDCont->StartW;//���
	sendimageheader.pic_H=PSSPDCont->StartH;//�߶�
	sendimageheader.Pic_Len=PSSPDCont->StartW*PSSPDCont->StartH;//���ظ���

	SViewModeOne(&sendimageheader);//������Ҫ��ʾ������
	
}
/**
  * @brief View_Num_Handle 
  * @param submodhead:Can��ģ��ָ��
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void Show_Num_Handle(SubShowModuleHeader *PSSMHead,\
					 SubModuleShowNumFontDataContent *PSSNFDCont,\
					 SubModuleCANNumDataContent *PSCNDCont,uint8_t Tcount,User_Header *Uhead)//������ʾ����
{
	uint32_t  Number={0};//��Ҫ��ʾ������
	uint16_t  Num_W=0;
	uint16_t  Num_H=0;
	uint32_t  NumAddr=0;
  uint8_t Num_kind=0;

	//������ʾ����Ĳ���
    //Clear_Region(PSSMHead);

	//����������ȶ���Ҫ��������ֵ�ֵ
	//���ø������ֻ����ֿ��е����� 
     Clear_Region(PSSMHead);
     OSIF_TimeDelay(8);
    Number=PSCNDCont->Data;
      if (PSSNFDCont->Num_Kind>Uhead->NumKindsize)//������ʲô��������
	{
		#ifdef DEBUG
		Log("numberkind>Max\r\n");
		#endif
  
	}
  else if(PSSNFDCont->Num_Kind==0)//˵�������ֲ���unicode���ֵı�������ʾ
  {
   //ȷ�����ֵĳ��� ����Ϣ ���ֿ��е�������ʾ�����unicodeֵ ����ֵ���

    //�����ָ���λ����Ҫ��ʾ�ĵ���ƴ����������
    //��ȡ���ֵ�ֵ ���ֵ�unicode��48-57
    //��ȷ��λ��
    //��������λ�� ȷ����ʾ����
    uint32_t  addr=48*128;
    
    SViewModeTwo(32*32*Tcount); //ȷ����Ҫ���Ͷ�������
    for(uint8_t i=0; i<Tcount; i++)
   {
        SViewModeTwo(32*32*Tcount);
      Flash_WordRead(Font_Buf,addr+(PSCNDCont->Data/(Mult10(i))%10)*128,128);//��ȡ�����ַ�����
      Font_Analyses(Font_Buf);//�����ַ�����  
     
      SendimageHeader sendimageheader;
      sendimageheader.Start_Addr=Uhead->FPGA_deputy_addr+(32*32*2)*i;//NumFlash_deputy_addr+Num_W*Num_H*20 Ϊƫ������ַ
      sendimageheader.Pic_Len=32*32;//��ȡ������Ҫ��ʾ���ֵ����ص㳤��
      sendimageheader.pic_X=PSSNFDCont->x+i*32;//��ʵ�������ʼ�Ľ����ļ���д���ض������� ��������������whindow�е���ʾ���꾭��ƫ��
      sendimageheader.pic_Y=PSSNFDCont->y;
      sendimageheader.pic_W=32;
      sendimageheader.pic_H=32;//�����겻�� ���ǳ����������ʾ��Χ
      SViewModeOne(&sendimageheader);//���ͱ�ͷ��ʼ����
    }
    
   }
   
	else//ֱ�ӷ��Ͱ������� ͨ��CAN item������ID���Ѿ�ȷ���������Ǽ�λ�� ֱ�Ӱ��� 
	{
   
		Num_kind=PSSNFDCont->Num_Kind;
		NumberSubKind *NumSK =	(NumberSubKind *)GetPosValue(Uhead->Num_list, Num_kind-1);//�õ��˽ڵ���CAN���������
		NumAddr=NumSK->NumFPGA_Addr;
		Num_W=NumSK->Num_W;
		Num_H=NumSK->Num_H;
    	//��ȡ��Ҫ��ʾ����ģ��������ĵ�ַ ����

    
        for(uint8_t i=0; i<Tcount; i++)
      {
        //��ȡ����ͼƬ��ַ
       
        SendimageHeader sendimageheader;
        sendimageheader.Start_Addr=(NumAddr+(Num_W*Num_H*4)*(PSCNDCont->Data/(Mult10(i))%10))/2;//NumFlash_deputy_addr+Num_W*Num_H*20 Ϊƫ������ַ
        sendimageheader.Pic_Len=Num_W*Num_H;//��ȡ������Ҫ��ʾ���ֵ����ص㳤��
        sendimageheader.pic_X=PSSNFDCont->x+i*Num_W;//��ʵ�������ʼ�Ľ����ļ���д���ض������� ��������������whindow�е���ʾ���꾭��ƫ��
        sendimageheader.pic_Y=PSSNFDCont->y;
        sendimageheader.pic_W=Num_W;
        sendimageheader.pic_H=Num_H;//�����겻�� ���ǳ����������ʾ��Χ
        SViewModeOne(&sendimageheader);//���ͱ�ͷ��ʼ����
        OSIF_TimeDelay(8);
      }
  	}
}


/**
  * @brief View_Font_Handle 
  * @param submodhead:Can��ģ��ָ��
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void Show_Font_Handle(SubShowModuleHeader *PSSMHead,\
					 SubCanModuleHeader *PSCMHead,\
					 SubModuleShowNumFontDataContent *PSSNFDCont,\
					 SubModuleCANFontDataContent *PSCFDCont,User_Header*Uhead)//������ʾ������
{
	
	uint8_t Font_W=0;
	uint8_t Font_H=0;
  uint16_t Nx=0;
  uint16_t Ny=0;
          SendimageHeader sendimageheader;
	//������ʾ����Ĳ���
    //Clear_Region(PSSMHead);

	//��can���յ������ж�ȡ��Ҫ��ʾUnicode��ֵ
	//ѡ���ֿ�
	//���ֿ��ж�ȡ��Ӧ��unicode�ֿ�ĵ���
	//��ʾ��ʽ������ȫ�����븱�ڴ� Ȼ������ʾ
	//  
	//PSSNFDCont.Unicode_Kind//���ֵ�����
    FontSubKind *FontSK =	(FontSubKind *)GetPosValue(Uhead->Font_list, PSSNFDCont->Unicode_Kind);//�õ��˽ڵ���CAN���������
    Font_W=FontSK->Font_W;	
    Font_H=FontSK->Font_H;		
    Nx=PSSNFDCont->x;
    Ny=PSSNFDCont->y;
    Clear_Region(PSSMHead);
    if(PSCFDCont->RData_len!=0)//��ʾģ���й̶��ַ���ʾ
    {
      for (uint8_t i = 1; i <PSCFDCont->RData_len+1; i++)
      {
        SViewModeTwo(Font_W*Font_H);
        Flash_WordRead(Font_Buf,FontSK->FontFlash_Addr+PSCFDCont->unicode_reg_pack[i-1]*(Font_W*Font_H)/8,(Font_W*Font_H)/8);//��ȡ�����ַ�����
        Font_Analyses(Font_Buf);//�����ַ�����

        sendimageheader.Start_Addr=Uhead->FPGA_deputy_addr;//+(Font_W*Font_H*2)*i;//NumFlash_deputy_addr+Num_W*Num_H*20 Ϊƫ������ַ
        sendimageheader.Pic_Len=Font_W*Font_H;//��ȡ������Ҫ��ʾ���ֵ����ص㳤��
        sendimageheader.pic_X=Nx;//��ʵ�������ʼ�Ľ����ļ���д���ض������� ��������������whindow�е���ʾ���꾭��ƫ��
        sendimageheader.pic_Y=Ny;
        sendimageheader.pic_W=Font_W;
        sendimageheader.pic_H=Font_H;//�����겻�� ���ǳ����������ʾ��Χ
        SViewModeOne(&sendimageheader);//���ͱ�ͷ��ʼ����
        OSIF_TimeDelay(8);
        
        if((Nx-PSSNFDCont->x)==PSSMHead->MaxW)//����
        {
          Nx=PSSNFDCont->x;//�ص���ʼx����
          Ny=Ny+Font_H;//y����
        }
        else
        {

           Nx=Nx+Font_W;
          
        }  
        
      }
    }
    
    if(PSCMHead->Effective_Data==255|PSCMHead->Effective_Data==PSCFDCont->NData_len)
    {
    //�����ַ������ͷ size��С=���ظ���*��Ч���ݳ���
    for (uint8_t i=1; i <PSCFDCont->NData_len+1; i++)
    {   SViewModeTwo(Font_W*Font_H);
        Flash_WordRead(Font_Buf,FontSK->FontFlash_Addr+PSCFDCont->unicode_pack[i-1]*(Font_W*Font_H)/8,(Font_W*Font_H)/8);//��ȡ�����ַ�����
        Font_Analyses(Font_Buf);//�����ַ�����

       if((Nx-PSSNFDCont->x)==PSSMHead->MaxW)//����
        {
          Nx=PSSNFDCont->x;//�ص���ʼx����
          Ny=Ny+Font_H;//y����
        }
   		//���䵽 ���ڴ��ַ
        sendimageheader.Start_Addr=Uhead->FPGA_deputy_addr;//+(Font_W*Font_H*2)*i;//NumFlash_deputy_addr+Num_W*Num_H*20 Ϊƫ������ַ
        sendimageheader.Pic_Len=Font_W*Font_H;//��ȡ������Ҫ��ʾ���ֵ����ص㳤��
        sendimageheader.pic_X=Nx;//��ʵ�������ʼ�Ľ����ļ���д���ض������� ��������������whindow�е���ʾ���꾭��ƫ��
        sendimageheader.pic_Y=Ny;
        sendimageheader.pic_W=Font_W;
        sendimageheader.pic_H=Font_H;//�����겻�� ���ǳ����������ʾ��Χ
        SViewModeOne(&sendimageheader);//���ͱ�ͷ��ʼ����
        OSIF_TimeDelay(8);
        if((Nx-PSSNFDCont->x)==PSSMHead->MaxW)//����
        {
          Nx=PSSNFDCont->x;//�ص���ʼx����
          Ny=Ny+Font_H;//y����
        }
        else
        {

           Nx=Nx+Font_W;
          
        } 
    }
    
    PSCMHead->Effective_Data=255;
    PSCFDCont->NData_len=0;
    }	
}

void Show_Animated_Handle(SubShowModuleHeader *PSSMHead,\
                       SubCanModuleHeader *PSCMHead,\
                       SubModuleShowAnimatedDataContent *PSSMHeadADCont,\
                       SubModuleCANNumDataContent *PSCNDCont)
{
  //������ʾ����
  //ȷ����Ҫ��֡id���͸�����3 ѭ����ʾ��η��� ����һ���ṹ�� �ṹ���������Ԫ�� ����Ԫ�ذ��� xy��wh ����ʱ�� ÿһ��ͼƬ�ĵȴ���ʾ����

}
void Show_Bar_Handle(SubShowModuleHeader *PSSMHead,\
                       SubCanModuleHeader *PSCMHead,\
                       SubModuleShowBarDataContent *PSSMHeadBDCont,\
                       SubModuleCANNumDataContent *PSCNDCont)
{
    float NowValue=1.0,Max_Value=0,CX=0,CY=0,K=0,SX=0,SY=0,FX=0,FY=0,B=0;

    float per=0;
    //uint8_t i = 0;

    NowValue=PSCNDCont->Data;
    Max_Value=PSSMHeadBDCont->Max_Value;
    per=(NowValue/Max_Value);//Ϊ��ȷ����ʾ�ĳ��ȴ�С�ռ�


    SX=PSSMHeadBDCont->X_S;
    SY=PSSMHeadBDCont->Y_S;
    FX=PSSMHeadBDCont->X_F;
    FY=PSSMHeadBDCont->Y_F;
    
    CX=FX-SX;
    CX=CX*per;//��ȡ����Ҫ��ʾ�ĳ���
    K=(FY-SY)/(FX-SX);
    B=FY-K*FX;
    CY=CX*K+B;

    LCD_DrawLine( PSSMHeadBDCont->X_S,PSSMHeadBDCont->Y_S, PSSMHeadBDCont->X_F,PSSMHeadBDCont->Y_F,\
                                PSSMHeadBDCont->unit_W,PSSMHeadBDCont->unit_H,0x00000000);//����
    //ȷ����ʾ����ĩ��ʾ
    LCD_DrawLine( (uint16_t)SX,  (uint16_t)SY, (uint16_t)CX,(uint16_t)CY,\
                                PSSMHeadBDCont->unit_W,PSSMHeadBDCont->unit_H,0x0000ff00);//����
    



}



//��ʼ����ʾ��ʾ��Ļ
void  show_init(uint8_t Layer)
{
    uint32_t Cvalue=0;

    for(uint16_t i=0;i<5;i++)
    {
      Cvalue=i<<16|0;
      show_Layer(Layer,Cvalue);//��ʼ����Ļ
      OSIF_TimeDelay(5);
    }



}
void show_Layer(uint8_t Layer,uint32_t ulReceivedValue)
{
  switch(Layer)
  {
    case 1:
      {
        ViewShow(AHead.UHead1,AHead.CHead1,ulReceivedValue);
      }break;
    case 2:
      {
        ViewShow(AHead.UHead2,AHead.CHead2,ulReceivedValue);
      }break;
    case 3:
      {
        ViewShow(AHead.UHead3,AHead.CHead3,ulReceivedValue);
      }break;
    default: break;
  }
  
}
