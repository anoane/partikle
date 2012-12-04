/*
 * $FILE: sensores.h
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

#include <LPC214x.h>


//**********************************************************
#define CHECK_TEMP 0x03
#define CHECK_HUMD 0x05
#define CHECK_STAT 0x07
#define WRITE_STAT 0x06
//**********************************************************
#define	I2C_SDA   	(1<<16) //PIN0.16
#define	I2C_SCL   	(1<<17) //PIN0.17
                                //PIN0.6  Read the current value of ADC 
//SHT7x
char sht75_read_byte(void);
void sht75_send_byte(char sht75_command);
void sht75_read(float *temperature, float *humidity);
void sht75_start(void);

//ADC P0.6
void adcInit0_6(void);
int adcRead0_6(void);























