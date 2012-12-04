/*
 * $FILE: sched_struct.h
 *
 * Structure used by the scheduler
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

#ifndef _SCHED_STRUCT_H_
#define _SCHED_STRUCT_H_

#ifndef _KERNEL_
#error Internal file. Do not include it in your sources.
#endif

#include <bitmap.h>
#include <stddef.h>

// Basically, it is composed by a 1024 lists of threads, where each
// list represents a different priority, that is, list 0 is priority 0
// and so on, to speed up the search in the structure, we use
// bitmaps, where each bit represents the state of a list, only
// ready threads are inserted in this structure

// TODO rework to fit smaller systems
// TODO correct priorities to be [min, max[ = [0, 1024[

/* priority ranges between [MaxPrio, MinPrio] */
enum{
	MaxPriolog2 = 0,
	MinPriolog2 = 10,					/* MinPriolog2 <= sizeof(int) && MinPriolog2 % 2 == 0 */
	
	/* derived from: MaxPriolog2, MinPriolog2 */
	MaxPrio			= (1<<MaxPriolog2) - 1,
	MinPrio		 = (1<<MinPriolog2) - 1,
	
	FLevellog2	= MinPriolog2 / 2,
	SLevellog2	= MinPriolog2 / 2,

	FLevelsz		= (1<<FLevellog2),
	SLevelsz		= (1<<SLevellog2),
	
	FLevelMask	= (1<<FLevellog2) - 1,
	SLevelMask	= (1<<SLevellog2) - 1,
};

#define PRIO2I(prio)	(((prio) & MinPrio) >> FLevellog2)
#define PRIO2J(prio)	((prio) & SLevelMask)
#define IJ2PRIO(i, j)	(((i) & FLevelMask) << FLevellog2) | ((j) & SLevelMask)

typedef struct active_threads_struct active_threads_t;
struct active_threads_struct {
	pthread_t tarray [FLevelsz][SLevelsz];	/* array of threads */
	bitmap_t flbmap;					/* First level bitmap */
	bitmap_t slbmap [SLevelsz];			/* Second level bitmap */
};

//---------//
// init_ss //
//---------//

static inline void
init_ss (active_threads_t *active_threads) {
	int i, j;
	for (i = 0; i < FLevelsz; i ++) {
		active_threads->slbmap [i] = clearbitmap ();
		for (j = 0; j < SLevelsz; j ++)
			active_threads->tarray [i][j] = NULL;
	}
	active_threads->flbmap = clearbitmap ();
}

//------------//
// lookfor_ss //
//------------//

/*
static int lookfor_ss (pthread_t thread, active_threads_t *active_threads) {
	int i, j;
	pthread_t p;

	for (i = 0; i < FLevelsz; i++)
		for (j = 0; j < SLevelsz; j++) {
			p = active_threads->tarray [i][j];
			while (p) {
	if (p == thread) {
		return 1;
	}
	p = p -> next_matrix;
			}
		}
	return 0;
}
*/

//-----------//
// insert_ss //
//-----------//

static inline void
insert_ss (pthread_t thread, int priority, active_threads_t *active_threads) {
	int i, j;
	
	thread -> next_matrix = thread -> prev_matrix = NULL;
	i = PRIO2I(priority);
	j = PRIO2J(priority);
	
	thread -> next_matrix = active_threads->tarray [i][j];
	if (active_threads->tarray [i][j]) 
		active_threads->tarray [i] [j] -> prev_matrix = thread;

	active_threads->tarray [i][j] = thread;
	active_threads->flbmap = setbit (active_threads->flbmap, i);
	active_threads->slbmap[i] = setbit (active_threads->slbmap[i], j);
	thread -> ss_i = i; 
	thread -> ss_j = j;
}

//----------------//
// insert_ss_tail //
//----------------//

static inline void
insert_ss_tail (pthread_t thread, int priority, active_threads_t *active_threads) {
	int i, j;
	pthread_t aux; 

	i = PRIO2I(priority);
	j = PRIO2J(priority);

	thread -> next_matrix = thread -> prev_matrix = NULL;
	
	if (active_threads -> tarray [i][j]) {
		aux = active_threads->tarray [i][j];
	
		// Aux'll be the tail
		while (aux -> next_matrix)
			aux = aux -> next_matrix;

		aux -> next_matrix = thread;
		thread -> prev_matrix = aux;
	} else {
		active_threads -> tarray [i][j] = thread;
	}

	active_threads->flbmap = setbit (active_threads->flbmap, i);
	active_threads->slbmap[i] = setbit (active_threads->slbmap[i], j);
	thread -> ss_i = i; 
	thread -> ss_j = j;
}

//-------------//
// get_head_ss //
//-------------//

// This function removes the head of a list
/*
static inline pthread_t
get_head_ss (int priority, active_threads_t *active_threads) {
	int i, j;
	pthread_t aux;
	
	i = PRIO2I(priority);
	j = PRIO2J(priority);
	
	// The correspondent list is empty
	if (!isbitset (active_threads->flbmap, i) || 
			!isbitset (active_threads->slbmap[i], j)) 
		return NULL;

	// Aux is the head of the list
	aux = active_threads->tarray[i][j];
	// Removing aux from the list
	active_threads->tarray [i][j] = aux -> next_matrix;
	aux -> next_matrix = NULL;

	if (active_threads->tarray [i][j])
		// If there are any block, it is not necessary to update the
		// bitmaps, just the pointers of this block
		active_threads->tarray [i][j] -> prev_matrix = NULL;
	else {
		// Updating bitmaps
		active_threads->slbmap[i] = clearbit (active_threads->slbmap[i], j);
		if (!active_threads->slbmap[i])
			active_threads->flbmap = clearbit (active_threads->flbmap, i);
	}
	
	return NULL;
}
*/

static inline pthread_t
get_head_ss (int *priority, active_threads_t *active_threads) {
	int i, j;
	pthread_t aux;

	if (!active_threads->flbmap) {
		if (priority)
			*priority = - 1;
		return NULL;
	}
	
	i = _ffs (active_threads->flbmap);
	j = _ffs (active_threads->slbmap [i]);

	if (priority)
		*priority = IJ2PRIO(i, j);
	
	// Aux is the head of the list
	aux = active_threads->tarray[i][j];

	// Removing aux from the list
	/*active_threads->tarray [i][j] = aux -> next_matrix;
		aux -> next_matrix = NULL;
		
		if (active_threads->tarray [i][j])
		// If there are any block, it is not necessary to update the
		// bitmaps, just the pointers of this block
		active_threads->tarray [i][j] -> prev_matrix = NULL;
		else {
		// Updating bitmaps
		active_threads->slbmap[i] = clearbit (active_threads->slbmap[i], j);
		if (!active_threads->slbmap[i])
		active_threads->flbmap = clearbit (active_threads->flbmap, i);
		}*/
	
	return aux;
}

/*
#include <stdio.h>
int _i, _j;
pthread_t _p;
static inline void check_ss (active_threads_t *active_threads) {
	printf ("++++++++++++\n");
	for (_i = 0; _i < 32; _i++)
		for (_j = 0; _j < 32; _j++) {
			//printf ("i, j: %d, %d\n", i, j);
			_p = active_threads->tarray [_i][_j];
			while (_p) {
	printf ("thread: 0x%x\n", _p);
	_p = _p -> next_matrix;
			}
		}
	printf ("<><><><>\n");
}
*/

//-----------//
// remove_ss //
//-----------//

static inline void
remove_ss (pthread_t thread, active_threads_t *active_threads) {
	int i = thread -> ss_i, j = thread -> ss_j;
	if (i == -1 || j == -1) return;

	if (thread -> prev_matrix)
		thread -> prev_matrix -> next_matrix = thread -> next_matrix;
	
	if (thread -> next_matrix)
		thread -> next_matrix -> prev_matrix = thread -> prev_matrix;
	
	if (active_threads->tarray [i][j] == thread)
		active_threads->tarray [i][j] = thread -> next_matrix;
	
	thread -> next_matrix = thread -> prev_matrix = NULL;
	if (!active_threads->tarray [i][j]) {
		// Updating bitmaps
		active_threads->slbmap[i] = clearbit (active_threads->slbmap[i], j);
		if (!active_threads->slbmap[i])
			active_threads->flbmap = clearbit (active_threads->flbmap, i);
	}

	thread -> ss_i = thread -> ss_j = -1;
}

#endif
