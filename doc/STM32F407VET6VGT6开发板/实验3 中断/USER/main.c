

/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/


#include "stm32f4xx.h"
#include "LED.H"
#include "delay.h"

  





int main(void)
{	
	unsigned char value=0;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	/* 按键端口初始化 */
	Key_GPIO_Config();	
/*外部中断初始化*/
  EXTI_Config();	
	while(1)
	{
	
		} 
}	





