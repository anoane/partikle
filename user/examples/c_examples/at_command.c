#include <stdio.h>
#include <serial1.h>
#include <retardos.h>
#include <stdlib.h>

unsigned char testok(void)
{
unsigned char status=0;
char c;

c=getkey_serial1();
if (c==0)
status=1; // "OK"
else
status=0; // ERROR

c=getkey_serial1();

  while(c!=0x0D)
  {
    c=getkey_serial1();
  }

  printf("\nstatus=%d",status); 
  return(status);
}

unsigned char econfig(void)
{
char *cec;
cec="+++";
putstring_serial1(cec);
pause(TWO_MS);
return(testok());
}

unsigned char ecommand(char *cmd, char *param)
{
char *at;
at="AT";
putstring_serial1(at);
putstring_serial1(cmd);
putstring_serial1(param);
putchar_serial1(0x0D);

return(testok());
}

unsigned char ChangerId(char *id)
{
  econfig();
  ecommand("DL",id);
  ecommand("WR","  ");
  ecommand("CN","  ");

  return 1;
}

int main(void)
{
char *dn;
uartinit1(9600);
initSysTime();
IODIR0|= 0x00100000;
while(1){
//nodo 5678
dn="5678";
ChangerId(dn);
printf("\nnodo5678->ON");
IOSET0|= 0x00100000;
pause(FIVE_SEC);
IOCLR0|= 0x00100000;
pause(FIVE_SEC);
printf("\nnodo5678->OFF");
//nodo 6789
dn="6789";
ChangerId(dn);
printf("\nnodo6789->ON");
IOSET0|= 0x00100000;
pause(FIVE_SEC);
IOCLR0|= 0x00100000;
pause(FIVE_SEC);
printf("\nnodo6789->OFF");
}
return 0;
}
