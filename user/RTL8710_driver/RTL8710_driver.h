#ifndef _RTL8710_DRIVER_H_
#define _RTL8710_DRIVER_H_

#include "stm32f10x.h"
#include "board_config.h"


typedef enum{
	WIFI_MODE_AP,
	WIFI_MODE_STA,
	WIFI_MODE_APSTA
}WIFIWork_Mode;


typedef enum{
	WIFI_START_SUCCEED = 0,//wifi 启动成功标志
	WIFI_SETUP_MODE,//配置模式
	WIFI_TRANSFER_MODE,//透传模式
}WIFI_WORK_STATUS;


extern unsigned char at_config_status;
extern unsigned char at_config_return;


void WIFI_RTL8710_Port_GPIO_Init(void);
void WIFI_RTL8710_Enable(void);
void WIFI_RTL8710_Disable(void);
void WIFI_RTL8710_ReSet(void);

void WIFI_RTL8710_Startup(void);
void WIFI_RTL8710_Init(void);

#endif
