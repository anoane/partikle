#include "system.h"
#include "lib.h"
#include "mem.h"

static char *trapnames[PsrMask+1] = {
        [ PsrMfiq ] = "Fiq interrupt",
        [ PsrMirq ] = "Irq interrupt",
        [ PsrMsvc ] = "SVC/SWI Exception",
        [ PsrMabt ] = "Prefetch Abort/Data Abort",
        [ PsrMabt+1 ] = "Data Abort",
        [ PsrMund ] = "Undefined instruction",
        [ PsrMsys ] = "Sys trap"
};

static char *
trap_str(int psr)
{
        char *str = trapnames[psr & PsrMask];
        if (!str)
                str = "Undefined trap";
        return(str);
}

//NOTE: order matters!
//struct Mach{
ulong fiqstack[8];
ulong sysstack[8];
ulong irqstack[8];
ulong abtstack[8];
ulong svcstack[KSTACK/sizeof(ulong)];
ulong undstack[8];
//} m;

static void initvic();

int
trapinit(void){
	/* Setup a stack for each mode */

	// done at startup (boot.S) to be able call main
	//setr13(PsrMsvc|PsrDirq|PsrDfiq, svcstack + nelem(svcstack));

	setr13(PsrMund|PsrDirq|PsrDfiq, undstack + nelem(undstack));
	setr13(PsrMabt|PsrDirq|PsrDfiq, abtstack + nelem(abtstack));
	setr13(PsrMfiq|PsrDirq|PsrDfiq, fiqstack + nelem(fiqstack));
	setr13(PsrMirq|PsrDirq|PsrDfiq, irqstack + nelem(irqstack));
	setr13(PsrMsys|PsrDirq|PsrDfiq, sysstack + nelem(sysstack));

	// init Virtual Interrupt Controller
	initvic();
	
	return 1;
}

enum
{
	r0, r1, r2, r3, r4, r5, r6, r7,
	r8, r9, r10, r11, r12, r13, r14, r15,

	sr = 11,
	ip = 12,
	sp = 13,
	lr = 14,
	pc = 15,
	cpsr = 16,
	spsr = 17,

	Nregs, /* size of regs*/
};

static inline void
saveregs(ulong regs[Nregs]){
	asm volatile ("str r0, %0" : "=m" (regs[r0]) : );
	asm volatile ("str r1, %0" : "=m" (regs[r1]) : );
	asm volatile ("str r2, %0" : "=m" (regs[r2]) : );
	asm volatile ("str r3, %0" : "=m" (regs[r3]) : );
	asm volatile ("str r4, %0" : "=m" (regs[r4]) : );
	asm volatile ("str r5, %0" : "=m" (regs[r5]) : );
	asm volatile ("str r6, %0" : "=m" (regs[r6]) : );
	asm volatile ("str r7, %0" : "=m" (regs[r7]) : );

	asm volatile ("str r8, %0" : "=m" (regs[r8]) : );
	asm volatile ("str r9, %0" : "=m" (regs[r9]) : );
	asm volatile ("str r10, %0" : "=m" (regs[r10]) : );
	asm volatile ("str r11, %0" : "=m" (regs[r11]) : );
	asm volatile ("str r12, %0" : "=m" (regs[r12]) : );
	asm volatile ("str r13, %0" : "=m" (regs[r13]) : );
	asm volatile ("str r14, %0" : "=m" (regs[r14]) : );
	asm volatile ("str r15, %0" : "=m" (regs[r15]) : );

	asm volatile ("mrs r0, cpsr; str r0, %0" : "=m" (regs[cpsr]) : );
	asm volatile ("mrs r0, spsr; str r0, %0" : "=m" (regs[spsr]) : );
	
	// sp points to regs[16]*sizeof(ulong) bytes
//	asm volatile ("stmdf sp!, {r0-r11}" : :);
//	asm volatile ("stmdf sp!, {r12-r15}" : :);
}

static void  panicnot(ulong *, int);

void
trap(void){
	ulong regs[Nregs];	/*= { [0 ... 15] = 0x11223344 };*/

	saveregs(regs);

	rprintf("\ntrap cps: %s (%08lx)"
		"\ntrap sps: %s (%08lx)"
		"\nret: lr: %08lx\n",
		trap_str(regs[cpsr]), regs[cpsr],
		trap_str(regs[spsr]), regs[spsr],
		regs[lr]);
	
	// nothing to do right now
	panicnot(regs, nelem(regs));
}

void
dumpregs(ulong * regs, int nregs){
	ulong n;

	for (n=0; n < nregs; n++){
		rprintf("r%d	%08lx\n", n, regs[n]);
	}
}

void
dumpstate(void){

	// dump memory layout
	rprintf("\ntext region\n");
	rprintf("%08lx %08lx\n", btext, etext);

	rprintf("data region\n");
	rprintf("%08lx %08lx %08lx\n", bdata, edata, etext);

	rprintf("bss region\n");
	rprintf("%08lx %08lx\n", bbss, ebss);
	
	// dump stacks
	rprintf("fiqstack %08lx[%d]%08lx\n", fiqstack, sizeof(fiqstack), fiqstack+nelem(fiqstack));
	rprintf("sysstack %08lx[%d]%08lx\n", sysstack, sizeof(sysstack), sysstack+nelem(sysstack));
	rprintf("irqstack %08lx[%d]%08lx\n", irqstack, sizeof(irqstack), irqstack+nelem(irqstack));
	rprintf("abtstack %08lx[%d]%08lx\n", abtstack, sizeof(abtstack), abtstack+nelem(abtstack));
	rprintf("svcstack %08lx[%d]%08lx\n", svcstack, sizeof(svcstack), svcstack+nelem(svcstack));
	rprintf("undstack %08lx[%d]%08lx\n", undstack, sizeof(undstack), undstack+nelem(undstack));
}

static void 
panicnot(ulong *regs, int nregs){
	rprintf("don't panic\n");

	dumpregs(regs, nregs);
	dumpstate();

	while(1)
		;
}

// irqs

static void
defvichandler (void)
{
	/* if the IRQ is not installed into the VIC, and interrupt occurs, the
	default interrupt VIC address will be used. This could happen in a race 
	condition. For debugging, use this endless loop to trace back. */
	/* For more details, see Philips appnote AN10414 */
    
	VICVectAddr = 0xff;		/* Acknowledge Interrupt */ 
	if(0)putstring_serial0("defvichandler\n");
	return;
}

static void
initvic(void) 
{
	ulong i = 0;
	ulong *vect_addr, *vect_cntl;
  	
	/* initialize VIC*/
	VICIntEnClr = 0xffffffff;
	VICVectAddr = 0;
	VICIntSelect = 0;

	vect_addr = (ulong *)(VICBASE + VECT_ADDR_INDEX);
	vect_cntl = (ulong *)(VICBASE + VECT_CNTL_INDEX);
	/* set all the vector and vector control register to 0 */
	for (i = 0; i < VIC_SIZE; i++, vect_addr++, vect_cntl++)
	{
		*vect_addr = 0;
		*vect_cntl = 0;
	}

	/* Install the default VIC handler here */
	VICDefVectAddr = (ulong) defvichandler;   
	return;
}

ulong
install_irq(ulong intn, void *handler)
{
	ulong i;
	ulong *vect_addr;
	ulong *vect_cntl;
	  
	VICIntEnClr = 1 << intn;	/* Disable Interrupt */
	vect_addr = (ulong *)(VICBASE + VECT_ADDR_INDEX);
	vect_cntl = (ulong *)(VICBASE + VECT_CNTL_INDEX);
	for (i = 0; i < VIC_SIZE; i++, vect_addr++, vect_cntl++)
	{
		/* find first un-assigned VIC address for the handler */
		if (*vect_addr == 0)
		{
			*vect_addr = (ulong)handler;	/* set interrupt vector */
			*vect_cntl = (ulong)((1<<VIC_ENABLE) | intn);
			break;
		}
	}
	
	if (i == VIC_SIZE)
		return FALSE;		/* fatal error, can't find empty vector slot */

	VICIntEnable = 1 << intn;	/* Enable Interrupt */
	return TRUE;
}

ulong
uninstall_irq(ulong intn)
{
	ulong i;
	ulong *vect_addr;
	ulong *vect_cntl;
	  
	VICIntEnClr = 1 << intn;	/* Disable Interrupt */
	vect_addr = (ulong *)(VICBASE + VECT_ADDR_INDEX);
	vect_cntl = (ulong *)(VICBASE + VECT_CNTL_INDEX);
	for (i = 0; i < VIC_SIZE; i++, vect_addr++, vect_cntl++)
	{
		/* find first un-assigned VIC address for the handler */
		if ((*vect_cntl & ~(1<<VIC_ENABLE) ) == intn)
		{
			*vect_addr = (ulong)0;	/* clear the VIC entry in the VIC table */
			*vect_cntl &= ~(1<<VIC_ENABLE); /* disable SLOT_EN bit */	
			break;
		}
	}

	if (i == VIC_SIZE)
		return FALSE;		/* fatal error, can't find interrupt number in vector slot */

	VICIntEnable = 1 << intn;	/* Enable Interrupt */
	return TRUE;
}

ulong
getspsr(void){
	ulong sp;
	asm volatile ("mrs  %0, spsr" : "=r" (sp) : ); // no inputs
	return sp;
}

void
setspsr(ulong sp){
	asm volatile ("msr  spsr, %0" : : "r" (sp)  ); // no outputs
}

ulong
getcpsr(void){
	ulong cp;
	asm volatile ("mrs  %0, cpsr" : "=r" (cp) : ); // no inputs
	return cp;
}

void
setcpsr(ulong cp){
	asm volatile ("msr  cpsr, %0" : : "r" (cp)  ); // no outputs
}

ulong
getcallerpc(void){
	ulong lr;
	asm volatile ("mov %0, lr" : "=r" (lr) : ); // no inputs
	return lr;
}

static inline void enableirq(void){
	ulong cp;
	
	cp = getcpsr();
	setcpsr(cp & ~PsrDirq);
	return cp;
	
	 
}

static inline void disableirq(void){
	ulong cp;
	
	cp = getcpsr();
	setcpsr(cp | PsrDirq);
	return cp;
	
}

ulong
enablefiq(void){
	ulong cp;
	
	cp = getcpsr();
	setcpsr(cp & ~PsrDfiq);
	return cp;
	
}

ulong
disablefiq(void){
	ulong cp;
	
	cp = getcpsr();
	setcpsr(cp | PsrDfiq);
	return cp;
	
}

