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
#include "Bluetooth.h"
#include "SysTick.h"
#include "usart_driver.h"
#include "ST_string.h"

/**********************************************************
**�������ܣ�WIFI ���Žӿڶ��壬��ʼ��
**B_IO0--------------PC3
**B_IO6--------------PB15
**B_RES--------------RESET
**B_EN---------------PA1
**B_Tx---------------USART3_Rx
**B_Rx---------------USART3_Tx
**BCTS---------------PB14--USART3_RTS
**BRTS---------------PB15--USART3_CTS
***********************************************************/
void Bluetooth_RF_BM_S02_Port_GPIO_Init(void)
{
	//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

	//B_IO0-------PC3------�ָ���������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	//B_IO6-------PB15------����״ָ̬ʾ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//B_EN--------PA1------ʹ�ܷ��͹㲥
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_3);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void Bluetooth_Sendmsg(uint8_t data[],uint8_t len)
{
	int i = 0;
	for(i=0;i<len;i++)
	{
#ifdef USART3_ENABLE
		usart3_putc(data[i]);
#endif
	}
}

/**********************************************************
**�������ܣBBluetooth_RF_BM_S02�޸���������
***********************************************************/
void Bluetooth_RF_BM_S02_change_name(uint32_t id)
{
	unsigned char BLE_NAME[23];
	
	sprintf((char*)BLE_NAME,"TTM:REN-OWL-%d",id);
	Clear_USART_Buff(USART3_BUF);

#ifdef USART3_ENABLE
	usart1_putstr(BLE_NAME,st_strlen(BLE_NAME));
#endif
	
	delay_ms(100);
	Clear_USART_Buff(USART3_BUF);
#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
	printf("Bluetooth�޸�����OK��\r\n");
#endif
#endif
}

/**********************************************************
**�������ܣBBluetooth_RF_BM_S02��ȡMAC��ַ
***********************************************************/
void Bluetooth_RF_BM_S02_getMAC(void)
{
	Clear_USART_Buff(USART3_BUF);

#ifdef USART3_ENABLE
	usart1_putstr("TTM:MAC-?",st_strlen("TTM:MAC-?"));
#endif
	
	delay_ms(100);
	
#ifdef USART1_DEBUG	
#ifdef BLE_DEBUG
	printf("%s\r\n",Usart3_RecvStat.Recv_start);
#endif
#endif

	Clear_USART_Buff(USART3_BUF);
}

/**********************************************************
**�������ܣBBluetooth_RF_BM_S02�޸Ĳ�����
***********************************************************/
void Bluetooth_RF_BM_S02_change_BPS(void)
{
	
#ifdef USART3_ENABLE
	usart1_putstr("TTM:BPS-115200",st_strlen("TTM:BPS-115200"));
#endif
	
	delay_ms(100);

	Clear_USART_Buff(USART3_BUF);
}


/**********************************************************
**�������ܣBBluetooth_RF_BM_S02�޸����Ӽ��ʱ��
***********************************************************/
void Bluetooth_RF_BM_S02_change_Connect_time(void)
{
	Clear_USART_Buff(USART3_BUF);

#ifdef USART3_ENABLE
	usart1_putstr("TTM:CIT-100ms",st_strlen("TTM:CIT-100ms"));
#endif
	
	delay_ms(100);

#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
	printf("%s\r\n",Usart3_RecvStat.Recv_start);
#endif
#endif
	Clear_USART_Buff(USART3_BUF);
}


/**********************************************************
**�������ܣBBluetooth_RF_BM_S02��������ģʽ
***********************************************************/
void Bluetooth_RF_BM_S02_Connect_Mode(void)
{
	BM_S02_EN(GPIO_OUT_LOW);
#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
	printf("Bluetooth��������ģʽOK\r\n");
#endif
#endif
}

/**********************************************************
**�������ܣBBluetooth_RF_BM_S02����͹���ģʽ������˯��ģʽ
***********************************************************/
void Bluetooth_RF_BM_S02_Sleep_Mode(void)
{
	BM_S02_EN(GPIO_OUT_HIGH);
#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
	printf("Bluetooth����͹���ģʽOK\r\n");
#endif
#endif
	
}

/**********************************************************
**�������ܣ��ж�ģ���Ƿ�������
**����	ֵ��0------���ӳɹ�----IO6����͵�ƽ
**			1------����ʧ��----IO6����ߵ�ƽ
***********************************************************/
uint8_t Bluetooth_RF_BM_S02_Check_Connect_State(void)
{
	if(BM_S02_Check_Connect_State == Bit_RESET)
	{	
#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
		printf("Bluetooth Connect OK\r\n");
#endif
#endif
		return 0;
	}
	else
	{
	
#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
		printf("Bluetooth connect failed!\r\n");
#endif
#endif
		return 1;
	}
}

/**********************************************************
**�������ܣ��ָ���������
**������0��ǳ�ָ�
		1����ָ�
***********************************************************/
void Bluetooth_RF_BM_S02_RestoreFactory(int rank)
{
	int j = 0;
	if(rank)
	{
		BM_S02_RestoreFactory(GPIO_OUT_LOW);
		for(j=0;j<21;j++)
		{
			delay_ms(1000);
		}		
		BM_S02_RestoreFactory(GPIO_OUT_HIGH);
	
#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
		printf("Bluetooth��ָ���������OK��\r\n");
#endif
#endif
	}
	else
	{
		BM_S02_RestoreFactory(GPIO_OUT_LOW);
		
		for(j=0;j<5;j++)
		{
			delay_ms(1000);
		}
		
		BM_S02_RestoreFactory(GPIO_OUT_HIGH);
	
#ifdef USART1_DEBUG
#ifdef BLE_DEBUG
		printf("Bluetoothǳ�ָ���������OK��\r\n");
#endif
#endif
	}
}




