#include <stdio.h> 
#include <string.h>
#include <serial1.h>
#include <time.h>
#include <stdlib.h>

//struct type definition
typedef struct  
{ 
  char descriptor_msg[6]; 
  int  address; 
  float num;
}msg_on_send;

//const
#define BUFSIZ 1024
#define msgStart '<'
#define msgEnd '>'
#define Separator '|'
#define msgsize strlen(buf)

main(void)
{ 
//definitions

	struct timespec t = {5, 0};
  	msg_on_send myMsg;
        char  buf[BUFSIZ];
        int i;
//init serial
        uartinit1(9600);
//asignation struct 
        strcpy(myMsg.descriptor_msg,"@nodo1");
        myMsg.address=123;
        myMsg.num=28.76;

//bucle send
  for(i=0;i<5;i++)
{
   //serialization
   snprintf(buf, sizeof(buf), "<%s|%05d|%2.2f>", myMsg.descriptor_msg, myMsg.address,myMsg.num);
   printf  ("buf has changed to : %s\n", buf);
   //casting
   unsigned char* msgtosend = (unsigned char*)&buf;
   printf("%s\n",buf);
   //send message
   putstring_serialn1(msgtosend,msgsize);
   //time sleep 
   nanosleep (&t, &t);
} 
return(0);  
}

