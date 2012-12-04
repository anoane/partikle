/*
 * $FILE: sensores.c
 *
 * Partikle sensor application
 *
 * $VERSION: 1.0
 *
 * Author: Pedro Aarón  <pedro_saldana@ucol.mx> , Apolinar González <apogon@ucol.mx> 
 *
 * $LICENSE:
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include <sensores.h>
#include <retardos.h>

//Specific SHT start command
void sht75_start(void)
{
    IODIR0 |= I2C_SDA;			//Write-SDA is an output
    IOSET0 = I2C_SDA;; 			//SHT_SDA = 1;
    IOSET0 = I2C_SCL; 			//SHT_SCK = 1;
    pause(ONE_MS);
    IOCLR0 = I2C_SDA; 			//SHT_SDA = 0;
    pause(ONE_MS);
    IOCLR0 = I2C_SCL; 			//SHT_SCK = 0;
    pause(ONE_MS);
    IOSET0 = I2C_SCL; 			//SHT_SCK = 1;
    pause(ONE_MS);
    IOSET0 = I2C_SDA;		        //SHT_SDA = 1;
    pause(ONE_MS);
    IOCLR0 = I2C_SCL; 			//SHT_SCK = 0;
    pause(ONE_MS);
}

//Read 8 bits from the SHT sensor
char sht75_read_byte(void)
{
    char in_byte = 0;
    unsigned char i;
    IOCLR0 = I2C_SCL; 			//SHT_SCK = 0;
    IODIR0 &= ~I2C_SDA;			//Reading-SDA is an input;

    for(i = 0; i<8; i++)
    {
        IOCLR0 = I2C_SCL; 		//SHT_SCK = 0;
        pause(ONE_MS);
        IOSET0 = I2C_SCL; 		//SHT_SCK = 1;
        pause(ONE_MS);
	in_byte = in_byte << 1;
        if (IOPIN0 & I2C_SDA)in_byte |= 1; 
    }

    //Send acknowledge to SHT7x
    IOCLR0 = I2C_SCL; 			//SHT_SCK = 0;
    IODIR0 |= I2C_SDA;			//Done Reading-SDA is an output
    IOCLR0 = I2C_SDA;
    IOSET0 = I2C_SCL; 			//SHT_SCK = 1;
    pause(ONE_MS);
    IOCLR0 = I2C_SCL; 			//SHT_SCK = 0; //Falling edge of 9th clock
    pause(ONE_MS);

    return(in_byte);
}

void sht75_send_byte(char sht75_command)
{
    unsigned char i;
    IODIR0 |= I2C_SDA; 			//Writing-SDA is an output;

    for(i = 8; i>0; i--)
    {
        IOCLR0 = I2C_SCL; 		//SHT_SCK = 0;
        pause(ONE_MS);

        if(sht75_command & (1 << (i-1)))
        {
            IOSET0 = I2C_SDA;
        }
        else
        {
            IOCLR0 = I2C_SDA;
        }
	
        IOSET0 = I2C_SCL; 		//SHT_SCK = 1;
        pause(ONE_MS);
    }
    //Wait for SHT7x to acknowledge.
    IOCLR0 = I2C_SCL; 			//SHT_SCK = 0;
    IODIR0 &= ~I2C_SDA;			//Done Reading-SDA is an input;	
    while (IOPIN0 & I2C_SDA);		//Wait for SHT to pull line low
    IOSET0 = I2C_SCL; 			//SHT_SCK = 1;
    pause(ONE_MS);
    IOCLR0 = I2C_SCL; 			//SHT_SCK = 0; //Falling edge of 9th clock
    pause(ONE_MS);

}

//Init the sensor and read out the humidity and temperature data
void sht75_read(float *temperature, float *humidity)
{
        unsigned long int response, checksum, rh;
	const float C1=-2.0468;           // for 12 Bit RH
	const float C2=+0.0367;           // for 12 Bit RH
	const float C3=-0.0000015955;     // for 12 Bit RH
	const float T1=+0.01;             // for 12 Bit RH
	const float T2=+0.00008;          // for 12 Bit RH	 
   	float rh_lin;                     // rh_lin:  Humidity linear
    	float rh_true;                    // rh_true: Temperature compensated humidity	
       
	IODIR0 = (I2C_SDA | I2C_SCL);     // set the output bit direction
    //================================================================	
    //Check temperature
    //================================================================
	sht75_start();
	sht75_send_byte(CHECK_TEMP);
	while (IOPIN0 & I2C_SDA);	  //Wait for SHT to finish measurement (SDA will be pulled low)
	response = sht75_read_byte(); 	  //Read 8MSB
	response <<= 8;
	response |= sht75_read_byte(); 	  //Read 8LSB
	
	*temperature = (response * 0.01)-39.6;
	
	response = sht75_read_byte();
	checksum = response;

    //================================================================
    //Check humidity
    //================================================================
	sht75_start(); 			 //Issue transmission start
	sht75_send_byte(CHECK_HUMD); 	 //Now send command code
	//Wait for SHT7x to pull SDA low to signal measurement completion. 
	//This can take up to 210ms for 14 bit measurements
	while (IOPIN0 & I2C_SDA);        //Wait for SHT to finish measurement (SDA will be pulled low)

	response = sht75_read_byte();    //Read 8MSB
	response <<= 8;
	response |= sht75_read_byte();   //Read 8LSB
	rh = response;
	//One more "read" to get the checksum
	response = sht75_read_byte();
	checksum = response;
	
	rh_lin=C3*rh*rh + C2*rh + C1;                   //calc. humidity from ticks to [%RH]
    	rh_true=(*temperature-25)*(T1+T2*rh)+rh_lin;    //calc. temperature compensated humidity [%RH]
 
	*humidity = rh_true;
      
        if((*temperature > 123.8) || (*temperature < -40) || (*humidity > 100.0) || (*humidity < 0.0))
         {           
            *temperature = 0.0;
            *humidity = 0.0;     
        }
}


// Initialise ADC Conver <ter
void adcInit0_6(void)
{
  // Select ADC8 Pin Connect P0.6
  PINSEL0 |= 0x00003000;
  // Initialise ADC converter
  AD1CR &= 0x00000000;			// Clear All Bit Control
  AD1CR |= 0x00000001;			// Select ADC = AIN8
  AD1CR |= 0x00000600;			// ADC Clock = VBP(PCLK) / 7
  AD1CR |= 0x00010000;                  // Busrt = 1 = Conversion Continue
  AD1CR &= 0xFFF1FFFF;			// CLKS = 000 = 10Bit : 11 Cycle Clock Conversion 
  AD1CR |= 0x00200000;			// PDN = 1 = Active ADC Module
  AD1CR &= 0xFF3FFFFF;			// TEST[1:0] = 00 = Normal Mode
  AD1CR &= 0xF7FFFFFF;			// EDGE = 0 = Conversion on Falling Edge  
}

// Read the current value of ADC P0.6
int adcRead0_6(void)
{ 
  unsigned int val;
    // Manually start conversions (rather than waiting on an external input)
    AD1CR |= 0x01000000;		 // START = 001 = Start Conversion Now 
    // Wait for the conversion to complete
    do			  			
    {
     val = AD1GDR;                   	 // Read A/D Data Register 
    }        
    while ((val & 0x80000000) == 0);     // Wait ADC Conversion Complete
   val = (val >> 6) & 0x03FF;            // Shift ADC Result to Integer
  return (val);
}

