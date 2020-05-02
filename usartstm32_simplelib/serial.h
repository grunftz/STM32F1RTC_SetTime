/**************************************************
*STM32F103C8 -BluePill USART driver header file v1.0
*Autor: GrunF
*Date:15.12.2019
*Machine:Thinkpad DELL 5490
*Revision notes:29.01.2020
*Minor bugs fix
*Comment fix
****************************************************/
#ifndef serial_h
#define serial_h

#include "stm32f10x.h"                  // Device header

/***************************************************
*
*GENERIC MACROS
*
****************************************************/
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					2400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 		  115200

//-------------------EXTERN DEFS----------------------------
extern char buffer[];
//----------------------------------------------------------

/***************************************************
*
*API's for GPIO 
*
****************************************************/

//-------------------------------------------
//Peripherial clock control
//-------------------------------------------

void Usart_enable(USART_TypeDef *pUSARTx, bool remap=false);																		  

//-------------------------------------------
//Baudrate definition
//-------------------------------------------


void Usart_begin(USART_TypeDef *pUSARTx, uint32_t BaudRate);	

//-------------------------------------------
//Data available in buffer
//-------------------------------------------


bool Usart_available(USART_TypeDef *pUSARTx);	

//-------------------------------------------
//Data read
//-------------------------------------------


uint8_t Usart_read(USART_TypeDef *pUSARTx);	

//-------------------------------------------
//Data send
//-------------------------------------------


void Usart_println(USART_TypeDef *pUSARTx, char *msg, ...);


#endif


