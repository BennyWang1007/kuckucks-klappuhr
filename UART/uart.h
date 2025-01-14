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
void SendNumberInt8(int8_t num);
void SendNumberInt16(int16_t num);
void SendNumberUInt8(uint8_t num);
void SendNumberUInt16(uint16_t num);
void RecordInput(void);
int8_t get_int8_input(void);
int16_t get_int16_input(void);

#endif  /* _UART_H */