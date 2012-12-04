#include <LPC214x.h>
#include <stdio.h>
#include <system.h>
#include <sys/io.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <serial1.h>

#define LENGHTMSGTOPACK 10
#define LENGHTMSGTORESP 20

void delay(unsigned long int);
char *substr(char*, int, int);
unsigned int val;
float temp;
char* MesgtoSend;

//dtoa implementation **************
char * dtoa (double value, int ndigits, char *buf)
{
  char *p = buf;
  sprintf (buf, "%-#.*g", ndigits, value);
  /* It seems they expect us to return .XXXX instead of 0.XXXX */ 
  if (*p == '-')
    p++;
  if (*p == '0' && p[1] == '.')
    memmove (p, p + 1, strlen (p + 1) + 1);
  /* They want Xe-YY, not X.e-YY, and XXXX instead of XXXX.  */
  p = strchr (buf, 'e');
  if (!p)
    {
      p = buf + strlen (buf);
      /* They don't want trailing zeroes.  */
      while (p[-1] == '0' && p > buf + 2)
        *--p = '\0';
    }
  if (p > buf && p[-1] == '.')
    memmove (p - 1, p, strlen (p) + 1);
  //printf("El bufer es: %s\n\n",buf);
  return buf;
}
//End dtoa implementation **************

//ftoa implementation **************
char *ftoa(float f, int ndigits){
   char * buffer;
   buffer=malloc(30);
     return(dtoa((double)f,ndigits,buffer));
}
//end ftoa implementation **************


char *FloatMsgtoPack(float float_subMsgtoPack, int ndigits){
  //char *MsgtoSend;
  //MsgtoSend=malloc(LENGHTMSGTOPACK);
  ftoa(float_subMsgtoPack, ndigits);
}



//----------------serializacion-------------------------

char * Serialize(float data){
   
   char* MsgtoSend, *MsgtoPack, *datatoreturn;
   MsgtoSend = malloc(LENGHTMSGTOPACK);
   MsgtoPack = malloc(LENGHTMSGTOPACK);
   strcat(MsgtoSend,"#"); //inicio del mensaje
   //Serializacion de los campos del struct del topico **************
   MsgtoPack = FloatMsgtoPack(data, 6); //cantidad de digitos a considerar
   strcat(MsgtoSend,MsgtoPack);
   strcat(MsgtoSend,"#"); //fin del mensaje
   free(MsgtoPack);
   strcpy(datatoreturn,MsgtoSend);
   return datatoreturn;
}



int main(void)
{
int z=0,x,band=0;
char *msgResp=malloc(LENGHTMSGTORESP);
struct timespec t = {5, 0}; //time struct
// ADC configuration...
PINSEL0 |= 0x00003000; // Select ADC8 Pin Connect P0.6
AD1CR &= 0x00000000; // Clear All Bit Control
AD1CR |= 0x00000001; // Select ADC = AIN8 
AD1CR |= 0x00000600; // ADC Clock = VBP(PCLK) / 7
AD1CR |= 0x00010000; // Busrt = 1 = Conversion Continue
AD1CR &= 0xFFF1FFFF; // CLKS = 000 = 10Bit : 11 Cycle Clock Conversion 
AD1CR |= 0x00200000; // PDN = 1 = Active ADC Module 
AD1CR &= 0xFF3FFFFF; // TEST[1:0] = 00 = Normal Mode 
AD1CR &= 0xF7FFFFFF; // EDGE = 0 = Conversion on Falling edge
AD1CR |= 0x01000000; // START = 001 = Start Conversion Now 
//END ADC configuration
uartinit1(38400); //iniciacion de puerto uart Xbee
while(1)
{

do
{
val = AD1DR0;
}
while ((val & 0x80000000) == 0);
val = (val >> 6) & 0x03FF;
temp = (val*3.3)/1023;
z=z+1;
printf("\n%dADC8 Result = %2.6f Volt.",z,temp);
MesgtoSend=Serialize(temp); //serialiacion
if((MesgtoSend[0]=='#') && (MesgtoSend[strlen(MesgtoSend-1)]=='#')){
for(x=0; x<=9; x++)
	{
		if(((MesgtoSend[x]>='0')&&(MesgtoSend[x]<='9'))||(MesgtoSend[x]=='.')||(MesgtoSend[x]=='#'))
		{
			band=1;	
		}
			else
			{
			band=0; break;
			}
}
}
else 
{
printf("\nDato no correctamente estructurado");
nanosleep (&t, 2);
}
if(band==1)
{
printf("el dato a enviar es:%s\n",MesgtoSend);
putstring_serial1(MesgtoSend); //envio de datos por uart
temp=0;
nanosleep (&t, 2);
getstring_serial1(msgResp,20);
printf("\nstatus control=%s",msgResp);
nanosleep (&t, 2);
}
else
{
printf("\ndato no enviado!!");
nanosleep (&t, 2);
}
}
}
//delay implementation
void delay(unsigned long int count1)
{
while(count1 > 0) {
count1--;
}
}
//substr implementation
char* substr(char* cadena, int comienzo, int longitud)
{
longitud=0;
if (longitud == 0)
longitud = strlen(cadena)-comienzo;
char *nuevo = (char*)malloc(sizeof(char) * (longitud+1));
nuevo[longitud] = '\0';
strncpy(nuevo, cadena + comienzo, longitud);
return nuevo;
}
