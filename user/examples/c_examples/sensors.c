#include<stdio.h>
#include<sensores.h>
#include<time.h>

float temperature,humidity;

int psensor(void)
{
int smov,presencia;
adcInit0_6();
presencia=adcRead0_6();
if (presencia > 0)
smov=1;
else
smov=0;
return(smov);
}

int tsensor(void)
{
int vtemp,hum,temp;
sht75_read(&temperature, &humidity);
hum = (int)humidity;
temp = (int)temperature;
if (temp > 28 )
vtemp=1;
else
vtemp=0;
return(vtemp);
}

int lsensor(void)
{
unsigned int Ilumin,Elumin;
adcInit0_6();
Ilumin=adcRead0_6();
if (Ilumin > 50)
Elumin=1;
else
Elumin=0;
return(Elumin);
}

int main(void)
{
struct timespec t = {5, 0};
//declaraciones
int sp1,sl1,st1,et1,et0,et2,et3;
/*
sp=sensor de presencia
sl=sensor de iluminancia
st=sensor de temperatura
et0=Etapa de reposo
et1=Etapa Activa
*/
//lectura de sensores
sp1=psensor();
sl1=lsensor();
st1=tsensor();
//inicializacion de estados iluminancia
et0=1;
et1=0;
//inicializacion de estados Temperatura
et2=1;
et3=0;
while(1)
{
//****** Luminosidad en base a presencia ******//
if ((et0==1)&&((sp1==1)&&(sl1==0))){
et1=1;
et0=0;
}

if ((et0==0)&&((sp1==1)&&(sl1==1))){
et0=1;
et1=0;
}

//************************************************

//****** Encendido y apagado del AC en base a la temperatura ******//

if ((et0==1)&&((sp1==1)&&(st1==1))){
et3=1;
et2=0;
}

if ((et0==0)&&((sp1==1)&&(st1==0))){
et2=1;
et3=0;
}
nanosleep (&t, &t);
}
return(0);
}

