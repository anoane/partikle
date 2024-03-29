/*
 * $FILE: fs.c
 *
 * PaRTiKle's filesystem
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

#include <config.h>
#include <ctype.h>
#include <linkage.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fs.h>

// Definitions

struct File files [MAX_FILES] =
  {[0 ... MAX_FILES - 1] = 
	{
		type: Nodev,
		qid: {0},
		flags: 0,
		pos: 0,
	}
};

int
newfd(){
  int fd;
  for (fd = 0; fd < MAX_FILES; fd ++)
    if (files[fd].type == Nodev)
      break;

  // check/give fields in a consistent state
  return fd;
}

int
freefd(int fd){
  if (fd < 0 || MAX_FILES < fd)
	  return -1;

  // reset fd fields to reuse
  files[fd] = (struct File) { 
  	type: Nodev,
  	qid: {0},
  	pos: 0,
  	flags: 0,
  };
  return 0;
}

//----------//
// open_sys //
//----------//

asmlinkage int open_sys (const char *path, int flags, int mode) {
	char dname[KNAMELEN];
	int i, fd;
	int len;
	
	if (strncmp(path, "/dev/", 5) != 0){
		i = devno('/');
		return devtab[i]->open((char *) path, mode);
	}

	len = strlen(path);
	for (i=strlen("/dev/"); i < len; i++)
		if (path[i] == '\0' || isdigit(path[i]) || path[i] == '/')
			break;
	
	if(i == len){
		SET_ERRNO(ENOENT);
		return -1;
	}
	memset(dname, '\0', i+1);
	strncpy(dname, path+5, i-5);
	
	for(i=0; devtab[i] != NULL; i++)
		if (strcmp(devtab[i]->name, dname) == 0)
			break;
	
	if (devtab[i] == NULL){
		SET_ERRNO(ENOENT);
		return -1;
	}

	fd = devtab[i]->open((char *) path, mode);
	return fd;
}

static int
checkfd (int fd) {
  if ((unsigned int) (fd) >= MAX_FILES || files[fd].type == Nodev) {
    SET_ERRNO(EBADF);
    return -1;
  }
  return 0;
}

//-----------//
// close_sys //
//-----------//

asmlinkage int close_sys (int fd) {
  int ret;
  struct File f;
  
  if ((ret = checkfd(fd)) < 0)
  	return ret;
  f = files[fd];
  devtab[f.type] -> close (fd);
  freefd(fd);
  return 0;
}

//-----------//
// write_sys //
//-----------//

asmlinkage ssize_t write_sys (int fd, const void *buf, size_t count) {
  int ret;
  struct File f;
  char *tmp_buf=(char *)buf;
  
  if ((ret = checkfd(fd)) < 0)
  	return ret;

  f = files[fd];
  ret = devtab[f.type] -> write (fd, tmp_buf, count, f.pos);
  if (ret < 0)
    ret=-ret;

  return ret;
}

//----------//
// read_sys //
//----------//

asmlinkage ssize_t read_sys (int fd, void *buf, size_t count) {
  int ret;
  struct File f;
  
  if ((ret = checkfd(fd)) < 0)
  	return ret;
  f = files[fd];
  ret = devtab[f.type] -> read (fd, buf, count, f.pos);
  if (ret < 0)
    ret=-ret;
  return ret;
}

//----------//
// mmap_sys //
//----------//

asmlinkage caddr_t mmap_sys (void  *start, size_t length, int prot, int flags, 
 			     int fd, off_t offset) {
  int ret;
  struct File f;
  caddr_t result;

  if ((ret = checkfd(fd)) < 0)
    return (caddr_t) EBADF;
  f = files[fd];

  if (!devtab[f.type] -> mmap)
    return (caddr_t) EINVAL;

  ret = devtab[f.type] -> mmap (fd, start, length, prot, flags, offset, &result);
  if (ret != 0) {
    return (caddr_t) ret;
  }
  return result;
}

//------------//
// munmap_sys //
//------------//

asmlinkage int munmap_sys (void *start, size_t length) {
  //iounmap (start);
  return 0;
}

//-----------//
// lseek_sys //
//-----------//

asmlinkage off_t lseek_sys (int fd, off_t offset, int whence) {
  return 0;
}

//-----------//
// ioctl_sys //
//-----------//

asmlinkage int ioctl_sys (int fd, unsigned cmd, unsigned long arg) {
  int ret;
  struct File f;
  
  if ((ret = checkfd(fd)) < 0)
  	return ret;
  f = files[fd];
  ret = devtab[f.type] -> ioctl (fd, cmd, arg);
  if (ret < 0) {
    return -ret;
  }
  return 0;
}
