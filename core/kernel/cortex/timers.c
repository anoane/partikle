#include "system.h"
#include "mem.h"
#include "lib.h"

#include <time.h>
#include <hwtimers.h>
#include <processor.h>

#define	DPRINT if(0)rprintf

// Timer Control 
#define TCR_ENABLE  (1 << 0)
#define TCR_RESET   (1 << 1)

// Timer Match
#define MR0_EINT	(1<<0)
#define MR0_RESET	(1<<1)
#define MR0_STOP	(1<<2)

#define T0IR_MR0	(1<<0)

void
enable_timer(char t){
	if (t == 0)
		T0TCR |= TCR_ENABLE;
	else
		T1TCR |= TCR_ENABLE;
}

void
disable_timer(char t){
	if (t == 0)
		T0TCR &= ~TCR_ENABLE;
	else
		T1TCR &= ~TCR_ENABLE;
}

void
reset_timer(char t){
	if (t == 0)
		T0TCR |= MR0_RESET;
	else
		T1TCR |= MR0_RESET;
}

volatile vlong ticks[2] = {0, 0};
static timer_handler_t t0uhand = 0;
 
static void
t0inthand(void){
	
	if(0)rprintf("t0inthand T0MR0 %d T0PR %d\n", T0MR0, T0PR);
 
	if (t0uhand){
		if(0)rprintf("t0uhand\n");
		(*t0uhand) ();
	}
	ticks[0] ++;
	
	T0IR |= T0IR_MR0;	/* clear interrupt flag */
	VICVectAddr = 0;	/* ack interrupt */
}

#define PCLK_DIV     3
#define HZ		(PCLK / PCLK_DIV)	/*  ticks per second */

int
timerinit(int t, int hz, void * handler){

	ticks[t] = 0;
	
	if (t ==0){
		T0PR = PCLK_DIV - 1;
		T0MR0 = HZ / hz;
		T0MCR = MR0_EINT|MR0_RESET;
	
		if(0)rprintf("timerinit T0MR0 %d = %d / %d\n", T0MR0, HZ, hz);
		if (install_irq(TIMER0_INT, handler) == FALSE)
			return FALSE;
		return TRUE;
	}else{
		T1PR = PCLK_DIV - 1;
		T1MR0 = HZ / hz;
		T1MCR = MR0_EINT|MR0_RESET;
	
		if(0)rprintf("timerinit T1MR0 %d = %d / %d\n", T1MR0, HZ, hz);
		if (install_irq(TIMER1_INT, handler) == FALSE)
			return FALSE;
		return TRUE;
	}
}

// timers

static int
irq_timerinit(void){
	DPRINT("irq_timerinit\n");
	timerinit(0, 1000, t0inthand);
	return 0;
}

static void
irq_timerwrite(duration_t nsperiod){
	DPRINT("irq_timerwrite ticks[0] %d (%lld, %016llx)\n", ticks[0], nsperiod, nsperiod);

	//disable_timer(0);
	//ticks[1] = nsperiod;
	T0PR = PCLK_DIV - 1;
	T0MR0 = (nsperiod * (duration_t)HZ) / NSECS_PER_SEC;
	T0MCR = MR0_EINT|MR0_RESET;
	enable_timer(0);
}

static duration_t
irq_timergetmax(void){
	return (duration_t)((int)~0)*NSECS_PER_SEC/(duration_t)PCLK;
}

static duration_t
irq_timergetmin(void){
	return 10 * USECS_PER_SEC;
}

static void
irq_timerstop(void){
	DPRINT("irq_timerstop\n");
	disable_timer(0);
}

static timer_handler_t
irq_timerhandler(timer_handler_t nuh){
	timer_handler_t oh;
	
	DPRINT("irq_timerhandler %08x\n", nuh);
	
	oh = t0uhand;
	t0uhand = nuh;
	return oh;
}

hwtimer_t irq_timer = {
  init_hwtimer: irq_timerinit,
  set_hwtimer: irq_timerwrite,
  get_max_interval: irq_timergetmax,
  get_min_interval: irq_timergetmin,
  set_timer_handler: irq_timerhandler,
  shutdown_hwtimer: irq_timerstop,
};

	// monotonic clock
/*
	
static hwtime_t
timer0start(void){
	return 0;
}

static hwtime_t
timer0read(void){
	hwtime_t ticks=T0MR0;
	
	if (!ticks) // no match
		ticks = T0PR;
		
	DPRINT("timer0read %lld\n",
		(hwtime_t)t0counter * ticks / (hwtime_t)(PCLK));
	return (hwtime_t)t0counter * ticks / (hwtime_t)(PCLK);
}

static duration_t
timer0read_nsec(void){
	hwtime_t ticks=T0MR0;
	
	if (!ticks) // no match
		ticks = T0PR;
	
	DPRINT("timer0read_nsec %lld\n",
		(hwtime_t)t0counter * ticks * NSECS_PER_SEC / (hwtime_t)(PCLK));
	
	return ticks / (hwtime_t)(PCLK);
}

static hwtime_t
timer0hwtime2dur(hwtime_t hw){
	hwtime_t ticks=T0MR0;
	
	if (!ticks) // no match
		ticks = T0PR;
	
	DPRINT("timer0hwtime2dur %lld\n", (hwtime_t)(hw * PCLK) / ticks);
	return (hwtime_t)(hw * PCLK) / ticks;
}

static duration_t
timer0getres (void) {
	DPRINT("timer0getres %lld\n", timer0hwtime2dur(PCLK));
	return timer0hwtime2dur(PCLK);
}

static void
timer0stop(void){
	DPRINT("timer0stop\n");
}

system_clock_t timer0_clock = {
	init_clock: (void*) timer0start,
	gettime_hwt: timer0read,
	gettime_nsec: timer0read_nsec,
	hwtime2duration: timer0hwtime2dur,
	getres: timer0getres,
	shutdown_clock: timer0stop,
};
system_clock_t * monotonic_clock = &timer0_clock;
*/

// TODO add rtc also for port/time.c
hwtimer_t *hwtimers[1] = {
  &irq_timer,
};

int
getnrhwtimers (void) {
	return 1;
}

hwtimer_t*
get_best_hwtimer (void) {
	return &irq_timer;
}


void
timerslink(void){
	timerinit(0, 1000, t0inthand);
	enable_timer(0);
}
