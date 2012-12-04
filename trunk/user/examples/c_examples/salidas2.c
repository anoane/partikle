#include <LPC214x.h>
#include <stdio.h>
#include <system.h>
#include <sys/io.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <serial1.h>

#define MAXNUMSUBMSG 118
#define LENGHTMSGTOPACK 10
#define LENGHTMSGTORESP 20

void delay(unsigned long int);
double atof(char* string);

/*char* substr(char *dest, char *src, int position, int length)
{
dest[0]='\0';
strncat(dest, (src + position), length);
return(dest);
} */

// substr implementation************************************
char* substr(char* cadena, int comienzo, int longitud)
{
longitud=0;
if (longitud == 0)
longitud = strlen(cadena)-comienzo;
char *nuevo = (char*)malloc(sizeof(char) * (longitud+1));
nuevo[longitud] = '\0';
strncpy(nuevo, cadena + comienzo, longitud);
printf("\ndesde substr=%s",nuevo);
return nuevo;

}
// end substr ***********************************************



//implementacion de la deserializacion
float deSerialize(char *MsgtoReceive){
char *MsgtoTopic=malloc(LENGHTMSGTOPACK);
float final;
  if((MsgtoReceive[0]=='#') && (MsgtoReceive[strlen(MsgtoReceive-1)]=='#')) { //reconocimiento de cadena
  MsgtoTopic=substr(MsgtoReceive,1,strlen(MsgtoReceive)-2); //llamada a substr
	//substr(MsgtoTopic, MsgtoReceive, 1, strlen(MsgtoReceive)-2);
	printf("\nsubstraccion=%s",MsgtoTopic);
  }
  final=atof(MsgtoTopic);
  printf("\nantes de liberar%f",final); //convercion string a flotante
  free(MsgtoTopic);
  printf("\ndespues de liberar%f",final);
  return final;
}

int main()
{
int x,y,band;
char *msgreceive=malloc(LENGHTMSGTOPACK);
char *MesgtoResp1=malloc(LENGHTMSGTORESP);
char *MesgtoResp2=malloc(LENGHTMSGTORESP);
float fReceive;
struct timespec t = {2, 0}; //time struct
MesgtoResp1="status warning";
MesgtoResp2="status ok";
uartinit1(38400); //iniciacion de puerto uart Xbee
while(1)
{
getstring_serial1(msgreceive,10); //lectura de puerto uart
printf("\ndesde main=%s",msgreceive);
	if((msgreceive[0]=='#') && (msgreceive[strlen(msgreceive-1)]=='#')) {
	for(x=0; x<=9; x++)
	{
		if(((msgreceive[x]>='0')&&(msgreceive[x]<='9'))||(msgreceive[x]=='.')||(msgreceive[x]=='#'))
		{
			band=1;	
		}
			else
			{
			band=0; break;
			}
	}
		if(band==1)
		{		
			fReceive=deSerialize(msgreceive); //deserializacion
			printf("\nel numero flotante es=%f\n",fReceive);
		}
			else
			printf("\ndato perdido");
		if(fReceive>=0.5)
		{
		printf("\nAlta temperatura-->%f",fReceive);
		//IODIR1 = IODIR1 | (1<<3); //Hacemos un OR lógico con el bit 3, para activar P0.3 como salida.
		//IOSET1 = 1<<3; // Poner P0.03 a '1'
		//printf("\nAdvertencia Enviada");
		nanosleep(&t,0);
			for(y=0; y<4; y++)
			{
			putstring_serial1(MesgtoResp1);
			nanosleep(&t,0);
			}
		}
		else
		{
		//printf("\nTemperatura controlada");
		nanosleep(&t,0);
			for(y=0; y<4; y++){	
			putstring_serial1(MesgtoResp2);
			nanosleep(&t,0);
			}
		}
		
}		
}
return(0);
}

//delay implementation ********************
void delay(unsigned long int count1)
{
while(count1 > 0) {
count1--; // Loop Decrease Counter
}
}

// atof implementation ********************
double  atof(char* string)
{   
    float fin=0;
    double n=0, p=0;
    int i=0, s=0;

    for ( i=0; string[i]==' ' || string[i]=='\n' || string[i]=='\t'; i++ );

    if ( string[i]=='+' || string[i]=='-' )
        s=(string[i++]=='+') ? 1:-1;
    else
        s=1;
    for ( n=0 ; string[i]>='0' && string[i]<='9'; i++ )
        n = 10*n+string[i]-'0';

    if (string[i]=='.')
        i++;
    for ( p=1 ; string[i]>='0' && string[i]<='9'; i++ )
    {
        n = 10*n+string[i]-'0';
        p *= 10;
    }
free(string);
	fin=s*n/p;
    return fin;
}
// END atof implementation *********************************

