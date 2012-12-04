/*
 * $FILE: devrtf.c
 *
 * fifo driver
 *
 * $VERSION: 1.0
 *
 * Author: Bai Shuwei <baishw06@lzu.cn or buroc@126.com> 
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

#include <config.h>
#include <fs.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include <arch/xm_syscalls.h>

enum {
	MAX_FIFO_COUNT = 16,
	MAX_FIFO_SIZE = 4096,
};
struct prtk_fifo_struct {
	int uflag;		// isused flag
	int oflag;		// isopen flag
	int path;		// is this field needed?, is just the index i in fifo[i].
};
typedef struct prtk_fifo_struct * prtk_fifo_t;
static struct prtk_fifo_struct fifos[MAX_FIFO_COUNT] = {{0, 0, -1}};  

int
rtfinit(void)
{
        int i;

        for(i = 0; i < MAX_FIFO_COUNT; i ++) {
                fifos[i].oflag = 0;
                fifos[i].path = -1;
        }
        return 0;
}

static int
rtfopen(char *name, int mode) 
{
	char *mn, *pm;
	int i, fd = 0;
	Qid qid = {0};
	
	//printf("%s\n", name);	
	mn = name + strlen("/dev/rtf");
	while(*mn && !isdigit(*mn)) {
		mn++;
	}
	
	if(!(*mn)) {
		SET_ERRNO(ENOENT);
		return -1;
	}
	qid.path = strtoul(mn, &pm, 10);
	//printf("qid.path = %d\n", qid.path);

	if(qid.path < 0 || qid.path >= MAX_FIFO_COUNT)
		return -1;

	xm_disable_events_flag();
	// rtf is one reader / one writer
	if(fifos[qid.path].path != -1) {
		xm_enable_events_flag();
		return -1;
	}

	for(i = 0; i < MAX_FIFO_COUNT; i++)
		if(fifos[i].oflag == 0)
			break;
	if(i == MAX_FIFO_COUNT) {
		xm_enable_events_flag();
		return -1;
	}
	
	if(!fifos[i].uflag++) {
		fifos[i].oflag = 1;
		fifos[i].path = i;
	}
	
	if ((fd = newfd()) >= MAX_FILES){
		freefd(fd);
		SET_ERRNO(ENOMEM);
		return -1;
	}
	files[fd].type = devno('f');
	files[fd].qid = qid;
	files[fd].pos = 0;

	xm_enable_events_flag();

	return fd;
}

static long
rtfread(int fd, void *dest, long size, long long offset) 
{
//	unsigned long flags;
	int path = files[fd].qid.path;
	int ret;

	xm_disable_events_flag();
	if(fifos[path].oflag == 0) {
		/* Not open */
		xm_enable_events_flag();
		return -1;
	}
	if(dest == NULL || size < 0) {
		/* Error parameters, segmentation fault*/
		xm_enable_events_flag();
		return -1;
 	}
	
	ret = xm_fifo_read(path, dest, size);
	
	xm_enable_events_flag();
	return ret;
}

static long
rtfwrite(int fd, const void *src, long size, long long offset)
{
	int path = files[fd].qid.path;
	int ret;

	xm_disable_events_flag();
	if(fifos[path].oflag == 0) {
		/* No open */
		xm_enable_events_flag();
		return -1;
	}
	if(src == NULL || size < 0) {
		/* Error parameters, segmentation fualt*/
		xm_enable_events_flag();
		return -1;
        }

	ret = xm_fifo_write(path, src, size);

	xm_enable_events_flag();
	return ret;
}

static void
rtfclose(int fd) 
{
	int path = files[fd].qid.path;

	xm_disable_events_flag();

	if(path < 0 || path >= MAX_FIFO_COUNT) {
		xm_enable_events_flag();
		return;
	}
	if(!(--fifos[path].uflag)) {
		fifos[path].oflag = 0;
		fifos[path].path = -1;
	}

	xm_enable_events_flag();
}

/*
static int rtf_ioctl(int fd, unsigned int request, unsigned long extra) 
{
	return 0;
}

static int rtf_mmap(int fd, void  *start,  int length, int prot, int flags, int offset, char **result)
{
	return 0;
}
*/

struct Dev rtfdevtab = {
	'f',
	"rtf",

	rtfinit,
	rtfopen,
	rtfread,
	rtfwrite,
	rtfclose,
//	ioctl: rtf_ioctl,
//	mmap: rtf_mmap
};
