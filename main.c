#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
#include "LED color.h"
#include <stdio.h>

 #define DEBUG_USARTx USART1
 #define DEBUG_USART_CLK RCC_APB2Periph_USART1
 #define DEBUG_USART_APBxClkCmd RCC_APB2PeriphClockCmd
 #define DEBUG_USART_BAUDRATE 115200

 // USART GPIO 引脚宏定義
 #define DEBUG_USART_GPIO_CLK (RCC_APB2Periph_GPIOA)
 #define DEBUG_USART_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd

 #define DEBUG_USART_TX_GPIO_PORT GPIOA
 #define DEBUG_USART_TX_GPIO_PIN GPIO_Pin_9
 #define DEBUG_USART_RX_GPIO_PORT GPIOA
 #define DEBUG_USART_RX_GPIO_PIN GPIO_Pin_10

 #define DEBUG_USART_IRQ USART1_IRQn
 #define DEBUG_USART_IRQHandler USART1_IRQHandler
 
void USART_Config(void) //USART初始化函數
 {
 GPIO_InitTypeDef GPIO_InitStructure;		//GPIO的struct 宣告變數GPIO_InitStructure
 USART_InitTypeDef USART_InitStructure;	//USART的struct 宣告變數USART_InitStructure

 // 打開串口GPIO 的時鐘
 DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);

 // 打開串口外設的時鐘
 DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

 // 將USART Tx 的GPIO 配置為推挽復用模式    為什麼???
 GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN; //TX腳位為A9
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//推挽復用模式
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速率
 GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure); //將上面的設定帶入函式

 // 將USART Rx 的GPIO 配置為浮空输入模式   為什麼用浮空???不用速率 
 GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;  //RX腳位為A10
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空模式
 GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure); //將上面的設定帶入函式

 // 配置序列埠的工作参数
 // 配置波特率
 USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
 // 配置针數據字長
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 // 配置停止位
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 // 配置校驗位
 USART_InitStructure.USART_Parity = USART_Parity_No ;
 // 配置硬件流控制
 USART_InitStructure.USART_HardwareFlowControl =
 USART_HardwareFlowControl_None;
		 /*硬件流控制是一種資料傳輸的流量控制方式，透過利用額外的硬體線路來控制資料的傳輸，以保證資料傳輸的穩定性和可靠性。
		 在串列通訊中，硬件流控制通常由兩條額外的線路來實現，一條是RTS（Request To Send）線，另一條是CTS（Clear To Send）線。
		 硬件流控制的基本原理是：發送方在發送資料前，先通過控制RTS線將CTS線拉高，請求接收方準備好接收資料。
		 接收方在準備好接收資料後，將CTS線拉高，表示已準備好接收資料。發送方在接收到CTS信號後，開始發送資料。
		 當資料發送完畢後，發送方通過控制RTS線將CTS線拉低，告知接收方資料傳輸結束。*/
 
 // 配置工作模式，收發一起
 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 // 完成串口的初始化配置
 USART_Init(DEBUG_USARTx, &USART_InitStructure);

 // 使能串口
 USART_Cmd(DEBUG_USARTx, ENABLE);
 }

 //重定義printf 函數，使用printf，板子送資料給電腦
 int fputc(int ch, FILE *f){
	 /* 發送一个字節數據到串口,板子送資料給電腦 */
	 USART_SendData(DEBUG_USARTx, (uint8_t) ch);

	 /* 等待發送完畢 */
	 while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
	 return (ch);
 }

 ///重定義scanf、getchar 等函数，板子收電腦資料
 int fgetc(FILE *f)
 {
	 /* 等待串口輸入數據,板子收電腦資料 */
	 while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

	 return USART_ReceiveData(DEBUG_USARTx);
 }
 
 void LED_GPIO_Config(){
	
	GPIO_PinConfigure(GPIOB,0,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT10MHZ);	//初始化 GPIOB,0號號腳位，上拉輸出，輸出10MHZ(LED green)
	GPIO_PinConfigure(GPIOB,1,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT10MHZ);	//初始化 GPIOB,1號號腳位，上拉輸出，輸出10MHZ(LED bule)
	GPIO_PinConfigure(GPIOB,5,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT10MHZ); //初始化 GPIOB,5號號腳位，上拉輸出，輸出10MHZ(LED red)
 
 }
 
 static void Show_Message(void);
 
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  char ch;
  
  /* 初始化RGB彩灯 */
  LED_GPIO_Config();
  
  /* 初始化USART 配置模式为 115200 8-N-1 */
  USART_Config();
	
  /* 打印指令输入提示信息 */
  Show_Message();
  
	while(1)
	{	
    /* 获取字符指令 */
   
	  scanf("%c",&ch);
    printf("receive: %c ",ch);
   
		//USART_SendData(DEBUG_USARTx, (uint8_t) ch);
    /* 根据字符指令控制RGB彩灯颜色 */

		//ch++;
		switch(ch)
    {
      case '1':
        LED_RED;
			break;
      case '2':
        LED_GREEN;
      break;
      case '3':
        LED_BLUE;
      break;
      case '4':
        LED_YELLOW;
      break;
      case '5':
        LED_PURPLE;
      break;
      case '6':
        LED_CYAN;
      break;
      case '7':
        LED_WHITE;
      break;
      case '8':
        LED_RGBOFF;
      break;
      default:
        /* 如果不是指定指令字符，打印提示信息 */
        Show_Message();
        break;      
    }   
	}	
}


/**
  * @brief  打印指令输入提示信息
  * @param  无
  * @retval 无
  */
static void Show_Message(void)
{
  printf("\r\n   这是一个通过串口通信指令控制RGB彩灯实验 \n");
  printf("使用  USART  参数为：%d 8-N-1 \n",DEBUG_USART_BAUDRATE);
  printf("开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
  printf("   指令   ------ 彩灯颜色 \n");
  printf("     1    ------    红 \n");
  printf("     2    ------    绿 \n");
  printf("     3    ------    蓝 \n");
  printf("     4    ------    黄 \n");
  printf("     5    ------    紫 \n");
  printf("     6    ------    青 \n");
  printf("     7    ------    白 \n");
  printf("     8    ------    灭 \n");  
}

void SystemInit(){
	int i=0;
	for(i=0;i<10;i++){
	}
}
