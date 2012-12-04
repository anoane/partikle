/*
 * $FILE: processor.h
 *
 * Processor related functions: Context switch, Enable/Inable
 * interrupts, etc
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

#ifndef _ARCH_PROCESSOR_H_
#define _ARCH_PROCESSOR_H_

#ifndef _KERNEL_
#error Internal file. Do not include it in your sources.
#endif

#ifndef __ASSEMBLY__

#include <arch/xm_syscalls.h>

#define IDLE xm_suspend_domain()

#define CALL_SIGNAL_HANDLER(hdl, signal, sinfo, void_p) \
  hdl (signal, sinfo, void_p)

#define CALL_CANCEL_HANDLER(hdl, arg) hdl (arg)

#define hw_cli() xm_disable_events_flag()
#define hw_sti() xm_enable_events_flag()

#define hw_restore_flags(flags) (flags)?hw_sti():hw_cli()

#define hw_save_flags(flags) flags = (xm_is_events_flag_enabled ())?1:0

#define hw_save_flags_and_cli(flags) {\
  hw_save_flags(flags); \
  hw_cli (); \
}

static inline int hw_are_interrupts_enabled (void) {
  return xm_is_events_flag_enabled ();
}

#define hw_get_stack(stack) __asm__ __volatile__ ("movl %%esp, %0\n\t" : "=m" (stack))
#define hw_set_stack(stack) __asm__ __volatile__ ("movl %0, %%esp\n\t" ::"m" (stack))

#define hw_push_stack(r) __asm__ __volatile__ ("pushl %0\n\t" : : "m" (r))
#define hw_pop_stack(r) __asm__ __volatile__ ("pop %0\n\t" : "=m" (r))


#define PUSH_REGISTERS() \
  __asm__ __volatile__("pushl %%eax\n\t" \
                       "pushl %%ebp\n\t" \
                       "pushl %%edi\n\t" \
                       "pushl %%esi\n\t" \
                       "pushl %%edx\n\t" \
                       "pushl %%ecx\n\t" \
                       "pushl %%ebx\n\t"::)

#ifdef CONFIG_FP_SUPPORT
#define PUSH_FP() \
  __asm__ __volatile__("sub $108,%%esp\n\t" \
                       "fnsave (%%esp)\n\t" \
                       "fwait\n\t"::)

#define POP_FP() \
  __asm__ __volatile__("frstor (%%esp)\n\t" \
                       "add $108, %%esp\n\t" ::)

#else

#define PUSH_FP()
#define POP_FP()

#endif

#define POP_REGISTERS() \
  __asm__ __volatile__("popl %%ebx\n\t" \
                       "popl %%ecx\n\t" \
                       "popl %%edx\n\t" \
                       "popl %%esi\n\t" \
                       "popl %%edi\n\t" \
                       "popl %%ebp\n\t" \
                       "popl %%eax\n\t"::)

#define CHANGE_THREAD(new_thread, current_thread) \
  __asm__ __volatile__("movl (%%ebx), %%edx\n\t" \
                       "pushl $1f\n\t" \
                       "movl %%esp, (%%edx)\n\t" \
                       "movl (%%ecx), %%esp\n\t" \
                       "movl %%ecx, (%%ebx)\n\t" \
                       "ret\n\t" \
                       "1:\n\t" : :"c" (new_thread), \
                       "b" (current_thread))

#define context_switch(new_thread, current_thread) \
  PUSH_REGISTERS(); \
  PUSH_FP(); \
  CHANGE_THREAD(new_thread, current_thread); \
  POP_FP(); \
  POP_REGISTERS();

#define rdtsc(low,high) \
  __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

#else

#define PUSH_REGISTERS() \
  pushl %eax; \
  pushl %ebp; \
  pushl %edi; \
  pushl %esi; \
  pushl %edx; \
  pushl %ecx; \
  pushl %ebx;

#define POP_REGISTERS() \
  popl %ebx; \
  popl %ecx; \
  popl %edx; \
  popl %esi; \
  popl %edi; \
  popl %ebp; \
  popl %eax;

#endif

#endif
