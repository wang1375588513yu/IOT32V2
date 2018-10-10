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

unsigned char at_config_status = 0;//确定是处于AT指令配置模式还是传输模式
unsigned char at_config_return = 0;//标志AT指令配置时的返回状态

/*********************************************
**函数功能：向RTL8710发送AT指令
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
**函数功能：检测是否从RTL8710读取到数据
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
**函数功能：检测读取是否读取到RTL8710的数据
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
**函数功能：清空数组缓冲
********************************************************/
void WIFI_RTL8710_Clear_RecvData(void)
{
#ifdef USART2_ENABLE	
	Clear_USART_Buff(USART2_BUF);
#endif
}

/*************************************************************
**函数功能：检测网络连接状态
**返回	值：1------连接失败
**			0------连接成功
**************************************************************/
u8 WIFI_RTL8710_Check_NetWork_Status(void)
{
	return 0;
}


/*************************************************************
**函数功能：配置WIFI RTL8710为TCP客户端模式
**参	数：IP_Addr------指针指向服务器端口号和IP地址
**			格式：42.62.41.209,04002
**返回	值：成功
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
**函数功能：连接AP
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
**函数功能：断开连接AP
********************************************************************/
void WIFI_RTL8710_NotConnect_AP(void)
{	
	WIFI_RTL8710_Send_data("ATWD\r\n");
}

/*******************************************************************
**函数功能：设置启动时自动连接AP
********************************************************************/
void WIFI_RTL8710_Auto_Connect(void)
{	
	WIFI_RTL8710_Send_data("ATPG=1\r\n");
}


/*******************************************************************
**函数功能：SmartConfig配置WiFi
********************************************************************/
void WIFI_RTL8710_SmartConfig_Setup_Connect(void)
{	
	WIFI_RTL8710_Send_data("ATWQ\r\n");
}

/*******************************************************************
**函数功能：STA模式下,为模组配置静态IP
**ip_addr--------IP地址
**gateway_mask---格式 "192.168.1.1,255.255.255.0"
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
**函数功能：设置WiFi的工作模式 AP--STA--APSTA
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
**函数功能：关闭所有连接
*****************************************************************/
void WIFI_RTL8710_CloseAll_Connection(void)
{
	WIFI_RTL8710_Send_data("ATPD=0\r\n");
}

/****************************************************************
**函数功能：使能透传模式
*****************************************************************/
void WIFI_RTL8710_Enable_TransparentTransmission_mode(void)
{
	WIFI_RTL8710_Send_data("ATPU=1\r\n");
}

/****************************************************************
**函数功能：进入传输模式
*****************************************************************/
void WIFI_RTL8710_Enter_Transmission_mode(void)
{
	WIFI_RTL8710_Send_data("ATPT\r\n");
}

/****************************************************************
**函数功能：WIFI扫描周边路由器
*****************************************************************/
void WIFI_RTL8710_Scan_Router(void)
{
	WIFI_RTL8710_Send_data("ATWS\r\n");
}

/****************************************************************
**函数功能：WIFI模组重启
*****************************************************************/
void WIFI_RTL8710_Reboot(void)
{
	WIFI_RTL8710_Send_data("ATSR\r\n");
}

/****************************************************************
**函数功能：保存配置信息到flash
*****************************************************************/
void WIFI_RTL8710_Save_info_To_Flash(void)
{
	WIFI_RTL8710_Send_data("ATPL=1\r\n");
}



/**********************************************************
**函数功能：WIFI 引脚接口定义，初始化
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
**函数功能：WIFI模块使能
**********************************************************/
void WIFI_RTL8710_Enable(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

/*********************************************************
**函数功能：WIFI模块使能
**********************************************************/
void WIFI_RTL8710_Disable(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

/*********************************************************
**函数功能：WIFI模块复位
**********************************************************/
void WIFI_RTL8710_ReSet(void)
{
	WIFI_RTL8710_Disable();
	delay_ms(300);
	WIFI_RTL8710_Enable();
	delay_ms(300);
}


/****************************************************************
**函数功能：WIFI模块初始化
*****************************************************************/
void WIFI_RTL8710_Startup(void)
{
	WIFI_RTL8710_Port_GPIO_Init();
	WIFI_RTL8710_Enable();
}


/****************************************************************
**函数功能：WIFI模块初始化
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

