/* Copyright (c) 2000, 2013, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#ifndef MY_DIR_H
#define MY_DIR_H

#include "my_global.h"

#include <sys/stat.h>

#ifdef	__cplusplus
extern "C" {
#endif

	/* Defines for my_dir and my_stat */

#ifdef _WIN32
#define S_IROTH _S_IREAD
#define S_IFIFO _S_IFIFO
#endif

#define MY_S_IFMT	S_IFMT	/* type of file */
#define MY_S_IFDIR	S_IFDIR /* directory */
#define MY_S_IFCHR	S_IFCHR /* character special */
#define MY_S_IFBLK	S_IFBLK /* block special */
#define MY_S_IFREG	S_IFREG /* regular */
#define MY_S_IFIFO	S_IFIFO /* fifo */
#define MY_S_ISUID	S_ISUID /* set user id on execution */
#define MY_S_ISGID	S_ISGID /* set group id on execution */
#define MY_S_ISVTX	S_ISVTX /* save swapped text even after use */
#define MY_S_IREAD	S_IREAD /* read permission, owner */
#define MY_S_IWRITE	S_IWRITE	/* write permission, owner */
#define MY_S_IEXEC	S_IEXEC /* execute/search permission, owner */

#define MY_S_ISDIR(m)	(((m) & MY_S_IFMT) == MY_S_IFDIR)
#define MY_S_ISCHR(m)	(((m) & MY_S_IFMT) == MY_S_IFCHR)
#define MY_S_ISBLK(m)	(((m) & MY_S_IFMT) == MY_S_IFBLK)
#define MY_S_ISREG(m)	(((m) & MY_S_IFMT) == MY_S_IFREG)
#define MY_S_ISFIFO(m)	(((m) & MY_S_IFMT) == MY_S_IFIFO)

#define MY_DONT_SORT	512	/* my_lib; Don't sort files */
#define MY_WANT_STAT	1024	/* my_lib; stat files */

	/* typedefs for my_dir & my_stat */

#if(_MSC_VER)
#define MY_STAT struct _stati64 /* 64 bit file size */
#else
#define MY_STAT struct stat	/* Orginal struct have what we need */
#endif

/* Struct describing one file returned from my_dir */
typedef struct fileinfo
{
  char			*name;
  MY_STAT		*mystat;
} FILEINFO;

typedef struct st_my_dir	/* Struct returned from my_dir */
{
  /*
    These members are just copies of parts of DYNAMIC_ARRAY structure, 
    which is allocated right after the end of MY_DIR structure (MEM_ROOT
    for storing names is also resides there). We've left them here because
    we don't want to change code that uses my_dir.
  */
  struct fileinfo	*dir_entry;
  uint			number_off_files;
} MY_DIR;

extern MY_DIR *my_dir(const char *path,myf MyFlags);
extern void my_dirend(MY_DIR *buffer);
extern MY_STAT *my_stat(const char *path, MY_STAT *stat_area, myf my_flags);
extern int my_fstat(int filenr, MY_STAT *stat_area, myf MyFlags);

#ifdef	__cplusplus
}
#endif

#endif /* MY_DIR_H */
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
