#include<serialize.h>
#include<convtopack.h>
#include <system.h>
#include <sys/io.h>
#include <LPC214x.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGHTMSGTOPACK 20

char *Serializefloat(float data){
   
   char* MsgtoSend, *MsgtoPack;
   MsgtoSend = malloc(LENGHTMSGTOPACK);
   MsgtoPack = malloc(LENGHTMSGTOPACK);
   strcat(MsgtoSend,"#"); //inicio del mensaje
   MsgtoPack = FloatMsgtoPack(data, 6); //cantidad de digitos a considerar
   strcat(MsgtoSend,MsgtoPack);
   strcat(MsgtoSend,"#");
   free(MsgtoPack);
   return MsgtoSend;
   
}

char *Serializeint(int data){
   
   char* MsgtoSend, *MsgtoPack;
   MsgtoSend = malloc(LENGHTMSGTOPACK);
   MsgtoPack = malloc(LENGHTMSGTOPACK);
   strcat(MsgtoSend,"#"); //inicio del mensaje
   MsgtoPack = IntMsgtoPack(data);
   strcat(MsgtoSend,MsgtoPack);
   strcat(MsgtoSend,"#");
   free(MsgtoPack);
   return MsgtoSend;
   
}

char *Serializedoble(double data){
   
   char* MsgtoSend, *MsgtoPack;
   MsgtoSend = malloc(LENGHTMSGTOPACK);
   MsgtoPack = malloc(LENGHTMSGTOPACK);
   strcat(MsgtoSend,"#"); //inicio del mensaje
   MsgtoPack = DoubleMsgtoPack(data, 15); //cantidad de digitos a considerar
   strcat(MsgtoSend,MsgtoPack);
   strcat(MsgtoSend,"#");
   free(MsgtoPack);
   return MsgtoSend;
   
}

char *Serializelongint(long int data){
   
   char* MsgtoSend, *MsgtoPack;
   MsgtoSend = malloc(LENGHTMSGTOPACK);
   MsgtoPack = malloc(LENGHTMSGTOPACK);
   strcat(MsgtoSend,"#"); //inicio del mensaje
   MsgtoPack = LongIntMsgtoPack(data);
   strcat(MsgtoSend,MsgtoPack);
   strcat(MsgtoSend,"#");
   free(MsgtoPack);
   return MsgtoSend;
   
}

char *Serializestring(char *data){
   
   char* MsgtoSend, *MsgtoPack;
   MsgtoSend = malloc(LENGHTMSGTOPACK);
   MsgtoPack = malloc(LENGHTMSGTOPACK);
   strcat(MsgtoSend,"#"); //inicio del mensaje
   MsgtoPack = StrMsgtoPack(data);
   strcat(MsgtoSend,MsgtoPack);
   strcat(MsgtoSend,"#");
   free(MsgtoPack);
   return MsgtoSend;
   
}
