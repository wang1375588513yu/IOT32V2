/*********************************OWL-IOT32*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "RTL8710_driver.h"
#include "usart_driver.h"
#include "ST_string.h"
#include "SysTick.h"

#include <stdlib.h>
#include "STM32_ChipID.h"

unsigned char at_config_status = 0;//ȷ���Ǵ���ATָ������ģʽ���Ǵ���ģʽ
unsigned char at_config_return = 0;//��־ATָ������ʱ�ķ���״̬

/*********************************************
**�������ܣ���RTL8710����ATָ��
**********************************************/
void WIFI_RTL8710_Send_data(u8* str)
{
#ifdef USART2_ENABLE
	if(st_strlen((unsigned char*)str) == Usart2_Sendstr(str))
	{
		delay_ms(100);
#ifdef USART1_DEBUG	
	printf("\r\nAT Send : %s\r\n",str);
#endif
		delay_ms(5);
	}
	else
	{
#ifdef USART1_DEBUG	
		printf("\r\nAT Send fild\r\n");
#endif
	}
#endif
}

/**************************************
**�������ܣ�����Ƿ��RTL8710��ȡ������
**************************************/
u8 WIFI_RTL8710_Read_data(void)
{
#ifdef USART2_ENABLE
	u8 i = 0;
	while(!Usart2_RecvStat.Recvcount)
	{
		i++;
		
		delay_ms(2);
		
		if(i>10)
		{
#ifdef USART1_DEBUG	
			printf("Check Recv Data: timeout\r\n");
#endif
			return 1;
		}
	}
#endif
	return 0;
}

/**************************************************
**�������ܣ�����ȡ�Ƿ��ȡ��RTL8710������
***************************************************/
u8 WIFI_RTL8710_Check_data(void)
{
#ifdef USART2_ENABLE
	u8 i = 0;
	while(!Usart2_RecvStat.Recvcount)
	{
		i++;
		
		delay_ms(100);
		
		if(i>10)
		{
#ifdef USART1_DEBUG	
			printf("Check Recv Data: timeout\r\n");
#endif
			return 1;
		}
	}
	
#ifdef USART1_DEBUG	
	printf("AT return: %s\r\n",Usart2_buff);
#endif
#endif

	return 0;
}

/*******************************************************
**�������ܣ�������黺��
********************************************************/
void WIFI_RTL8710_Clear_RecvData(void)
{
#ifdef USART2_ENABLE	
	Clear_USART_Buff(USART2_BUF);
#endif
}

/*************************************************************
**�������ܣ������������״̬
**����	ֵ��1------����ʧ��
**			0------���ӳɹ�
**************************************************************/
u8 WIFI_RTL8710_Check_NetWork_Status(void)
{
	return 0;
}


/*************************************************************
**�������ܣ�����WIFI RTL8710ΪTCP�ͻ���ģʽ
**��	����IP_Addr------ָ��ָ��������˿ںź�IP��ַ
**			��ʽ��42.62.41.209,04002
**����	ֵ���ɹ�
**************************************************************/
u8 WIFI_RTL8710_Set_TCPClient_Mode(u8* IP_Addr)
{
	u8 buf[36] = "ATPC=0,";
	
	st_strcat(buf,IP_Addr);
	st_strcat(buf,"\r\n");
	WIFI_RTL8710_Send_data(buf);
	
	delay_ms(100);

	return at_config_return;
}


/*******************************************************************
**�������ܣ�����AP
********************************************************************/
void WIFI_RTL8710_Connect_AP(u8* ssid,u8* passwd)
{
	u8 buf[50] = "ATPN=";
	
	st_strcat(buf,ssid);
	st_strcat(buf,",");
	st_strcat(buf,passwd);
	st_strcat(buf,"\r\n");
	
	WIFI_RTL8710_Send_data(buf);
}

/*******************************************************************
**�������ܣ��Ͽ�����AP
********************************************************************/
void WIFI_RTL8710_NotConnect_AP(void)
{	
	WIFI_RTL8710_Send_data("ATWD\r\n");
}

/*******************************************************************
**�������ܣ���������ʱ�Զ�����AP
********************************************************************/
void WIFI_RTL8710_Auto_Connect(void)
{	
	WIFI_RTL8710_Send_data("ATPG=1\r\n");
}


/*******************************************************************
**�������ܣ�SmartConfig����WiFi
********************************************************************/
void WIFI_RTL8710_SmartConfig_Setup_Connect(void)
{	
	WIFI_RTL8710_Send_data("ATWQ\r\n");
}

/*******************************************************************
**�������ܣ�STAģʽ��,Ϊģ�����þ�̬IP
**ip_addr--------IP��ַ
**gateway_mask---��ʽ "192.168.1.1,255.255.255.0"
********************************************************************/
void WIFI_RTL8710_Setup_StaticIP(u8* ip_addr,u8* gateway_mask)
{	
	u8 buf[50] = "ATPN=";
	
	st_strcat(buf,ip_addr);
	st_strcat(buf,",");
	st_strcat(buf,gateway_mask);
	st_strcat(buf,"\r\n");
	//ATPE=<ip>(,<gateway>,<mask>)
//	WIFI_RTL8710_Send_data("ATPE=192.168.1.254,192.168.1.1,255.255.255.0");
	WIFI_RTL8710_Send_data(buf);
}

/***********************************************
**�������ܣ�����WiFi�Ĺ���ģʽ AP--STA--APSTA
************************************************/
void Set_RTL8710_Work_mode(WIFIWork_Mode mode)
{
	switch(mode)
	{
		case WIFI_MODE_AP:
			WIFI_RTL8710_Send_data("ATPW=2\r\n");
			//ATPA=<ssid>,<pwd>,<chl>,<hidden>
			WIFI_RTL8710_Send_data("ATPA=OWL-IOT32,123456,11,0\r\n");
			break;
		case WIFI_MODE_STA:
			WIFI_RTL8710_Send_data("ATPW=1\r\n");
			break;
		case WIFI_MODE_APSTA:
			WIFI_RTL8710_Send_data("ATPW=3\r\n");
			WIFI_RTL8710_Send_data("ATPA=OWL-IOT32,123456,11,0\r\n");
			break;
	}
}

/****************************************************************
**�������ܣ��ر���������
*****************************************************************/
void WIFI_RTL8710_CloseAll_Connection(void)
{
	WIFI_RTL8710_Send_data("ATPD=0\r\n");
}

/****************************************************************
**�������ܣ�ʹ��͸��ģʽ
*****************************************************************/
void WIFI_RTL8710_Enable_TransparentTransmission_mode(void)
{
	WIFI_RTL8710_Send_data("ATPU=1\r\n");
}

/****************************************************************
**�������ܣ����봫��ģʽ
*****************************************************************/
void WIFI_RTL8710_Enter_Transmission_mode(void)
{
	WIFI_RTL8710_Send_data("ATPT\r\n");
}

/****************************************************************
**�������ܣ�WIFIɨ���ܱ�·����
*****************************************************************/
void WIFI_RTL8710_Scan_Router(void)
{
	WIFI_RTL8710_Send_data("ATWS\r\n");
}

/****************************************************************
**�������ܣ�WIFIģ������
*****************************************************************/
void WIFI_RTL8710_Reboot(void)
{
	WIFI_RTL8710_Send_data("ATSR\r\n");
}

/****************************************************************
**�������ܣ�����������Ϣ��flash
*****************************************************************/
void WIFI_RTL8710_Save_info_To_Flash(void)
{
	WIFI_RTL8710_Send_data("ATPL=1\r\n");
}



/**********************************************************
**�������ܣ�WIFI ���Žӿڶ��壬��ʼ��
**CHIP_EN--------------PB9
***********************************************************/
void WIFI_RTL8710_Port_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

/*********************************************************
**�������ܣ�WIFIģ��ʹ��
**********************************************************/
void WIFI_RTL8710_Enable(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

/*********************************************************
**�������ܣ�WIFIģ��ʹ��
**********************************************************/
void WIFI_RTL8710_Disable(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

/*********************************************************
**�������ܣ�WIFIģ�鸴λ
**********************************************************/
void WIFI_RTL8710_ReSet(void)
{
	WIFI_RTL8710_Disable();
	delay_ms(300);
	WIFI_RTL8710_Enable();
	delay_ms(300);
}


/****************************************************************
**�������ܣ�WIFIģ���ʼ��
*****************************************************************/
void WIFI_RTL8710_Startup(void)
{
	WIFI_RTL8710_Port_GPIO_Init();
	WIFI_RTL8710_Enable();
}


/****************************************************************
**�������ܣ�WIFIģ���ʼ��
*****************************************************************/
void WIFI_RTL8710_Init(void)
{
	printf("WIFI_RTL8710_Init()!!\r\n");
	//1.close all connection
	WIFI_RTL8710_CloseAll_Connection();
	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
	
	//3.connect to AP
//	Set_RTL8710_Work_mode(WIFI_MODE_STA);
	WIFI_RTL8710_Connect_AP("TP-LINK_dfq","wrw112358");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//4.creater TCP client,single connection
	WIFI_RTL8710_Set_TCPClient_Mode("192.168.1.108,60000");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//2.enable transparent transmission mode
	WIFI_RTL8710_Enable_TransparentTransmission_mode();
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//6.Enter transmission mode
//	WIFI_RTL8710_Enter_Transmission_mode();
//	delay_ms(1000);
//	at_config_status = WIFI_TRANSFER_MODE;
}

