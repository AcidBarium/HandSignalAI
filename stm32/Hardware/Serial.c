#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData = 0;
uint8_t Serial_RxFlag = 0;

void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_Initstructure;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600; // 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 串口模式
    USART_InitStructure.USART_Parity = USART_Parity_No;             // 无奇偶校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          // 停止位为1
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 数据位为8位

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 使能接收中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         // USART1中断
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 子优先级

    NVIC_Init(&NVIC_InitStructure); // 初始化中断

    USART_Cmd(USART1, ENABLE); // 使能USART1
}

void Serial_SendByte(uint8_t data)
{
    USART_SendData(USART1, data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ; // 等待发送完成
}

void Serial_SendArray(uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        Serial_SendByte(data[i]);
    }
}

void Serial_SendString(const char *str)
{
    while (*str)
    {
        Serial_SendByte((uint8_t)*str++);
    }
}

void Serial_SendNumber(uint32_t number)
{
    char buffer[12]; // 足够存储32位整数的字符串表示
    int index = 0;

    if (number == 0)
    {
        Serial_SendByte('0');
        return;
    }

    while (number > 0)
    {
        buffer[index++] = (number % 10) + '0'; // 将数字转换为字符
        number /= 10;
    }

    // 反转字符串并发送
    for (int i = index - 1; i >= 0; i--)
    {
        Serial_SendByte(buffer[i]);
    }
}

int fputc(int ch, FILE *f)
{
    Serial_SendByte((uint8_t)ch); // 将字符发送到串口
    return ch;                    // 返回发送的字符
}

void Serial_Printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[256]; // 缓冲区大小可以根据需要调整
    vsnprintf(buffer, sizeof(buffer), format, args);

    Serial_SendString(buffer);

    va_end(args);
}

uint8_t Serial_GetRxflag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;    // 清除接收标志
        return 1; // 返回接收到的数据
    }
    else
    {
        return 0; // 如果没有接收到数据，返回0
    }
}

uint8_t Serial_GetRxData(void)
{
    return Serial_RxData; // 返回接收到的数据
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // 检查接收中断
    {
        Serial_RxData = USART_ReceiveData(USART1); // 接收数据
        Serial_RxFlag = 1;
        Serial_SendByte(Serial_RxData);                 // 回显接收到的数据
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
    }
}