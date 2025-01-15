#include <xc.h>
#include <string.h>
#include "stdio.h"  // for sprintf

#include "uart.h"

#define MY_STRLEN_LIMIT 15

char mystring[20];
int8_t lenStr = 0;

char my_input[20];
int8_t lenInput = 0;

char send_buf[20];

void UART_Initialize() {

    // set output
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;            
    
    //  Setting baud rate
    TXSTAbits.SYNC = 0;           // async
    BAUDCONbits.BRG16 = 0;          
    TXSTAbits.BRGH = 0;
    SPBRG = 12;                     // 1200 under 1MHZ, page207
    
   //   Serial enable
    RCSTAbits.SPEN = 1;         // enable
    PIR1bits.TXIF = 1;
    PIR1bits.RCIF = 0;          // clear interrupt flag
    TXSTAbits.TXEN = 1;         // enable tx
    RCSTAbits.CREN = 1;         // enable rx
    PIE1bits.TXIE = 0;          // disable tx intterrupt
    IPR1bits.TXIP = 0;          // low
//    PIE1bits.RCIE = 1;          // enable rx interrupt
    PIE1bits.RCIE = 0;          // enable rx interrupt

    IPR1bits.RCIP = 0;          // low

}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while (!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data
}


void UART_Write_Text(char* text) { // Output on Terminal
    while (*text)
        UART_Write(*text++);
}

void ClearBuffer(void) {
    for (int i = 0; i < MY_STRLEN_LIMIT; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void ClearInputBuffer(void) {
    for(int i = 0; i < MY_STRLEN_LIMIT; i++)
        my_input[i] = '\0';
    lenInput = 0;
}

void RecordInput(void) {
    for(int i = 0; i < lenStr; i++)
        my_input[i] = mystring[i];
    lenInput = lenStr;
}

void SendString(char *send_str) {
    int loop_count = strlen(send_str) / MY_STRLEN_LIMIT;
    for (int i = 0; i <= loop_count; i++) {
        char tmp[MY_STRLEN_LIMIT + 1];
        for (int k = 0; k < MY_STRLEN_LIMIT; k++) {
            tmp[k] = send_str[i * MY_STRLEN_LIMIT + k];
        }
        tmp[MY_STRLEN_LIMIT] = '\0';
        UART_Write_Text(tmp);
    }
}

void SendNumberUInt8(uint8_t num) {
    if (num == 0) {
        UART_Write('0');
        return;
    }
    
    char *p = send_buf;
    while (num > 0) {
        *p++ = num % 10 + '0';
        num /= 10;
    }
    p--;
    while (p >= send_buf) {
        UART_Write(*p--);
    }
}

void SendNumberUInt16(uint16_t num) {
    if (num == 0) {
        UART_Write('0');
        return;
    }
    
    char *p = send_buf;
    while (num > 0) {
        *p++ = num % 10 + '0';
        num /= 10;
    }
    p--;
    while (p >= send_buf) {
        UART_Write(*p--);
    }
}

void SendNumberInt16(int16_t num) {
    if (num == 0) {
        UART_Write('0');
        return;
    }

    char *p = send_buf;
    if (num < 0) {
        *p++ = '-';
        num = -num;
    }
    while (num > 0) {
        *p++ = num % 10 + '0';
        num /= 10;
    }
    p--;
    while (p >= send_buf) {
        UART_Write(*p--);
    }
}

void SendNumberInt8(int8_t num) {
    int16_t num2 = (int16_t)(num);
    SendNumberInt16(num2);
}


void MyusartRead(void) {
    /* TODObasic: try to use UART_Write to finish this function */
    while (!PIR1bits.RCIF);
    mystring[lenStr] = RCREG;

    // if is backspace
    if (mystring[lenStr] == '\b') {
        if (lenStr > 0) {
            mystring[lenStr] = '\0';
            lenStr--;
            SendString("lenStr: ");
            SendNumberInt8(lenStr);
        }
        UART_Write(0x08);
        return;
    }
    // if is enter
    if (mystring[lenStr] == '\r') {
        mystring[lenStr] = '\0';
        RecordInput();
        lenStr = 0;
        UART_Write('\r');
        UART_Write('\n');
        return;
    }
    UART_Write(RCREG);
    lenStr++;
    if (lenStr == MY_STRLEN_LIMIT)
        lenStr = 0;
    return;
}

char *GetString() {
    return my_input;
}

int8_t get_int8_input() {
    char *input = GetString();
    int8_t x = 0;
    while (1) {
        if (input[0] != '\0') {
            x = (int8_t)atoi(input);
            SendString("input = ");
            SendNumberInt8(x);
            SendString("\r\n");
            ClearBuffer();
            ClearInputBuffer();
            return x;
        }
    }
}

int16_t get_int16_input() {
    char *input = GetString();
    int16_t x = 0;
    while (1) {
        if (input[0] != '\0') {
            x = (int16_t)atoi(input);
            SendString("input = ");
            SendNumberInt16(x);
            SendString("\r\n");
            ClearBuffer();
            ClearInputBuffer();
            return x;
        }
    }
}

void __interrupt(low_priority)  Lo_ISR(void) {
    if (RCIF) {
        if (RCSTAbits.OERR) {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        MyusartRead();
    }
    return;
}