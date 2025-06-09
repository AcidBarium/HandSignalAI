#include "stm32f10x.h" // Device header

/**
 * 函    数：LED初始化
 * 参    数：无
 * 返 回 值：无
 */
void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_Initstructure;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

void LED_ON(uint8_t num)
{
    if (num == 1)
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    else if (num == 2)
        GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void LED_OFF(uint8_t num)
{
    if (num == 1)
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
    else if (num == 2)
        GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void LED_Turn(uint8_t num)
{
    if (num == 1)
    {
        if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
            GPIO_SetBits(GPIOA, GPIO_Pin_1);
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    }
    else if (num == 2)
    {
        if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
            GPIO_SetBits(GPIOA, GPIO_Pin_2);
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_2);
    }
}