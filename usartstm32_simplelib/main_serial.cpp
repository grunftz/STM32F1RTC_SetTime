//-------------------------INCLUDES---------------------------------------
#include "stm32f10x.h"                  // Device header
#include "serial.h"
#include "string.h"
//-------------------------END INCLUDES---------------------------------------

//-------------------------VARS---------------------------------------
char buffer [255];	  	//buffer for messages
int i,j,ticks;								//dummy & smart count
char data[6];						//data 
int count_time;					      	//time variable
//-------------------------END VARS---------------------------------------

//------------------------PROTOTYPES && other sutff---------------------------
extern "C" {void USART1_IRQHandler();}
extern "C" {void TIM4_IRQHandler();}

 void delay_ms(int ms);
 void delay_us(int us);

//---------------------END PROTPTYPs--------------------------------------


int main(void)
{
//-----------------------------RTC Base setings-----------------------------
RCC->APB1ENR |= (RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN); //enbale PWR and BKP
PWR->CR |= PWR_CR_DBP; // access to backup register
RCC->BDCR |= (RCC_BDCR_LSEON); // Enable LSE
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0); // wait until LSE is ready
RCC->BDCR |= RCC_BDCR_RTCSEL_LSE; // Select LSE
RCC->BDCR |= RCC_BDCR_RTCEN; // Enable RTC
RTC->CRL &= ~RTC_CRL_RSF; // Clear RSF
	while((RTC->CRL & RTC_CRL_RSF) == 0); // Wait
	while(!(RTC->CRL & RTC_CRL_RSF)); // Wait for the RSF bit in RTC_CRL to be set by hardware
	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for RTOFF
RTC->CRL |= RTC_CRL_CNF; // Configuration mode
RTC->PRLH = 0x0000; // Preload register
RTC->PRLL = 0x7FFF; // Per datasheet for 1 sec signal
RTC->CNTH = 0x0000; // date value ->counter register High
RTC->CNTL = 0x0000; // time value -> almost 12 o'clock :D 65535
RTC->CRL &= ~RTC_CRL_CNF;
	while((RTC->CRL & RTC_CRL_RTOFF) == 0){};
//------------------------------END RTC----------------------------------------

//--------------------------------USART---------------------------------------
	
	Usart_enable(USART1); 										// enable closck and setup pins!!!
	USART1->CR1 |=USART_CR1_RXNEIE; 		      //RXNE Interrupt Enable 
	Usart_begin(USART1,USART_STD_BAUD_9600);
	
	NVIC_EnableIRQ(USART1_IRQn);               //Enable usart IRQ 37
  Usart_println(USART1,"Loading app... \r\n");//Test usart
		
//--------------------------------END USART--------------------------------------
		
//-------------------------------TIMERS-----------------------------------------
		RCC->APB1ENR |=RCC_APB1ENR_TIM4EN; //timer $ enable GP Timer
		TIM4->PSC = 0; 											//prescaller
		TIM4->ARR =144; 											//iMHz freq  period 1uS
		TIM4->CR1|=TIM_CR1_URS;							//interrupt on overflow
		TIM4->DIER |=TIM_DIER_UIE;					//Update interupt enabled
		TIM4->EGR |=TIM_EGR_UG;							//update genration
		
		NVIC_EnableIRQ(TIM4_IRQn);					//enable TIM$ interupt number 30
		
		
		
		
//----------------------------------END TIMERS----------------------------------
	while(1)
	{

 
	
delay_ms(1000);
		count_time = 0;
		count_time = RTC->CNTH << 16;
		count_time |= RTC->CNTL & 0xffff;
		Usart_println(USART1," %i:%i:%i  ---> %i \r\n",(count_time/3600), ((count_time%3600)/60), ((count_time%3600)%60), count_time);
		if(count_time==864000)
			{count_time=0;
				RTC->CRL |= RTC_CRL_CNF;
				RTC->CNTL = 0x0000;
				RTC->CRL &= ~RTC_CRL_CNF;
		while((RTC->CRL & RTC_CRL_RTOFF) == 0);}
	
		
	}


}

	void USART1_IRQHandler(void)	
{
   //check incoming data
	      
		data[i] = USART1->DR;			//read it and store in data var
	
		if(i!=0) j =j*10+(data[i] - '0');// create int frm char array
         
	  i==(data[0]-'0')?i=0:i++;
		
				if(i==0)
				{
				  count_time=0;

					RTC->CRL |= RTC_CRL_CNF; //Configuration Flag ON
					j<=65535?RTC->CNTL = j:RTC->CNTL = j & 0xffff; // system TIME
					RTC->CRL &= ~RTC_CRL_CNF;////Configuration Flag OFF
				  while((RTC->CRL & RTC_CRL_RTOFF) == 0);
				
					j=0;
				}	
			
}	

void TIM4_IRQHandler(void)
{
	ticks++;
	TIM4->SR&= ~TIM_SR_UIF;//
	
}


void delay_us(int us)
{
	TIM4->CR1 |=TIM_CR1_CEN;
	ticks=0;
	while(ticks<us);
	TIM4->CR1 &=~TIM_CR1_CEN;
	
}

void delay_ms(int ms)
{
	TIM4->CR1 |=TIM_CR1_CEN;
	ticks=0;
	while(ticks<(ms*1000));
	TIM4->CR1 &=~TIM_CR1_CEN;
}






























		//while(Usart_available(USART1))
		//{
			//data=Usart_read(USART1);	
				
			
		//	Usart_println(USART1,"ovo je rezultat %c\r\n",data);
		//Usart_println(USART1,"%i \r\n",i);
		//++i;
		//}
		
	//23  59 59  --- 86399 ->count_time value
		//12  0 0  --- 43200   0xA8BE


