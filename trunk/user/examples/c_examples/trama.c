#include <stdio.h>
#include <stdlib.h>
#include <retardos.h>
#include <serial1.h>


int main(void)
{
int x,analoghigh,analoglow;
int analogvalue=0;
char c,bdiscard;
uartinit1(9600);
initSysTime();
while(1)
{
c=getkey_serial1();
if (c==0x7E)
{
printf("\n%02x",c);
pause(HUNDRED_MS);
for(x=0;x<=9;x++)
{
bdiscard=getkey_serial1();
printf("_%02x",bdiscard);
pause(HUNDRED_MS);
}
analoghigh=getkey_serial1();
pause(HUNDRED_MS);
analoglow=getkey_serial1();
pause(HUNDRED_MS);
analogvalue=analoglow+(analoghigh * 256);
printf("\n%02d",analogvalue);
}
}
return 0;
}
