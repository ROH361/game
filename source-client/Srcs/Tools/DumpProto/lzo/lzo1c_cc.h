/* lzo1c_cc.h -- definitions for the the LZO1C compression driver

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1996-2002 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
 */


/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the library and is subject
   to change.
 */


#ifndef __LZO1C_CC_H
#define __LZO1C_CC_H


/***********************************************************************
//
************************************************************************/

extern const lzo_compress_t _lzo1c_1_compress_func;
extern const lzo_compress_t _lzo1c_2_compress_func;
extern const lzo_compress_t _lzo1c_3_compress_func;
extern const lzo_compress_t _lzo1c_4_compress_func;
extern const lzo_compress_t _lzo1c_5_compress_func;
extern const lzo_compress_t _lzo1c_6_compress_func;
extern const lzo_compress_t _lzo1c_7_compress_func;
extern const lzo_compress_t _lzo1c_8_compress_func;
extern const lzo_compress_t _lzo1c_9_compress_func;

extern const lzo_compress_t _lzo1c_99_compress_func;


/***********************************************************************
//
************************************************************************/

LZO_EXTERN(lzo_byte *)
_lzo1c_store_run ( lzo_byte * const oo, const lzo_byte * const ii,
				   lzo_uint r_len);

#define STORE_RUN	_lzo1c_store_run


lzo_compress_t _lzo1c_get_compress_func(int clevel);

int _lzo1c_do_compress   ( const lzo_byte *in,  lzo_uint  in_len,
								 lzo_byte *out, lzo_uintp out_len,
								 lzo_voidp wrkmem,
								 lzo_compress_t func );


#endif /* already included */

/*
vi:ts=4:et
*/
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
