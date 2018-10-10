#include "data_pack.h"
#include "usart_driver.h"
#include "dht11_driver.h"
#include "adc_driver.h"
#include "eeprom.h"
#include "ST_string.h"
#include "led_driver.h"
#include "rtc_driver.h"


uint8_t DevSeqByte = 0;

uint8_t Pack_IntegerData(u8 buff[],uint16_t lenth,uint8_t cmd,uint8_t seq,uint8_t type)
{
	u8 array[16] = {0};
	uint8_t read_buf[32];
	int time[6];
	DHT11_Data_TypeDef DHT11_Data;
	u8 data_l = 0,data_h = 0;
	u8 i = 0;
	
	//start flag
	array[0] = 0xff;
	array[1] = 0xff;

	//lenth
	data_l = lenth&0xff;
	data_h = (lenth>>8)&0xff;
	array[2] = data_h;
	array[3] = data_l;

	//cmd
	array[4] = type;

	//seq
	array[5] = seq;
	//Device type
	array[6] = cmd;
	
	if(cmd == RGBLED)
	{
		//SUCCESS BYTE
		array[7] = 0x00;
		//CRC
		data_h = array[2];
		for(i = 3;i < 8;i++)
		{
			data_h = data_h^array[i];
		}

		array[8] = data_h;

		for(i = 0;i < 9;i++)
		{
			buff[i] = array[i];
			printf("%c",buff[i]);
		}

		return 9;
	}
	else if(cmd == TEMP_HUM)
	{
		//SUCCESS BYTE
		array[7] = 0x00;
		if( Read_DHT11(&DHT11_Data)==SUCCESS)
		{
				data_l = (DHT11_Data.temp_int)&0xff;
				data_h = (DHT11_Data.humi_int)&0xff;
				array[8] = data_h;
				array[9] = 0x00;
				array[10] = data_l;
				array[11] = 0x00;
		}
		//CRC
		data_h = array[2];
		for(i = 3;i < 12;i++)
		{
			data_h = data_h^array[i];
		}

		array[12] = data_h;

		for(i = 0;i < 13;i++)
		{
			buff[i] = array[i];
			printf("%c",buff[i]);
		}

		return 13;
	}
	else if(cmd == LIGHT_INDENSITY)
	{
		//SUCCESS BYTE
		array[7] = 0x00;
		data_l = (Get_Adc(11))&0xff;
		data_h = ((Get_Adc(11))>>8)&0xff;
		array[8] = data_h;
		array[9] = data_l;
		//CRC
		data_h = array[2];
		for(i = 3;i < 10;i++)
		{
			data_h = data_h^array[i];
		}

		array[10] = data_h;

		for(i = 0;i < 11;i++)
		{
			buff[i] = array[i];
			printf("%c",buff[i]);
		}

		return 11;
	}
	else if(cmd == SET_TIME)
	{
		//SUCCESS BYTE
		array[7] = 0x00;
		//CRC
		data_h = array[2];
		for(i = 3;i < 8;i++)
		{
			data_h = data_h^array[i];
		}

		array[8] = data_h;

		for(i = 0;i < 9;i++)
		{
			buff[i] = array[i];
			printf("%c",buff[i]);
		}

		return 9;
	}
	else if(cmd == SET_ALARM)
	{
		//SUCCESS BYTE
		array[7] = 0x00;
		//CRC
		data_h = array[2];
		for(i = 3;i < 8;i++)
		{
			data_h = data_h^array[i];
		}

		array[8] = data_h;

		for(i = 0;i < 9;i++)
		{
			buff[i] = array[i];
			printf("%c",buff[i]);
		}

		return 9;
	}
	else if(cmd == VIEW_ALARM)
	{
		//SUCCESS BYTE
		array[7] = 0x00;
		eeprom_ReadBytes(read_buf,0,19);
		strto_int(read_buf,time);
		//year
		data_l = (time[0])&0xff;
		data_h = ((time[0])>>8)&0xff;
		array[8] = data_h;
		array[9] = data_l;
		//month		 
		array[10] = (time[1])&0xff;
		//day
		array[11] = (time[2])&0xff;
		//hour
		array[12] = (time[3])&0xff;
		//min
		array[13] = (time[4])&0xff;
		//seconds
		array[14] = (time[5])&0xff;
		//CRC
		
		data_h = array[2];
		for(i = 3;i < 15;i++)
		{
			data_h = data_h^array[i];
		}

		array[15] = data_h;

		for(i = 0;i < 16;i++)
		{
			buff[i] = array[i];
			printf("%c",buff[i]);
		}

		return 16;
	}
	
	return 0;
}

uint8_t Untie_DataPack(uint8_t buff[])
{
	uint16_t lenth = 0;
	uint16_t i = 0;
	uint8_t crc_data = 0;
	tm Setup_timer;
	tm Alarm_timer;
	
	if((buff[0] == 0xff)&&(buff[1] == 0xff))
	{		
		lenth = 0xffff&((buff[2]<<8)|buff[3]);
		
		crc_data = buff[2];
		for(i = 0;i < lenth;i++)
		{
			crc_data = crc_data^buff[i+3];
		}
		usart1_putc(crc_data);
		if(crc_data != buff[lenth+3])
		{
			//printf("crc error!\n");
			return 0;
		}
		else
		{
			
			if(buff[6] == RGBLED)
			{
				Red_LED(buff[7]);
				Green_LED(buff[8]);
				Blue_LED(buff[9]);
			}
			else if(buff[6] == TEMP_HUM)
			{
				//return buff[6];
			}
			else if(buff[6] == LIGHT_INDENSITY)
			{
				//return buff[6];
			}
			else if(buff[6] == SET_TIME)
			{
				
				Setup_timer.w_year = (buff[7] << 8 & 0xFF00) | (buff[8] & 0xff);
				Setup_timer.w_month = buff[9] ;
				Setup_timer.w_date = buff[10] ;
				Setup_timer.hour = buff[11] ;
				Setup_timer.min = buff[12] ;
				Setup_timer.sec = buff[13] ;
				RTC_Set(Setup_timer.w_year,Setup_timer.w_month,Setup_timer.w_date,
										Setup_timer.hour,Setup_timer.min,Setup_timer.sec);
				//return buff[6];
			}
			else if(buff[6] == SET_ALARM)
			{
				
				Alarm_timer.w_year = ((buff[7] << 8) & 0xFF00) | (buff[8] & 0xff);
				Alarm_timer.w_month = buff[9] ;
				Alarm_timer.w_date = buff[10] ;
				Alarm_timer.hour = buff[11] ;
				Alarm_timer.min = buff[12] ;
				Alarm_timer.sec = buff[13] ;
				Save_Alarm(Alarm_timer.w_year,Alarm_timer.w_month,Alarm_timer.w_date,
										Alarm_timer.hour,Alarm_timer.min,Alarm_timer.sec);
				
			}
			else if(buff[6] == VIEW_ALARM)
			{
				//return buff[6];
			}
		}
			
		
		DevSeqByte = buff[5];
		
		return buff[6];
	}
	printf("data error\r\n");

	return 0;
}



