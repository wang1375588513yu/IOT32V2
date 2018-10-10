#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_


#include "stm32f10x.h"
#include "board_config.h"

#define BLE_DEBUG

#define GPIO_OUT_HIGH	1
#define GPIO_OUT_LOW	0


#define BM_S02_EN(a)		if (a)	\
							GPIO_SetBits(GPIOA,GPIO_Pin_1);\
							else		\
							GPIO_ResetBits(GPIOA,GPIO_Pin_1)
						
#define BM_S02_Check_Connect_State	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

#define BM_S02_RestoreFactory(a)	if (a)	\
									GPIO_SetBits(GPIOC,GPIO_Pin_3);\
									else		\
									GPIO_ResetBits(GPIOC,GPIO_Pin_3)

void Bluetooth_RF_BM_S02_Port_GPIO_Init(void);

void Bluetooth_Sendmsg(uint8_t data[],uint8_t len);

void Bluetooth_RF_BM_S02_change_name(uint32_t id);

void Bluetooth_RF_BM_S02_getMAC(void);

void Bluetooth_RF_BM_S02_change_BPS(void);

void Bluetooth_RF_BM_S02_change_Connect_time(void);

void Bluetooth_RF_BM_S02_Connect_Mode(void);

void Bluetooth_RF_BM_S02_Sleep_Mode(void);

uint8_t Bluetooth_RF_BM_S02_Check_Connect_State(void);

void Bluetooth_RF_BM_S02_RestoreFactory(int rank);




#endif
