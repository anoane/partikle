#include <LPC214x.h>
#include <stdio.h>
#include <serial1.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
struct timespec t = {5, 0}; //time struct
char *msgWrite=malloc(20);
int x=0;
uartinit1(9600); //iniciacion de puerto uart Xbee
msgWrite="123456789";
while(1)
{
x++;
printf("Writing...");
putstring_serial1(msgWrite);
printf("%s",msgWrite);
nanosleep(&t,2);
printf("write:%d",x);
}
return 0;
}
