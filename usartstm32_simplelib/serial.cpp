#include "stm32f10x.h"                  // Device header
#include "serial.h"  
#include <stdio.h>													
#include <stdint.h>
#include "string.h"													//string lib
#include "stdlib.h"													//standarad functions lib
#include "stdarg.h"													//standard arguments lib

//-------------------------------------------
//Send data
//-------------------------------------------
//******************************************
// @fn   	->Usart_println
//
// @expl	->Send data with arguments trought TX line
//
//@param[1]	-> message string
//@param[2] -> undefined numbers of arguments %i,%f......
//
//@return	->void
//
//********************************************

void Usart_println (USART_TypeDef *pUSARTx,char *msg, ...)
{
	va_list args; 														//get the list of arguments
	va_start(args,msg);												//extract argumets from message, reformat message
	vsprintf(buffer,msg,args);								//formating message acording to passed arguments

	//fill the buffer
	
	for(int i=0; i<strlen(buffer); i++)
	{
		pUSARTx->DR=buffer[i];										//byte by byte filing sending data via TX
		
		while(!(pUSARTx->SR & USART_SR_TXE)); 		//fill the array until the TX registar is empty
		
	}
}

//-------------------------------------------
//Peripherial clock control
//-------------------------------------------
//******************************************
// @fn   	->Usart_println
//
// @expl	->Enable clocks and pin setup
//
//@param[1]	-> Usart port
//@param[2] -> Reamp pins
//
//@return	->void
//
//********************************************

void Usart_enable(USART_TypeDef *pUSARTx,  bool remap)
{
		if (pUSARTx==USART1)
		{
			//gpio A clock enabled //enabling  USART1
			RCC->APB2ENR |=RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN |RCC_APB2ENR_AFIOEN;	
			//if(!remap)		
			//	{	
					//PA9 TX pin Alternative push pull 50MHz output  -> 1011
					GPIOA->CRH |=GPIO_CRH_MODE9 |GPIO_CRH_CNF9_1;
					GPIOA->CRH &=~GPIO_CRH_CNF9_0;      
					//PA10 RX pin floating input  -> 0100 //default state of GPIO	
			//	}
			//else
				//{
					// TO DO
				//}
		}
	
		if (pUSARTx==USART2)
			{
				//gpio A clock enabled //enabling  USART2
					RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;
					RCC->APB1ENR |=RCC_APB1ENR_USART2EN;
				//pin konfiguration for uart rx/tx
				//PA2 TX pin Alternative push pull 50MHz output  -> 1011
					GPIOA->CRL |=GPIO_CRL_MODE2  |GPIO_CRL_CNF2_1;
					GPIOA->CRL &=~GPIO_CRL_CNF2_0;      
				//PA3 RX pin floating input  -> 0100 //default state of GPIO	
			}
	
		if (pUSARTx==USART3)
			{
				//gpio B clock enabled //enabling  USART3
					//RCC->APB2ENR |=RCC_APB2ENR_IOPBEN | RCC_APB1ENR_USART3EN;
				//pin konfiguration for uart rx/tx
				//PB10 TX pin Alternative push pull 50MHz output  -> 1011
					GPIOB->CRH |=GPIO_CRH_MODE10  | GPIO_CRH_CNF10_1;
					GPIOB->CRH &=~GPIO_CRH_CNF10_0;      
				//PB11 RX pin floating input  -> 0100 //default state of GPIO
			}


}

//-------------------------------------------
//Baudrate definition
//-------------------------------------------
//******************************************
// @fn   	->Usart_begin
//
// @expl	->Set Baudrate
//
//@param[1]	-> Usart port
//@param[2] -> BaudeRate
//
//@return	->void
//
//********************************************

void Usart_begin(USART_TypeDef *pUSARTx, uint32_t BaudRate)
{
	uint32_t tempFreq;
	
	pUSARTx==USART1?tempFreq=72000000:tempFreq=36000000;
	pUSARTx ->BRR=tempFreq/BaudRate;

	pUSARTx->CR1|=USART_CR1_RE|USART_CR1_TE|USART_CR1_UE;
	

}

//-------------------------------------------
//If data available
//-------------------------------------------
//******************************************
// @fn   	->Usart_available
//
// @expl	->Enable clocks and pin setup
//
//@param[1]	-> Usart port
//
//@return	->bool
//
//********************************************
bool Usart_available(USART_TypeDef *pUSARTx)
{
	bool value=false;
	
	if(pUSARTx->SR &	USART_SR_RXNE)value=true;
	
	return value;
}

//-------------------------------------------
//Read data  from buffer
//-------------------------------------------
//******************************************
// @fn   	->Usart_read
//
// @expl	->Enable clocks and pin setup
//
//@param[1]	-> Usart port
//
//@return	->uint8_t
//
//********************************************

uint8_t Usart_read(USART_TypeDef *pUSARTx)
{
	uint8_t data;
	
		data = pUSARTx ->DR;
	
		while(!(pUSARTx->SR & USART_SR_TC));
	
	return data;
}
