#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

#define LED1_OFF GPIO_SetBits(GPIOE,GPIO_Pin_5);    //PF9输出高电平
#define LED1_ON GPIO_ResetBits(GPIOE,GPIO_Pin_5);   //PF9输出低电平
#define LED2_OFF GPIO_SetBits(GPIOE,GPIO_Pin_6);    //PF10输出高电平
#define LED2_ON GPIO_ResetBits(GPIOE,GPIO_Pin_6);   //PF10输出低电平
#define LED3_OFF GPIO_SetBits(GPIOC,GPIO_Pin_13);    //PF10输出高电平
#define LED3_ON GPIO_ResetBits(GPIOC,GPIO_Pin_13);   //PF10输出低电平


void LED_GPIO_Config(void);   /* LED 端口初始化 */

#endif /* __LED_H */