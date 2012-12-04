/*
 * $FILE: retardos.c
 *
 * Pause() application
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

//NOTE: This function was created to get precise delays, don't use for long delays

#include <LPC214x.h>
#include <retardos.h>


static unsigned long sysTICs;
static unsigned long lastT1TC;

/******************************************************************************
 *
 * Function Name: initSysTime()
 *
 * Description:
 *    This function initializes the LPC's Timer 1 
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void initSysTime(void)
{
  // setup Timer 1 to count forever
  T1TCR = TCR_RESET;                    // reset & disable timer 1
  T1PR = PCLK_DIV - 1;                  // set the prescale divider
  T1MCR = 0;                            // disable match registers
  T1CCR = 0;                            // disable compare registers
  T1EMR = 0;                            // disable external match register
  T1TCR = TCR_ENABLE;                   // enable timer 1
  sysTICs = 0;
}

/******************************************************************************
 *
 * Function Name: getSysTICs()
 *
 * Description:
 *    This function returns the current syetem time in TICs.
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    The current time in TICs as represented by sysTICs
 *
 *****************************************************************************/
unsigned long getSysTICs(void)
{
  unsigned long now = T1TC;

  sysTICs += (unsigned long)(now - lastT1TC);
  lastT1TC = now;
  return sysTICs;
}


/******************************************************************************
 *
 * Function Name: getElapsedSysTICs()
 *
 * Description:
 *    This function then returns the difference in TICs between the
 *    given starting time and the current system time.
 *
 * Calling Sequence: 
 *    The starting time.
 *
 * Returns:
 *    The time difference.
 *
 *****************************************************************************/
unsigned long getElapsedSysTICs(unsigned long startTime)
{
  return getSysTICs() - startTime;
}


/******************************************************************************
 *
 * Function Name: pause()
 *
 * Description:
 *    This function does not return until the specified 'duration' in
 *    TICs has elapsed.
 *
 * Calling Sequence: 
 *    duration - length of time in TICs to wait before returning
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void pause(unsigned long duration)
{
  unsigned long startTime = getSysTICs();

  while (getElapsedSysTICs(startTime) < duration)
    DUMMY();
}
