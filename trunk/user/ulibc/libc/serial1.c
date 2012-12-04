#include <serial1.h>

// BUG: why can't work with > 38400 ??
// the erase/programming flash loader neither
int
uartinit1 (unsigned long brate)
{
	unsigned long Fdiv;
	
	PINSEL0 |= 0x00050000;				// Enable RxD1 and TxD1
	outl(Uart1+Lcr, Dlab|Wls8);	// 8 bits, no Parity, 1 Stop bit

	Fdiv = ( PCLK / 16 ) / brate ;     	// baud rate
	outl(Uart1+Dll, Fdiv % 256);	// set baud rate
	outl(Uart1+Dlm, Fdiv / 256);
	
	outl(Uart1+Lcr, Wls8);		// DLAB = 0

	outl(Uart1+Fcr, 0x07);		// enable FIFOs

	return 1;
}

/* Write character to Serial Port 0 with \n -> \r\n  */
int
putchar_serial1 (int ch){
	if (ch == '\n')  {
		while (!(inl(Uart1+Lsr) & Thre))
			;
		outl(Uart1+Thr, '\r');                  /* output CR */
	}
	while (!(inl(Uart1+Lsr) & Thre))
		;
	outl(Uart1+Thr, ch);
	return ch;
}

/* Write character to Serial Port 0 without \n -> \r\n  */
int
putc_serial1 (int ch){
	while (!(inl(Uart1+Lsr) & Thre))
		;
	outl(Uart1+Thr, ch);
	return ch;
}

void
putstring_serial1 (const char *string){
	char ch;

	while ((ch = *string)) {
		putchar_serial1(ch);
		string++;
	}
}

int
putstring_serialn1 (const char *string, int n){
	int i=0;

	while (i++<n)
		putchar_serial1(*(string++));
	return n;
}


/* read character from Serial Port   */
int
getkey_serial1 (void){
	if (inl(Uart1+Lsr) & Dr)
		return inl(Uart1+Rbr);
	return 0;
}

/* return a proper C ('\0' ended) string */
int
getstring_serial1 (char *s, int n){
	int i, c;
	
	for (i=0; i < n; i++){
		while ( (c = getkey_serial1()) == 0)
			usleep(1);
		s[i] = c;
		
	}
	
	s[i] = '\0';
	
	return n;
}

