#include <stdio.h>
#include <stdlib.h>
#include <retardos.h>
#include <serial1.h>

int main(void)
{
long int sum;
uartinit1(9600);
initSysTime();
while(1){
putc_serial1(0x7E);
putc_serial1(0x0);
putc_serial1(0x10);
putc_serial1(0x17);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0xFF);
putc_serial1(0xFF);
putc_serial1(0xFF);
putc_serial1(0xFE);
putc_serial1(0x02);
putc_serial1(0x44);
putc_serial1(0x31); 
putc_serial1(0x05);
sum=(0x17 + 0xFF + 0xFF + 0xFF + 0xFE + 0x02 + 0x44 + 0x31 + 0x05);
putc_serial1(0xFF-(sum&0xFF));
printf("\nestado alto");
pause(FIVE_SEC);
putc_serial1(0x7E);
putc_serial1(0x0);
putc_serial1(0x10);
putc_serial1(0x17);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0x0);
putc_serial1(0xFF);
putc_serial1(0xFF);
putc_serial1(0xFF);
putc_serial1(0xFE);
putc_serial1(0x02);
putc_serial1(0x44);
putc_serial1(0x31); 
putc_serial1(0x04);
sum=(0x17 + 0xFF + 0xFF + 0xFF + 0xFE + 0x02 + 0x44 + 0x31 + 0x04);
putc_serial1(0xFF-(sum&0xFF));
printf("\nestado bajo");
pause(FIVE_SEC);
}
return 0;
}