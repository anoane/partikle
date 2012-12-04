/*
 * $FILE: retardos.h
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

#ifndef RETARDOS_H
#define RETARDOS_H

#include <LPC214x.h>
#include <system.h>

// Timer Control 
#define TCR_ENABLE  (1 << 0)
#define TCR_RESET   (1 << 1)

//DUMMY
#define DUMMY();
// Note: with a PCLK = CCLK/2 = 60MHz/2 and a Prescale divider of 3, we
// have a resolution of 100nSec.  Given the timer's counter register is
// 32-bits, we must make a call to one of the sysTime functions at least
// every ~430 sec.

// setup parameters
#define PCLK_DIV     3
#define sysTICSperSEC   (PCLK / PCLK_DIV)	/*  ticks per second */

// some helpful times for pause()
#define TEN_US          (unsigned long)((  10e-6 * sysTICSperSEC)+ .5)
#define TWENTY_US       (unsigned long)((  20e-6 * sysTICSperSEC)+ .5)
#define ONE_MS          (unsigned long)((  1e-3 * sysTICSperSEC) + .5)
#define TWO_MS          (unsigned long)((  2e-3 * sysTICSperSEC) + .5)
#define FIVE_MS         (unsigned long)((  5e-3 * sysTICSperSEC) + .5)
#define TEN_MS          (unsigned long)(( 10e-3 * sysTICSperSEC) + .5)
#define FIFTEEN_MS      (unsigned long)(( 15e-3 * sysTICSperSEC) + .5)
#define TWENTY_MS       (unsigned long)(( 20e-3 * sysTICSperSEC) + .5)
#define THIRTY_MS       (unsigned long)(( 30e-3 * sysTICSperSEC) + .5)
#define FIFTY_MS        (unsigned long)(( 50e-3 * sysTICSperSEC) + .5)
#define HUNDRED_MS      (unsigned long)((100e-3 * sysTICSperSEC) + .5)
#define ONE_FIFTY_MS    (unsigned long)((150e-3 * sysTICSperSEC) + .5)
#define QUARTER_SEC     (unsigned long)((250e-3 * sysTICSperSEC) + .5)
#define HALF_SEC        (unsigned long)((500e-3 * sysTICSperSEC) + .5)
#define ONE_SEC         (unsigned long)(( 1.0   * sysTICSperSEC) + .5)
#define TWO_SEC         (unsigned long)(( 2.0   * sysTICSperSEC) + .5)
#define FIVE_SEC        (unsigned long)(( 5.0   * sysTICSperSEC) + .5)
#define TEN_SEC         (unsigned long)((10.0   * sysTICSperSEC) + .5)


/******************************************************************************
 *
 * Function Name: initSysTime()
 *
 * Description:
 *    This function initializes the LPC's Timer 1 for use as more precise pauser
 * 
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void initSysTime(void);

/******************************************************************************
 *
 * Function Name: getSysTICs()
 *
 * Description:
 *    This function returns the current system time in TICs.
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    The current time in TICs
 *
 *****************************************************************************/
unsigned long getSysTICs(void);

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
unsigned long getElapsedSysTICs(unsigned long startTime);

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
void pause(unsigned long duration);

#endif
