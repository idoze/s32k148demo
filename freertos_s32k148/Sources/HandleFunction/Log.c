/**
  ******************************************************************************
  * File Name          : Log.h
  * Description        : Code for 3-dragon Log����
  * Author             : YJJ
  * Version            : 0.0.1
  * Date               : 2018.6.12
  ******************************************************************************
  */



#include "Log.h"


#include <stdio.h>
#include <stdarg.h>

void cli_puts(char* sz);


void cli_puts(char* sz)
{
/* ���������ĳ��������ַ����Ĵ��� */
    char c;
    uint32_t bytesRemaining;
    while ((c = *sz++) != 0) {
      
      	LPUART_DRV_SendData(INST_LPUART1, (uint8_t const*)&c, 1);
        /* Wait for transmission to be successful */
        while (LPUART_DRV_GetTransmitStatus(INST_LPUART1, &bytesRemaining)
        != STATUS_SUCCESS)
        {
        }    
    }
}

void cli_echo(const char* fmt, ...)
{
static char sz[1024] = { 0, };
/* ���ʹ�ÿɱ�����Լ�vsprintf��ʵ��printf */
va_list ap;

va_start(ap, fmt);
vsprintf(sz, fmt, ap); 
cli_puts(sz);
va_end(ap);
}