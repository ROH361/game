/* Copyright (c) 2000, 2014, Oracle and/or its affiliates. All rights reserved.

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

#ifndef _my_sys_h
#define _my_sys_h

#include "my_global.h"                  /* C_MODE_START, C_MODE_END */
#include "my_pthread.h"
#include "m_ctype.h"                    /* for CHARSET_INFO */

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif
#ifdef _WIN32
#include <malloc.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

C_MODE_START

#ifdef HAVE_VALGRIND
# include <valgrind/valgrind.h>
# define MEM_MALLOCLIKE_BLOCK(p1, p2, p3, p4) VALGRIND_MALLOCLIKE_BLOCK(p1, p2, p3, p4)
# define MEM_FREELIKE_BLOCK(p1, p2) VALGRIND_FREELIKE_BLOCK(p1, p2)
# include <valgrind/memcheck.h>
# define MEM_UNDEFINED(a,len) VALGRIND_MAKE_MEM_UNDEFINED(a,len)
# define MEM_NOACCESS(a,len) VALGRIND_MAKE_MEM_NOACCESS(a,len)
# define MEM_CHECK_ADDRESSABLE(a,len) VALGRIND_CHECK_MEM_IS_ADDRESSABLE(a,len)
#else /* HAVE_VALGRIND */
# define MEM_MALLOCLIKE_BLOCK(p1, p2, p3, p4) do {} while (0)
# define MEM_FREELIKE_BLOCK(p1, p2) do {} while (0)
# define MEM_UNDEFINED(a,len) ((void) 0)
# define MEM_NOACCESS(a,len) ((void) 0)
# define MEM_CHECK_ADDRESSABLE(a,len) ((void) 0)
#endif /* HAVE_VALGRIND */

#include <typelib.h>

#define MY_INIT(name)   { my_progname= name; my_init(); }

/**
  Max length of an error message generated by mysys utilities.
  Some mysys functions produce error messages. These mostly go
  to stderr.
  This constant defines the size of the buffer used to format
  the message. It should be kept in sync with MYSQL_ERRMSG_SIZE,
  since sometimes mysys errors are stored in the server diagnostics
  area, and we would like to avoid unexpected truncation.
*/
#define MYSYS_ERRMSG_SIZE   (512)
#define MYSYS_STRERROR_SIZE (128)

#define MY_FILE_ERROR	((size_t) -1)

	/* General bitmaps for my_func's */
#define MY_FFNF		1	/* Fatal if file not found */
#define MY_FNABP	2	/* Fatal if not all bytes read/writen */
#define MY_NABP		4	/* Error if not all bytes read/writen */
#define MY_FAE		8	/* Fatal if any error */
#define MY_WME		16	/* Write message on error */
#define MY_WAIT_IF_FULL 32	/* Wait and try again if disk full error */
#define MY_IGNORE_BADFD 32      /* my_sync: ignore 'bad descriptor' errors */
#define MY_SYNC_DIR     8192    /* my_create/delete/rename: sync directory */
#define MY_UNUSED       64      /* Unused (was support for RAID) */
#define MY_FULL_IO     512      /* For my_read - loop intil I/O is complete */
#define MY_DONT_CHECK_FILESIZE 128 /* Option to init_io_cache() */
#define MY_LINK_WARNING 32	/* my_redel() gives warning if links */
#define MY_COPYTIME	64	/* my_redel() copys time */
#define MY_DELETE_OLD	256	/* my_create_with_symlink() */
#define MY_RESOLVE_LINK 128	/* my_realpath(); Only resolve links */
#define MY_HOLD_ORIGINAL_MODES 128  /* my_copy() holds to file modes */
#define MY_REDEL_MAKE_BACKUP 256
#define MY_SEEK_NOT_DONE 32	/* my_lock may have to do a seek */
#define MY_DONT_WAIT	64	/* my_lock() don't wait if can't lock */
#define MY_ZEROFILL	32	/* my_malloc(), fill array with zero */
#define MY_ALLOW_ZERO_PTR 64	/* my_realloc() ; zero ptr -> malloc */
#define MY_FREE_ON_ERROR 128	/* my_realloc() ; Free old ptr on error */
#define MY_HOLD_ON_ERROR 256	/* my_realloc() ; Return old ptr on error */
#define MY_DONT_OVERWRITE_FILE 1024	/* my_copy: Don't overwrite file */
#define MY_THREADSAFE 2048      /* my_seek(): lock fd mutex */
#define MY_SYNC       4096      /* my_copy(): sync dst file */

#define MYF_RW MYF(MY_WME+MY_NABP)		/* For my_read & my_write */

#define MY_CHECK_ERROR	1	/* Params to my_end; Check open-close */
#define MY_GIVE_INFO	2	/* Give time info about process*/
#define MY_DONT_FREE_DBUG 4     /* Do not call DBUG_END() in my_end() */

/* Flags for my_error() */
#define ME_BELL		4	/* DEPRECATED: Ring bell then printing message */
#define ME_ERRORLOG	64	/* Write the error message to error log */
#define ME_FATALERROR   1024    /* Fatal statement error */

	/* Bits in last argument to fn_format */
#define MY_REPLACE_DIR		1	/* replace dir in name with 'dir' */
#define MY_REPLACE_EXT		2	/* replace extension with 'ext' */
#define MY_UNPACK_FILENAME	4	/* Unpack name (~ -> home) */
#define MY_PACK_FILENAME	8	/* Pack name (home -> ~) */
#define MY_RESOLVE_SYMLINKS	16	/* Resolve all symbolic links */
#define MY_RETURN_REAL_PATH	32	/* return full path for file */
#define MY_SAFE_PATH		64	/* Return NULL if too long path */
#define MY_RELATIVE_PATH	128	/* name is relative to 'dir' */
#define MY_APPEND_EXT           256     /* add 'ext' as additional extension*/


	/* My seek flags */
#define MY_SEEK_SET	0
#define MY_SEEK_CUR	1
#define MY_SEEK_END	2

	/* Some constants */
#define MY_WAIT_FOR_USER_TO_FIX_PANIC	60	/* in seconds */
#define MY_WAIT_GIVE_USER_A_MESSAGE	10	/* Every 10 times of prev */
#define MIN_COMPRESS_LENGTH		50	/* Don't compress small bl. */
#define DFLT_INIT_HITS  3

	/* root_alloc flags */
#define MY_KEEP_PREALLOC	1
#define MY_MARK_BLOCKS_FREE     2  /* move used to free list and reuse them */

	/* Internal error numbers (for assembler functions) */
#define MY_ERRNO_EDOM		33
#define MY_ERRNO_ERANGE		34

	/* Bits for get_date timeflag */
#define GETDATE_DATE_TIME	1
#define GETDATE_SHORT_DATE	2
#define GETDATE_HHMMSSTIME	4
#define GETDATE_GMT		8
#define GETDATE_FIXEDLENGTH	16

	/* defines when allocating data */
extern void *my_multi_malloc(PSI_memory_key key, myf flags, ...);

#include <mysql/psi/psi.h>
#include <mysql/service_mysql_alloc.h>
#include <mysql/psi/mysql_memory.h>

/*
  Switch to my_malloc() if the memory block to be allocated is bigger than
  max_alloca_sz.
*/
extern PSI_memory_key key_memory_max_alloca;
#define my_safe_alloca(size, max_alloca_sz) ((size <= max_alloca_sz) ? \
                                             my_alloca(size) : \
                                             my_malloc(key_memory_max_alloca, size, MYF(0)))
#define my_safe_afree(ptr, size, max_alloca_sz) if (size > max_alloca_sz) \
                                               my_free(ptr)

#if !defined(DBUG_OFF) || defined(HAVE_VALGRIND)
/**
  Put bad content in memory to be sure it will segfault if dereferenced.
  With Valgrind, verify that memory is addressable, and mark it undefined.
  We cache value of B because if B is expression which depends on A, memset()
  trashes value of B.
*/
#define TRASH(A,B) do {                                                 \
    const size_t l= (B);                                                \
    MEM_CHECK_ADDRESSABLE(A, l);                                        \
    memset(A, 0x8F, l);                                                 \
    MEM_UNDEFINED(A, l);                                                \
  } while (0)
#else
#define TRASH(A,B) do {} while(0)
#endif
#if defined(ENABLED_DEBUG_SYNC)
extern void (*debug_sync_C_callback_ptr)(const char *, size_t);
#define DEBUG_SYNC_C(_sync_point_name_) do {                            \
    if (debug_sync_C_callback_ptr != NULL)                              \
      (*debug_sync_C_callback_ptr)(STRING_WITH_LEN(_sync_point_name_)); } \
  while(0)
#define DEBUG_SYNC_C_IF_THD(thd, _sync_point_name_) do {                \
    if (debug_sync_C_callback_ptr != NULL && thd)                       \
      (*debug_sync_C_callback_ptr)(STRING_WITH_LEN(_sync_point_name_)); } \
  while(0)
#else
#define DEBUG_SYNC_C(_sync_point_name_)
#define DEBUG_SYNC_C_IF_THD(thd, _sync_point_name_)
#endif /* defined(ENABLED_DEBUG_SYNC) */

#ifdef HAVE_LINUX_LARGE_PAGES
extern uint my_get_large_page_size(void);
extern uchar * my_large_malloc(PSI_memory_key key, size_t size, myf my_flags);
extern void my_large_free(uchar *ptr);
extern my_bool my_use_large_pages;
extern uint    my_large_page_size;
#else
#define my_get_large_page_size() (0)
#define my_large_malloc(A,B,C) my_malloc((A),(B),(C))
#define my_large_free(A) my_free((A))
#endif /* HAVE_LINUX_LARGE_PAGES */

#define my_alloca(SZ) alloca((size_t) (SZ))
#define my_afree(PTR) {}

#include <errno.h>			/* errno is a define */

extern char *home_dir;			/* Home directory for user */
extern const char *my_progname;		/* program-name (printed in errors) */
extern char curr_dir[];		/* Current directory for user */
extern void (*error_handler_hook)(uint my_err, const char *str,myf MyFlags);
extern void (*fatal_error_handler_hook)(uint my_err, const char *str,
				       myf MyFlags);
extern void (*local_message_hook)(enum loglevel ll,
                                  const char *format, va_list args);
extern uint my_file_limit;
extern ulong my_thread_stack_size;

extern void (*proc_info_hook)(void *, const PSI_stage_info *, PSI_stage_info *,
                              const char *, const char *, const unsigned int);

/* charsets */
#define MY_ALL_CHARSETS_SIZE 2048
extern MYSQL_PLUGIN_IMPORT CHARSET_INFO *default_charset_info;
extern MYSQL_PLUGIN_IMPORT CHARSET_INFO *all_charsets[MY_ALL_CHARSETS_SIZE];
extern CHARSET_INFO compiled_charsets[];

/* statistics */
extern ulong	my_file_opened,my_stream_opened, my_tmp_file_created;
extern ulong    my_file_total_opened;
extern my_bool	my_init_done;

extern MYSQL_PLUGIN_IMPORT int my_umask;		/* Default creation mask  */
extern int my_umask_dir;

extern ulong	my_default_record_cache_size;
extern my_bool  my_disable_locking,
                my_enable_symlinks;
extern char	wild_many,wild_one,wild_prefix;
extern const char *charsets_dir;

enum cache_type
{
  TYPE_NOT_SET= 0, READ_CACHE, WRITE_CACHE,
  SEQ_READ_APPEND		/* sequential read or append */,
  READ_FIFO, READ_NET,WRITE_NET};

enum flush_type
{
  FLUSH_KEEP,           /* flush block and keep it in the cache */
  FLUSH_RELEASE,        /* flush block and remove it from the cache */
  FLUSH_IGNORE_CHANGED, /* remove block from the cache */
  /*
    As my_disable_flush_pagecache_blocks is always 0, the following option
    is strictly equivalent to FLUSH_KEEP
  */
  FLUSH_FORCE_WRITE
};

enum file_type
{
  UNOPEN = 0, FILE_BY_OPEN, FILE_BY_CREATE, STREAM_BY_FOPEN, STREAM_BY_FDOPEN,
  FILE_BY_MKSTEMP, FILE_BY_DUP
};

struct st_my_file_info
{
  char  *name;
#ifdef _WIN32
  HANDLE fhandle;   /* win32 file handle */
  int    oflag;     /* open flags, e.g O_APPEND */
#endif
  enum   file_type	type;
};

extern struct st_my_file_info *my_file_info;

typedef struct st_dynamic_array
{
  uchar *buffer;
  uint elements,max_element;
  uint alloc_increment;
  uint size_of_element;
} DYNAMIC_ARRAY;

typedef struct st_my_tmpdir
{
  char **list;
  uint cur, max;
  mysql_mutex_t mutex;
} MY_TMPDIR;

typedef struct st_dynamic_string
{
  char *str;
  size_t length,max_length,alloc_increment;
} DYNAMIC_STRING;

struct st_io_cache;
typedef int (*IO_CACHE_CALLBACK)(struct st_io_cache*);

typedef struct st_io_cache_share
{
  mysql_mutex_t       mutex;           /* To sync on reads into buffer. */
  mysql_cond_t        cond;            /* To wait for signals. */
  mysql_cond_t        cond_writer;     /* For a synchronized writer. */
  /* Offset in file corresponding to the first byte of buffer. */
  my_off_t              pos_in_file;
  /* If a synchronized write cache is the source of the data. */
  struct st_io_cache    *source_cache;
  uchar                 *buffer;         /* The read buffer. */
  uchar                 *read_end;       /* Behind last valid byte of buffer. */
  int                   running_threads; /* threads not in lock. */
  int                   total_threads;   /* threads sharing the cache. */
  int                   error;           /* Last error. */
} IO_CACHE_SHARE;

typedef struct st_io_cache		/* Used when cacheing files */
{
  /* Offset in file corresponding to the first byte of uchar* buffer. */
  my_off_t pos_in_file;
  /*
    The offset of end of file for READ_CACHE and WRITE_CACHE.
    For SEQ_READ_APPEND it the maximum of the actual end of file and
    the position represented by read_end.
  */
  my_off_t end_of_file;
  /* Points to current read position in the buffer */
  uchar	*read_pos;
  /* the non-inclusive boundary in the buffer for the currently valid read */
  uchar  *read_end;
  uchar  *buffer;				/* The read buffer */
  /* Used in ASYNC_IO */
  uchar  *request_pos;

  /* Only used in WRITE caches and in SEQ_READ_APPEND to buffer writes */
  uchar  *write_buffer;
  /*
    Only used in SEQ_READ_APPEND, and points to the current read position
    in the write buffer. Note that reads in SEQ_READ_APPEND caches can
    happen from both read buffer (uchar* buffer) and write buffer
    (uchar* write_buffer).
  */
  uchar *append_read_pos;
  /* Points to current write position in the write buffer */
  uchar *write_pos;
  /* The non-inclusive boundary of the valid write area */
  uchar *write_end;

  /*
    Current_pos and current_end are convenience variables used by
    my_b_tell() and other routines that need to know the current offset
    current_pos points to &write_pos, and current_end to &write_end in a
    WRITE_CACHE, and &read_pos and &read_end respectively otherwise
  */
  uchar  **current_pos, **current_end;

  /*
    The lock is for append buffer used in SEQ_READ_APPEND cache
    need mutex copying from append buffer to read buffer.
  */
  mysql_mutex_t append_buffer_lock;
  /*
    The following is used when several threads are reading the
    same file in parallel. They are synchronized on disk
    accesses reading the cached part of the file asynchronously.
    It should be set to NULL to disable the feature.  Only
    READ_CACHE mode is supported.
  */
  IO_CACHE_SHARE *share;

  /*
    A caller will use my_b_read() macro to read from the cache
    if the data is already in cache, it will be simply copied with
    memcpy() and internal variables will be accordinging updated with
    no functions invoked. However, if the data is not fully in the cache,
    my_b_read() will call read_function to fetch the data. read_function
    must never be invoked directly.
  */
  int (*read_function)(struct st_io_cache *,uchar *,size_t);
  /*
    Same idea as in the case of read_function, except my_b_write() needs to
    be replaced with my_b_append() for a SEQ_READ_APPEND cache
  */
  int (*write_function)(struct st_io_cache *,const uchar *,size_t);
  /*
    Specifies the type of the cache. Depending on the type of the cache
    certain operations might not be available and yield unpredicatable
    results. Details to be documented later
  */
  enum cache_type type;
  /*
    Callbacks when the actual read I/O happens. These were added and
    are currently used for binary logging of LOAD DATA INFILE - when a
    block is read from the file, we create a block create/append event, and
    when IO_CACHE is closed, we create an end event. These functions could,
    of course be used for other things
  */
  IO_CACHE_CALLBACK pre_read;
  IO_CACHE_CALLBACK post_read;
  IO_CACHE_CALLBACK pre_close;
  /*
    Counts the number of times, when we were forced to use disk. We use it to
    increase the binlog_cache_disk_use and binlog_stmt_cache_disk_use status
    variables.
  */
  ulong disk_writes;
  void* arg;				/* for use by pre/post_read */
  char *file_name;			/* if used with 'open_cached_file' */
  char *dir,*prefix;
  File file; /* file descriptor */
  /*
    seek_not_done is set by my_b_seek() to inform the upcoming read/write
    operation that a seek needs to be preformed prior to the actual I/O
    error is 0 if the cache operation was successful, -1 if there was a
    "hard" error, and the actual number of I/O-ed bytes if the read/write was
    partial.
  */
  int	seek_not_done,error;
  /* buffer_length is memory size allocated for buffer or write_buffer */
  size_t	buffer_length;
  /* read_length is the same as buffer_length except when we use async io */
  size_t  read_length;
  myf	myflags;			/* Flags used to my_read/my_write */
  /*
    alloced_buffer is 1 if the buffer was allocated by init_io_cache() and
    0 if it was supplied by the user.
    Currently READ_NET is the only one that will use a buffer allocated
    somewhere else
  */
  my_bool alloced_buffer;
  /*
    Offset of the space allotted for commit sequence number from the beginning
    of the cache that will be used to update it when the transaction has
    its commit sequence.
   */
  uint commit_seq_offset;
} IO_CACHE;

typedef int (*qsort2_cmp)(const void *, const void *, const void *);

typedef void (*my_error_reporter)(enum loglevel level, const char *format, ...)
  __attribute__((format(printf, 2, 3)));

extern my_error_reporter my_charset_error_reporter;

	/* defines for mf_iocache */

	/* Test if buffer is inited */
#define my_b_clear(info) (info)->buffer=0
#define my_b_inited(info) (info)->buffer
#define my_b_EOF INT_MIN

#define my_b_read(info,Buffer,Count) \
  ((info)->read_pos + (Count) <= (info)->read_end ?\
   (memcpy(Buffer,(info)->read_pos,(size_t) (Count)), \
    ((info)->read_pos+=(Count)),0) :\
   (*(info)->read_function)((info),Buffer,Count))

#define my_b_write(info,Buffer,Count) \
 ((info)->write_pos + (Count) <=(info)->write_end ?\
  (memcpy((info)->write_pos, (Buffer), (size_t)(Count)),\
   ((info)->write_pos+=(Count)),0) : \
   (*(info)->write_function)((info),(uchar *)(Buffer),(Count)))

#define my_b_get(info) \
  ((info)->read_pos != (info)->read_end ?\
   ((info)->read_pos++, (int) (uchar) (info)->read_pos[-1]) :\
   _my_b_get(info))

#define my_b_tell(info) ((info)->pos_in_file + \
			 (size_t) (*(info)->current_pos - (info)->request_pos))

#define my_b_get_buffer_start(info) (info)->request_pos 
#define my_b_get_bytes_in_buffer(info) (char*) (info)->read_end -   \
  (char*) my_b_get_buffer_start(info)
#define my_b_get_pos_in_file(info) (info)->pos_in_file

/* tell write offset in the SEQ_APPEND cache */
int      my_b_copy_to_file(IO_CACHE *cache, FILE *file);
my_off_t my_b_append_tell(IO_CACHE* info);
my_off_t my_b_safe_tell(IO_CACHE* info); /* picks the correct tell() */

#define my_b_bytes_in_cache(info) (size_t) (*(info)->current_end - \
					  *(info)->current_pos)

typedef uint32 ha_checksum;

#include <my_alloc.h>


	/* Prototypes for mysys and my_func functions */

extern int my_copy(const char *from,const char *to,myf MyFlags);
extern int my_delete(const char *name,myf MyFlags);
extern int my_getwd(char * buf,size_t size,myf MyFlags);
extern int my_setwd(const char *dir,myf MyFlags);
extern int my_lock(File fd,int op,my_off_t start, my_off_t length,myf MyFlags);
extern void *my_once_alloc(size_t Size,myf MyFlags);
extern void my_once_free(void);
extern char *my_once_strdup(const char *src,myf myflags);
extern void *my_once_memdup(const void *src, size_t len, myf myflags);
extern File my_open(const char *FileName,int Flags,myf MyFlags);
extern File my_register_filename(File fd, const char *FileName,
				 enum file_type type_of_file,
				 uint error_message_number, myf MyFlags);
extern File my_create(const char *FileName,int CreateFlags,
		      int AccessFlags, myf MyFlags);
extern int my_close(File Filedes,myf MyFlags);
extern int my_mkdir(const char *dir, int Flags, myf MyFlags);
extern int my_readlink(char *to, const char *filename, myf MyFlags);
extern int my_is_symlink(const char *filename);
extern int my_realpath(char *to, const char *filename, myf MyFlags);
extern File my_create_with_symlink(const char *linkname, const char *filename,
				   int createflags, int access_flags,
				   myf MyFlags);
extern int my_delete_with_symlink(const char *name, myf MyFlags);
extern int my_rename_with_symlink(const char *from,const char *to,myf MyFlags);
extern int my_symlink(const char *content, const char *linkname, myf MyFlags);
extern size_t my_read(File Filedes,uchar *Buffer,size_t Count,myf MyFlags);
extern size_t my_pread(File Filedes,uchar *Buffer,size_t Count,my_off_t offset,
		     myf MyFlags);
extern int my_rename(const char *from,const char *to,myf MyFlags);
extern my_off_t my_seek(File fd,my_off_t pos,int whence,myf MyFlags);
extern my_off_t my_tell(File fd,myf MyFlags);
extern size_t my_write(File Filedes,const uchar *Buffer,size_t Count,
		     myf MyFlags);
extern size_t my_pwrite(File Filedes,const uchar *Buffer,size_t Count,
		      my_off_t offset,myf MyFlags);
extern size_t my_fread(FILE *stream,uchar *Buffer,size_t Count,myf MyFlags);
extern size_t my_fwrite(FILE *stream,const uchar *Buffer,size_t Count,
		      myf MyFlags);
extern my_off_t my_fseek(FILE *stream,my_off_t pos,int whence,myf MyFlags);
extern my_off_t my_ftell(FILE *stream,myf MyFlags);

/* implemented in my_syslog.c */

// Maximum size of message  that will be logged.
#define MAX_SYSLOG_MESSAGE_SIZE 1024

/* Platform-independent SysLog support */

/* facilities on unixoid syslog. harmless on systemd / Win platforms. */
typedef struct st_syslog_facility { int id; const char *name; } SYSLOG_FACILITY;
extern SYSLOG_FACILITY syslog_facility[];

enum my_syslog_options { MY_SYSLOG_PIDS= 1 };

int my_openlog(const char *eventSourceName, int option, int facility);
int my_closelog();
int my_syslog(const CHARSET_INFO *cs, enum loglevel level, const char *msg);

/* implemented in my_memmem.c */
extern void *my_memmem(const void *haystack, size_t haystacklen,
                       const void *needle, size_t needlelen);


#ifdef _WIN32
extern int      my_access(const char *path, int amode);
#else
#define my_access access
#endif

extern int check_if_legal_filename(const char *path);
extern int check_if_legal_tablename(const char *path);

#ifdef _WIN32
extern my_bool is_filename_allowed(const char *name, size_t length,
                   my_bool allow_current_dir);
#else /* _WIN32 */
# define is_filename_allowed(name, length, allow_cwd) (TRUE)
#endif /* _WIN32 */ 

#ifdef _WIN32
extern int nt_share_delete(const char *name,myf MyFlags);
#define my_delete_allow_opened(fname,flags)  nt_share_delete((fname),(flags))
#else
#define my_delete_allow_opened(fname,flags)  my_delete((fname),(flags))
#endif

#ifdef _WIN32
/* Windows-only functions (CRT equivalents)*/
extern HANDLE   my_get_osfhandle(File fd);
extern void     my_osmaperr(unsigned long last_error);
#endif

extern void init_glob_errs(void);
extern const char** get_global_errmsgs();
extern void wait_for_free_space(const char *filename, int errors);
extern FILE *my_fopen(const char *FileName,int Flags,myf MyFlags);
extern FILE *my_fdopen(File Filedes,const char *name, int Flags,myf MyFlags);
extern FILE *my_freopen(const char *path, const char *mode, FILE *stream);
extern int my_fclose(FILE *fd,myf MyFlags);
extern File my_fileno(FILE *fd);
extern int my_chsize(File fd,my_off_t newlength, int filler, myf MyFlags);
extern void thr_set_sync_wait_callback(void (*before_sync)(void),
                                       void (*after_sync)(void));
extern int my_sync(File fd, myf my_flags);
extern int my_sync_dir(const char *dir_name, myf my_flags);
extern int my_sync_dir_by_file(const char *file_name, myf my_flags);
extern char *my_strerror(char *buf, size_t len, int errnum);
extern const char *my_get_err_msg(int nr);
extern void my_error(int nr,myf MyFlags, ...);
extern void my_printf_error(uint my_err, const char *format,
                            myf MyFlags, ...)
  __attribute__((format(printf, 2, 4)));
extern void my_printv_error(uint error, const char *format, myf MyFlags,
                            va_list ap);
extern int my_error_register(const char** (*get_errmsgs) (),
                             int first, int last);
extern const char **my_error_unregister(int first, int last);
extern void my_message(uint my_err, const char *str,myf MyFlags);
extern void my_message_stderr(uint my_err, const char *str, myf MyFlags);
void my_message_local_stderr(enum loglevel ll,
                             const char *format, va_list args);
extern void my_message_local(enum loglevel ll, const char *format, ...);
extern my_bool my_init(void);
extern void my_end(int infoflag);
extern int my_redel(const char *from, const char *to, int MyFlags);
extern int my_copystat(const char *from, const char *to, int MyFlags);
extern char * my_filename(File fd);
extern my_bool my_chmod(const char *filename, ulong PermFlags, myf my_flags);

#ifdef EXTRA_DEBUG
void my_print_open_files(void);
#else
#define my_print_open_files()
#endif

extern my_bool init_tmpdir(MY_TMPDIR *tmpdir, const char *pathlist);
extern char *my_tmpdir(MY_TMPDIR *tmpdir);
extern void free_tmpdir(MY_TMPDIR *tmpdir);

extern size_t dirname_part(char * to,const char *name, size_t *to_res_length);
extern size_t dirname_length(const char *name);
#define base_name(A) (A+dirname_length(A))
extern int test_if_hard_path(const char *dir_name);
extern my_bool has_path(const char *name);
extern char *convert_dirname(char *to, const char *from, const char *from_end);
extern void to_unix_path(char * name);
extern char * fn_ext(const char *name);
extern char * fn_same(char * toname,const char *name,int flag);
extern char * fn_format(char * to,const char *name,const char *dir,
			   const char *form, uint flag);
extern size_t strlength(const char *str);
extern void pack_dirname(char * to,const char *from);
extern size_t normalize_dirname(char * to, const char *from);
extern size_t unpack_dirname(char * to,const char *from);
extern size_t cleanup_dirname(char * to,const char *from);
extern size_t system_filename(char * to,const char *from);
extern size_t unpack_filename(char * to,const char *from);
extern char * intern_filename(char * to,const char *from);
extern char * directory_file_name(char * dst, const char *src);
extern int pack_filename(char * to, const char *name, size_t max_length);
extern char * my_path(char * to,const char *progname,
			 const char *own_pathname_part);
extern char * my_load_path(char * to, const char *path,
			      const char *own_path_prefix);
extern int wild_compare(const char *str,const char *wildstr,
                        pbool str_is_pattern);
extern my_bool array_append_string_unique(const char *str,
                                          const char **array, size_t size);
extern void get_date(char * to,int timeflag,time_t use_time);
extern void soundex(CHARSET_INFO *, char * out_pntr, char * in_pntr,
                    pbool remove_garbage);

extern my_bool radixsort_is_appliccable(uint n_items, size_t size_of_element);
extern void radixsort_for_str_ptr(uchar* base[], uint number_of_elements,
				  size_t size_of_element,uchar *buffer[]);
extern void my_qsort(void *base_ptr, size_t total_elems, size_t size,
                     qsort_cmp cmp);
extern void my_qsort2(void *base_ptr, size_t total_elems, size_t size,
                      qsort2_cmp cmp, const void *cmp_argument);
extern qsort2_cmp get_ptr_compare(size_t);
void my_store_ptr(uchar *buff, size_t pack_length, my_off_t pos);
my_off_t my_get_ptr(uchar *ptr, size_t pack_length);
extern int init_io_cache(IO_CACHE *info,File file,size_t cachesize,
			 enum cache_type type,my_off_t seek_offset,
			 pbool use_async_io, myf cache_myflags);
extern my_bool reinit_io_cache(IO_CACHE *info,enum cache_type type,
			       my_off_t seek_offset,pbool use_async_io,
			       pbool clear_cache);
extern void setup_io_cache(IO_CACHE* info);
extern int _my_b_read(IO_CACHE *info,uchar *Buffer,size_t Count);
extern int _my_b_read_r(IO_CACHE *info,uchar *Buffer,size_t Count);
extern void init_io_cache_share(IO_CACHE *read_cache, IO_CACHE_SHARE *cshare,
                                IO_CACHE *write_cache, uint num_threads);
extern void remove_io_thread(IO_CACHE *info);
extern int _my_b_seq_read(IO_CACHE *info,uchar *Buffer,size_t Count);
extern int _my_b_net_read(IO_CACHE *info,uchar *Buffer,size_t Count);
extern int _my_b_get(IO_CACHE *info);
extern int _my_b_write(IO_CACHE *info,const uchar *Buffer,size_t Count);
extern int my_b_append(IO_CACHE *info,const uchar *Buffer,size_t Count);
extern int my_b_safe_write(IO_CACHE *info,const uchar *Buffer,size_t Count);

extern int my_block_write(IO_CACHE *info, const uchar *Buffer,
			  size_t Count, my_off_t pos);
extern int my_b_flush_io_cache(IO_CACHE *info, int need_append_buffer_lock);

#define flush_io_cache(info) my_b_flush_io_cache((info),1)

extern int end_io_cache(IO_CACHE *info);
extern size_t my_b_fill(IO_CACHE *info);
extern void my_b_seek(IO_CACHE *info,my_off_t pos);
extern size_t my_b_gets(IO_CACHE *info, char *to, size_t max_length);
extern my_off_t my_b_filelength(IO_CACHE *info);
extern size_t my_b_printf(IO_CACHE *info, const char* fmt, ...)
  __attribute__((format(printf, 2, 3)));
extern size_t my_b_vprintf(IO_CACHE *info, const char* fmt, va_list ap);
extern my_bool open_cached_file(IO_CACHE *cache,const char *dir,
				 const char *prefix, size_t cache_size,
				 myf cache_myflags);
extern my_bool real_open_cached_file(IO_CACHE *cache);
extern void close_cached_file(IO_CACHE *cache);
File create_temp_file(char *to, const char *dir, const char *pfx,
		      int mode, myf MyFlags);

// Use Prealloced_array or std::vector or something similar in C++
#if defined(__cplusplus)

#define init_dynamic_array please_use_an_appropriately_typed_container
#define my_init_dynamic_array please_use_an_appropriately_typed_container

#else

extern my_bool my_init_dynamic_array(DYNAMIC_ARRAY *array, uint element_size,
                                     void *init_buffer, uint init_alloc,
                                     uint alloc_increment);
/* init_dynamic_array() function is deprecated */
extern my_bool init_dynamic_array(DYNAMIC_ARRAY *array, uint element_size,
                                  uint init_alloc, uint alloc_increment);
#define dynamic_element(array,array_index,type) \
  ((type)((array)->buffer) +(array_index))

#endif  /* __cplusplus */

/* Some functions are still in use in C++, because HASH uses DYNAMIC_ARRAY */
extern my_bool insert_dynamic(DYNAMIC_ARRAY *array, const void *element);
extern void *alloc_dynamic(DYNAMIC_ARRAY *array);
extern void *pop_dynamic(DYNAMIC_ARRAY*);
extern void get_dynamic(DYNAMIC_ARRAY *array, void *element, uint array_index);
extern void delete_dynamic(DYNAMIC_ARRAY *array);
extern void freeze_size(DYNAMIC_ARRAY *array);
static inline void reset_dynamic(DYNAMIC_ARRAY *array)
{
  array->elements= 0;
}

extern my_bool init_dynamic_string(DYNAMIC_STRING *str, const char *init_str,
				   size_t init_alloc,size_t alloc_increment);
extern my_bool dynstr_append(DYNAMIC_STRING *str, const char *append);
my_bool dynstr_append_mem(DYNAMIC_STRING *str, const char *append,
			  size_t length);
extern my_bool dynstr_append_os_quoted(DYNAMIC_STRING *str, const char *append,
                                       ...);
extern my_bool dynstr_set(DYNAMIC_STRING *str, const char *init_str);
extern my_bool dynstr_realloc(DYNAMIC_STRING *str, size_t additional_size);
extern my_bool dynstr_trunc(DYNAMIC_STRING *str, size_t n);
extern void dynstr_free(DYNAMIC_STRING *str);
#define alloc_root_inited(A) ((A)->min_malloc != 0)
#define ALLOC_ROOT_MIN_BLOCK_SIZE (MALLOC_OVERHEAD + sizeof(USED_MEM) + 8)
#define clear_alloc_root(A) do { (A)->free= (A)->used= (A)->pre_alloc= 0; (A)->min_malloc=0;} while(0)
extern void init_alloc_root(PSI_memory_key key,
                            MEM_ROOT *mem_root, size_t block_size,
			    size_t pre_alloc_size);
extern void *alloc_root(MEM_ROOT *mem_root, size_t Size);
extern void *multi_alloc_root(MEM_ROOT *mem_root, ...);
extern void free_root(MEM_ROOT *root, myf MyFLAGS);
extern void reset_root_defaults(MEM_ROOT *mem_root, size_t block_size,
                                size_t prealloc_size);
extern char *strdup_root(MEM_ROOT *root,const char *str);
static inline char *safe_strdup_root(MEM_ROOT *root, const char *str)
{
  return str ? strdup_root(root, str) : 0;
}
extern char *strmake_root(MEM_ROOT *root,const char *str,size_t len);
extern void *memdup_root(MEM_ROOT *root,const void *str, size_t len);
extern my_bool my_compress(uchar *, size_t *, size_t *);
extern my_bool my_uncompress(uchar *, size_t , size_t *);
extern uchar *my_compress_alloc(const uchar *packet, size_t *len,
                                size_t *complen);
extern int packfrm(uchar *, size_t, uchar **, size_t *);
extern int unpackfrm(uchar **, size_t *, const uchar *);

extern ha_checksum my_checksum(ha_checksum crc, const uchar *mem,
                               size_t count);

/* Wait a given number of microseconds */
static inline void my_sleep(time_t m_seconds)
{
#if defined(_WIN32)
  Sleep((DWORD)m_seconds/1000+1);      /* Sleep() has millisecond arg */
#else
  struct timeval t;
  t.tv_sec=  m_seconds / 1000000L;
  t.tv_usec= m_seconds % 1000000L;
  select(0,0,0,0,&t); /* sleep */
#endif
}

extern ulong crc32(ulong crc, const uchar *buf, uint len);
extern uint my_set_max_open_files(uint files);
void my_free_open_file_info(void);

extern time_t my_time(myf flags);
extern ulonglong my_getsystime(void);
extern ulonglong my_micro_time();
extern my_bool my_gethwaddr(uchar *to);

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>

#ifndef MAP_NOSYNC
#define MAP_NOSYNC      0
#endif

#ifdef HAVE_MMAP64
#define my_mmap(a,b,c,d,e,f)    mmap64(a,b,c,d,e,f)
#else
#define my_mmap(a,b,c,d,e,f)    mmap(a,b,c,d,e,f)
#endif
#define my_munmap(a,b)          munmap((a),(b))

#else
/* not a complete set of mmap() flags, but only those that nesessary */
#define PROT_READ        1
#define PROT_WRITE       2
#define MAP_NORESERVE    0
#define MAP_SHARED       0x0001
#define MAP_PRIVATE      0x0002
#define MAP_NOSYNC       0x0800
#define MAP_FAILED       ((void *)-1)
#define MS_SYNC          0x0000

void *my_mmap(void *, size_t, int, int, int, my_off_t);
int my_munmap(void *, size_t);
#endif

/* my_getpagesize */
static inline int my_getpagesize()
{
#ifndef _WIN32
  return getpagesize();
#else
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return (int)si.dwPageSize;
#endif
}

int my_msync(int, void *, size_t, int);

/* character sets */
extern void my_charset_loader_init_mysys(MY_CHARSET_LOADER *loader);
extern uint get_charset_number(const char *cs_name, uint cs_flags);
extern uint get_collation_number(const char *name);
extern const char *get_charset_name(uint cs_number);

extern CHARSET_INFO *get_charset(uint cs_number, myf flags);
extern CHARSET_INFO *get_charset_by_name(const char *cs_name, myf flags);
extern CHARSET_INFO *my_collation_get_by_name(MY_CHARSET_LOADER *loader,
                                              const char *name, myf flags);
extern CHARSET_INFO *get_charset_by_csname(const char *cs_name,
					   uint cs_flags, myf my_flags);
extern CHARSET_INFO *my_charset_get_by_name(MY_CHARSET_LOADER *loader,
                                            const char *name,
                                            uint cs_flags, myf my_flags);
extern my_bool resolve_charset(const char *cs_name,
                               const CHARSET_INFO *default_cs,
                               const CHARSET_INFO **cs);
extern my_bool resolve_collation(const char *cl_name,
                                 const CHARSET_INFO *default_cl,
                                 const CHARSET_INFO **cl);
extern void free_charsets(void);
extern char *get_charsets_dir(char *buf);
extern my_bool my_charset_same(const CHARSET_INFO *cs1,
                               const CHARSET_INFO *cs2);
extern my_bool init_compiled_charsets(myf flags);
extern void add_compiled_collation(CHARSET_INFO *cs);
extern size_t escape_string_for_mysql(const CHARSET_INFO *charset_info,
                                      char *to, size_t to_length,
                                      const char *from, size_t length);
#ifdef _WIN32
/* File system character set */
extern CHARSET_INFO *fs_character_set(void);
#endif
extern size_t escape_quotes_for_mysql(CHARSET_INFO *charset_info,
                                      char *to, size_t to_length,
                                      const char *from, size_t length);

#ifdef _WIN32
extern my_bool have_tcpip;		/* Is set if tcpip is used */

/* implemented in my_windac.c */

int my_security_attr_create(SECURITY_ATTRIBUTES **psa, const char **perror,
                            DWORD owner_rights, DWORD everybody_rights);

void my_security_attr_free(SECURITY_ATTRIBUTES *sa);

/* implemented in my_conio.c */
my_bool my_win_is_console(FILE *file);
char *my_win_console_readline(const CHARSET_INFO *cs, char *mbbuf, size_t mbbufsize,
                              size_t *nread);
void my_win_console_write(const CHARSET_INFO *cs, const char *data, size_t datalen);
void my_win_console_fputs(const CHARSET_INFO *cs, const char *data);
void my_win_console_putc(const CHARSET_INFO *cs, int c);
void my_win_console_vfprintf(const CHARSET_INFO *cs, const char *fmt, va_list args);
int my_win_translate_command_line_args(const CHARSET_INFO *cs, int *ac, char ***av);
#endif /* _WIN32 */

#include <mysql/psi/psi.h>

#ifdef HAVE_PSI_INTERFACE
extern MYSQL_PLUGIN_IMPORT struct PSI_bootstrap *PSI_hook;
extern void set_psi_server(PSI *psi);
void my_init_mysys_psi_keys(void);
#endif

struct st_mysql_file;
extern struct st_mysql_file *mysql_stdin;

C_MODE_END
#endif /* _my_sys_h */
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
