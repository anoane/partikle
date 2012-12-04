//serial1.c
#ifndef _SERIAL1_H_
#define _SERIAL1_H_

#include "system.h"
#include "uart.h"
#include "sys/io.h"
#include "LPC214x.h"
#include "unistd.h"

int uartinit1 (unsigned long brate);
int putchar_serial1 (int ch);
int putc_serial1 (int ch);
void putstring_serial1 (const char *string);
int putstring_serialn1 (const char *string, int n);
int getkey_serial1 (void);
int getstring_serial1 (char *string, int n);

#endif

