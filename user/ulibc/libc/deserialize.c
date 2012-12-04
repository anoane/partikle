#include <sys/io.h>
#include <stdlib.h>
#include <string.h>
#include <convtopack.h>
#include <deserialize.h>

//implementacion de la deserializacion
float deSerialfloat(char *MsgtoReceive){
char *MsgtoTopic=malloc(15);
float final;
  if((MsgtoReceive[0]=='#') && (MsgtoReceive[strlen(MsgtoReceive-1)]=='#')) { //reconocimiento de cadena
  MsgtoTopic=substring(MsgtoReceive,1,strlen(MsgtoReceive)-2); //llamada a substr
  }
  final=atof(MsgtoTopic);
  free(MsgtoTopic);
  return final;
}

int deSerialint(char *MsgtoReceive){
char *MsgtoTopic=malloc(6);
float final;
  if((MsgtoReceive[0]=='#') && (MsgtoReceive[strlen(MsgtoReceive-1)]=='#')) { //reconocimiento de cadena
  MsgtoTopic=substring(MsgtoReceive,1,strlen(MsgtoReceive)-2); //llamada a substr
  }
  final=atoi(MsgtoTopic);
  free(MsgtoTopic);
  return final;
}





