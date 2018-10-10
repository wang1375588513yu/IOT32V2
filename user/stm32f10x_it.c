/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "key_driver.h"
#include "Data_Structure.h"
#include "usart_driver.h"
#include "RTL8710_driver.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	
}

void EXTI0_IRQHandler(void)
{
	
}

void EXTI2_IRQHandler(void)
{
	
}

void EXTI4_IRQHandler(void)
{

}

//extern IIC_Type_Def IIC_Structure;
void EXTI9_5_IRQHandler(void)
{
}

void EXTI15_10_IRQHandler(void)
{
	
}




/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USART1_IRQHandler(void)
{
	
}

uint8_t Recv2_Count = 0;
uint16_t Recv2_flag = 0;
uint8_t ch = 0;
void USART2_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//transmission mode
	{
		USART_ClearITPendingBit (USART2, USART_IT_RXNE);
		
		ch = USART2->DR;
		usart1_putc(ch);
		
#if 0
		if(at_config_status == WIFI_START_SUCCEED)
		{
			uint8_t ch = 0;
			ch = USART2->DR;
			
			if(('A' == ch)||('T' == ch))
				Recv2_Count++;
			if(Recv2_Count >= 2)
			{
				at_config_status = WIFI_SETUP_MODE;
				Recv2_Count = 0;
			}
			usart1_putc(ch);
		}
		else if(at_config_status == WIFI_TRANSFER_MODE)
		{
			*(Usart2_RecvStat.Recv_end) = USART2->DR;
			
			Usart2_RecvStat.Recv_end++;
			Usart2_RecvStat.Recvcount++;
			
			if(USART2_BUFF <= Usart2_RecvStat.Recvcount)
			{
				Usart2_RecvStat.Recvcount = 0;
				Usart2_RecvStat.Recv_end = &Usart2_buff[0];
			}
		}
		else if(at_config_status == WIFI_SETUP_MODE)//Setup Config mode
		{
			uint8_t ch = 0;
			
			ch = USART2->DR;
			
			if(ch == 'O')
			{
				Recv2_flag = 1;
				Recv2_Count++;
			}
			if(Recv2_flag&&(Recv2_Count==1)&&(ch == 'K'))
			{
				at_config_return = SUCCESS;
			}
			
			if(ch == '>')
			{
				at_config_status = WIFI_TRANSFER_MODE;
			}
			usart1_putc(ch);
		}
	#endif
	} 
}

#if 0
uint8_t ch;
uint8_t Recv_Count = 0;
uint16_t Recv_flag = 0;

void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART3);
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit (USART3, USART_IT_RXNE);
		
//		*(Usart3_RecvStat.Recv_end) = USART_ReceiveData(USART3);
		*(Usart3_RecvStat.Recv_end) = USART3->DR;
		
		Usart3_RecvStat.Recv_end++;
		Usart3_RecvStat.Recvcount++;
		
		if(USART3_BUFF <= Usart3_RecvStat.Recvcount)
		{
			Usart3_RecvStat.Recvcount = 0;
			Usart3_RecvStat.Recv_end = &Usart3_buff[0];
		}
		Recv_Count++;
		if(((u8)Usart3_buff[0] == 0xff)&&((u8)Usart3_buff[1] == 0xff))
		{			
			if(Recv_Count == 4)
			{
				Recv_flag = 0xffff&((Usart3_buff[2]<<8)|Usart3_buff[3]);
			}
			if(Recv_Count == (Recv_flag+4))
			{
				put_msg_Fifo(MSG_DATA_PACK);
				Recv_Count = 0;
				Recv_flag = 0;
			}
		}
		if(USART3->DR == '$')
		{
			printf("%c\n",USART3_BUF);
			Clear_USART_Buff(USART3_BUF);
			Recv_Count = 0;
			Recv_flag = 0;
		}

		
//	  	printf( "%c", Usart3_buff[Usart2_RecvStat.Recvcount-1]);    //将接受到的数据直接返回打印
//		usart3_putc(Usart3_buff[Usart2_RecvStat.Recvcount-1]);
		
		
	} 

}
int time_out_flag = 0;
void TIM2_IRQHandler(void)
{
	time_out_flag++;
	if (TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET)
	{
		if((time_out_flag%100) == 0)
		{
			put_msg_Fifo(MSG_200MS);
			
		}
		//printf("TIM2_IRQHandler\r\n");
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 
	}
}
#if 1
//??????????,???????,?????????????.
void RTCAlarm_IRQHandler(void)
{	
//	if(SET == RTC_GetFlagStatus(RTC_FLAG_ALR))
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{
//		RTC_ClearFlag(RTC_IT_ALR);		////???????
		
		if(EXTI_GetITStatus(EXTI_Line17))
		{
			EXTI_ClearITPendingBit(EXTI_Line17);     //???????
		}

		RTC_WaitForLastTask();  
		RTC_ClearITPendingBit(RTC_IT_ALR);  
		RTC_WaitForLastTask(); 
	}
}
#endif
#if 1
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_SEC);

		/* Enable time update */
		TimeDisplay = 1;
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{
		//printf("IT_ALR1");
//		/* Clear the RTC Second interrupt */
//		RTC_ClearITPendingBit(RTC_IT_ALR);

		//Beep_Contrl(BEEP_ON);

		/* Wait until last write operation on RTC registers has finished */
		
		if(EXTI_GetITStatus(EXTI_Line17))
		{
			EXTI_ClearITPendingBit(EXTI_Line17);     //???????
		}

		RTC_WaitForLastTask();  
		RTC_ClearITPendingBit(RTC_IT_ALR);  
		RTC_WaitForLastTask();
	}
}
#endif

void TIM3_IRQHandler(void)
{	
	
}


void SPI1_IRQHandler(void)
{
	
}
#endif


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
int time_out_flag = 0;
void TIM2_IRQHandler(void)
{
//	printf("TIM2_IRQHandler\r\n");
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
	{
		ADC_Scan();
		Key_Scan();
		time_out_flag++;
		if((time_out_flag%100) == 0)
		{
			put_msg_Fifo(MSG_200MS);
			
		}
		
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
	}
}



void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART3);
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit (USART3, USART_IT_RXNE);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
