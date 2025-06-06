/* config1f.h -- configuration for the LZO1F algorithm

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


#ifndef __LZO_CONFIG1F_H
#define __LZO_CONFIG1F_H

#include <lzo1f.h>
#include "lzo_conf.h"
#include "lzo_util.h"


/***********************************************************************
//
************************************************************************/

#define LZO_EOF_CODE
#undef LZO_DETERMINISTIC

#define M2_MAX_OFFSET	0x0800
#define M3_MAX_OFFSET	0x3fff

#define M2_MIN_LEN		3
#define M2_MAX_LEN		8
#define M3_MIN_LEN		3
#define M3_MAX_LEN		33

#define M3_MARKER		224


/***********************************************************************
//
************************************************************************/

#ifndef MIN_LOOKAHEAD
#define MIN_LOOKAHEAD		(M2_MAX_LEN + 1)
#endif

#if defined(LZO_NEED_DICT_H)

#ifndef LZO_HASH
#define LZO_HASH			LZO_HASH_LZO_INCREMENTAL_A
#endif
#define DL_MIN_LEN			M2_MIN_LEN
#include "lzo_dict.h"

#endif



#endif /* already included */

/*
vi:ts=4:et
*/
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
