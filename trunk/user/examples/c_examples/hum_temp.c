#include <string.h>
#include <stdio.h>
#include <sensores.h>
#include <retardos.h>
#include <time.h>

int main(void)
{
//float temperature,humidity;
//int temp,hum;
unsigned int val;
initSysTime();
PINSEL0 |= 0x00300000;
  // Initialise ADC converter
  AD1CR &= 0x00000000;			// Clear All Bit Control
  AD1CR |= 0x00000004;			// Select ADC = AIN8
  AD1CR |= 0x00000600;			// ADC Clock = VBP(PCLK) / 7
  AD1CR |= 0x00010000;                  // Busrt = 1 = Conversion Continue
  AD1CR &= 0xFFF1FFFF;			// CLKS = 000 = 10Bit : 11 Cycle Clock Conversion 
  AD1CR |= 0x00200000;			// PDN = 1 = Active ADC Module
  AD1CR &= 0xFF3FFFFF;			// TEST[1:0] = 00 = Normal Mode
  AD1CR &= 0xF7FFFFFF;
  	

while(1)
{
AD1CR |= 0x01000000;		 // START = 001 = Start Conversion Now 
    do			  			
    {
     val = AD1DR2;                   	 // Read A/D Data Register 
    }while ((val & 0x80000000) == 0);     // Wait ADC Conversion Complete
    val = (val >> 6) & 0x03FF;
    val=1024-val;
printf("\nIluminancia:%d",val);
pause(ONE_SEC);
}
return 0;
}
