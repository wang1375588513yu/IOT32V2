#ifndef _DATA_PACK_H_
#define _DATA_PACK_H_

#include "stm32f10x.h" 


#if 0
typedef enum
{
	0x10,	//Accelerometer
	0x11,	//Gyroscope
	0x12,	//Magnetometer
	0x13,	//euler
	0x14,	//quaternion
	0x15,	//linear_accel
	0x16,	//gravity
	0x17,	//temperature
}DEV_TYPE;

#else

typedef enum
{
	RGBLED = 0X01,
	TEMP_HUM,
	LIGHT_INDENSITY,
	SET_TIME,
	SET_ALARM,
	VIEW_ALARM,
}DEV_TYPE;

#endif

typedef enum
{
	CONTROL_ACK = 0X7F,
	QUERY_ACK = 0X8F,
}ACK_TYPE;


extern uint8_t DevSeqByte;


uint8_t Pack_IntegerData(u8 buff[],uint16_t lenth,uint8_t cmd,uint8_t seq,uint8_t type);
uint8_t Pack_IntegerData_Temp(u8 buff[],uint16_t lenth,uint8_t seq,float temp);
uint8_t Untie_DataPack(uint8_t buff[]);

#endif



