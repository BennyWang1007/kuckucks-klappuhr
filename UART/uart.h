#ifndef _UART_H
#define _UART_H

#include "../constants.c"
      
void UART_Initialize(void);
char *GetString(void);
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer(void);
void ClearInputBuffer(void);
void MyusartRead(void);
void SendString(char *send_str);
void SendNumberInt8(char num);
void SendNumberInt16(int num);
void RecordInput(void);
int8 get_int8_input(void);
int16 get_int16_input(void);

#endif  /* _UART_H */