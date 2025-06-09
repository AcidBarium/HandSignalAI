#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"
#include "PWM.h"
#include "Motor.h"

uint8_t receivedByte = 0;

uint8_t KeyNum;		// 定义用于接收按键键码的变量
int8_t Speed = 100; // 定义速度变量
int8_t Li = 100;

int8_t isMo = 0;

int main(void)
{
	OLED_Init();

	Serial_Init();
	Motor_Init();

	// OLED_ShowString(1, 1, "Serial Test");

	while (1)
	{
		if (Serial_GetRxflag() == 1)
		{
			receivedByte = Serial_GetRxData();
			Serial_SendByte(receivedByte); // 数据回传
		}

		OLED_ShowHexNum(2, 1, receivedByte, 2);
		Speed = receivedByte;
		Li = receivedByte;

		KeyNum = Key_GetNum(); // 获取按键键码
		if (KeyNum == 1)	   // 按键1按下
		{
			isMo = (isMo + 1) % 2;
		}

		if (isMo)
		{
			OLED_ShowString(1, 1, "Speed:");	// 1行1列显示字符串Speed:
			Motor_SetSpeed(Speed);				// 设置直流电机的速度为速度变量
			OLED_ShowSignedNum(1, 7, Speed, 3); // OLED显示速度变量
		}
		else
		{
			Motor_SetSpeed(0); // 设置直流电机的速度为速度变量
			OLED_ShowString(1, 1, "LED        ");
			PWM_SetCompare1(Li);
			// for (int8_t i = 0; i <= 100; i++)
			// {
			// 	PWM_SetCompare1(i); // 依次将定时器的CCR寄存器设置为0~100，PWM占空比逐渐增大，LED逐渐变亮
			// 	Delay_ms(10);		// 延时10ms
			// }
			// for (int8_t i = 0; i <= 100; i++)
			// {
			// 	PWM_SetCompare1(100 - i); // 依次将定时器的CCR寄存器设置为100~0，PWM占空比逐渐减小，LED逐渐变暗
			// 	Delay_ms(10);			  // 延时10ms
			// }
		}
	}
}
