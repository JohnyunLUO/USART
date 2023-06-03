#include "stm32f10x.h" 
#include "GPIO_STM32F10x.h"
#define __IO volatile

typedef unsigned int uint32_t;


#define SOFT_DELAY CountingDelay(0x0FFFFF)
void CountingDelay(__IO u32 nCount){
	for (; nCount != 0; nCount--);
	}

 
//#define LED_R_OFF GPIO_PortWrite (GPIOB,0,1)
	
#define	LED_R_OFF GPIO_PinWrite (GPIOB,5,1)
#define LED_R_ON  GPIO_PinWrite (GPIOB,5,0)

#define LED_G_OFF GPIO_PinWrite (GPIOB,0,1)
#define LED_G_ON  GPIO_PinWrite (GPIOB,0,0)

#define LED_B_OFF GPIO_PinWrite (GPIOB,1,1)
#define LED_B_ON  GPIO_PinWrite (GPIOB,1,0)

//#define GPIOB ((GPIO_TypeDef*) GPIOB_BASE)


//RED
#define LED_RED \
 LED_R_ON;\
 LED_G_OFF;\
 LED_B_OFF

 //Green
#define LED_GREEN  LED_R_OFF;LED_G_ON;LED_B_OFF

 //Blue
#define LED_BLUE \
 LED_R_OFF;\
 LED_G_OFF;\
 LED_B_ON


 //Yellow(RED+Green)
#define LED_YELLOW \
 LED_R_ON;\
 LED_G_ON;\
 LED_B_OFF
 
 //Purple(Red+Blue)
#define LED_PURPLE \
 LED_R_ON;\
 LED_G_OFF;\
 LED_B_ON

 //Cyan(Green+Blue)
#define LED_CYAN \
 LED_R_OFF;\
 LED_G_ON;\
 LED_B_ON

 //While(R+G+B)
#define LED_WHITE \
 LED_R_ON;\
 LED_G_ON;\
 LED_B_ON

 //Dark(All off)
#define LED_RGBOFF \
 LED_R_OFF;\
 LED_G_OFF;\
 LED_B_OFF

enum color{
	
	   RED,
		 
		 GREEN,
		 
		 BLUE,
		
	   YELLOW,
		 
		 PURPLE,
		 
		 CYAN,
		 
		 WHITE,
		 
		 DARK
		 
};


extern enum color ledcolor;