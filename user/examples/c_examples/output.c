#include <LPC214x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <retardos.h>

int main()
{
int j;
initSysTime();
IODIR0|= 0x00100000;
while(1)
{
IOSET0|= 0x00100000;
for(j=0;j<10;j++)
{
pause(ONE_SEC);
}
IOCLR0|= 0x00100000;
for(j=0;j<10;j++)
{
pause(ONE_SEC);
}
}
return 0;
}
