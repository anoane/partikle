/*
 * $FILE: io.h
 *
 * Port's related functions
 *
 * $VERSION: 1.0
 *
 * Author: Miguel Masmano <mimastel@doctor.upv.es>
 *
 * $LICENSE:  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _IO_H_
#define _IO_H_

//#ifndef _KERNEL_
//#error Internal file. Do not include it in your sources.
//#endif

#define io_write(addr, val) \
  (*(volatile unsigned long *)(addr) = (val))

#define io_read(addr) \
  (*(volatile unsigned long *)(addr))
  
#define nelem(v)	(sizeof(v) / sizeof(v[0]))

#define inb(a) (*(volatile unsigned char *)(a))
#define outb(a, v) (*(volatile unsigned char *)(a)) = (v)

#define inw(a) (*(volatile unsigned short *)(a))
#define outw(a, v) (*(volatile unsigned short *)(a)) = (v)

#define inl(a) (*(volatile unsigned long *)(a))
#define outl(a, v) (*(volatile unsigned long *)(a)) = (v)

#ifdef _KERNEL_
extern int iopl_sys (int level);
#endif

#endif // _IO_H_
