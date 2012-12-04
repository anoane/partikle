#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<serial1.h>

#define msgsize 20

int main(void)
{
//declarations
int i=0,x;
char *pch;
char *txt[4];
//reserve memory
char *data=malloc(sizeof(msgsize));
char data2[msgsize];
txt[0]=malloc(sizeof(char)*6);
txt[1]=malloc(sizeof(char)*5);
txt[2]=malloc(sizeof(char)*5);

//init serial
uartinit1(9600);
while(1)
{

//lecture serial
x=getstring_serial1(data,msgsize);
strcpy(data2,data);
printf("data2:%s\n",data2);
pch=strtok (data2,"<|>");
while(pch!=NULL)
  {

    strcpy(txt[i],pch);
    printf ("txt[%d]=%s | pch = %s\n",i,txt[i],pch);
    i++;
    pch = strtok (NULL, "<|>");
    printf ("pch siguiente = %s\n",pch);

  }
 i=0;
}
}

