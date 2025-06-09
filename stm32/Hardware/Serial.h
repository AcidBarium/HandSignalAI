#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

void Serial_Init(void);
void Serial_SendByte(uint8_t data);
void Serial_SendArray(uint8_t *data, uint16_t length);
void Serial_SendString(const char *str);
void Serial_SendNumber(uint32_t number);
void Serial_Printf(const char *format, ...);
uint8_t Serial_GetRxData(void);
uint8_t Serial_GetRxflag(void);

#endif
