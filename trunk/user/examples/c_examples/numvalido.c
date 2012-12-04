#include <LPC214x.h>
#include <stdio.h>
#include <system.h>
#include <sys/io.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <serial1.h>

#define LENGHTMSGTOPACK 10
main()
{
struct timespec t = {5, 0}; //time struct
int x;
char *msgreceive=malloc(LENGHTMSGTOPACK);
char *msgaux=malloc(LENGHTMSGTOPACK);
while(1)
{
msgreceive="#2.513*89#";
for(x=0; x<=9; x++)
{
if(((msgreceive[x]>='0')&&(msgreceive[x]<='9'))||(msgreceive[x]=='.')||(msgreceive[x]=='#'))
msgaux[x]=msgreceive[x];
}
if(strcmp(msgaux,msgreceive)==0)
printf("cadena valida\n");
else
printf("cadena invalida\n");
nanosleep (&t, 0);
}
}
