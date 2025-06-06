/* stats1b.h -- statistics for the the LZO library

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


#ifndef __LZO_STATS1B_H
#define __LZO_STATS1B_H

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
// Collect statistical information when compressing.
// Useful for finetuning the compression algorithm.
// Examine the symbol 'lzo1b_stats' with a debugger.
************************************************************************/

#if defined(LZO_COLLECT_STATS)
#  define LZO_STATS(expr)	expr
#else
#  define LZO_STATS(expr)	((void) 0)
#endif


#if defined(LZO_COLLECT_STATS)

typedef struct
{
/* algorithm configuration */
	unsigned r_bits;
	unsigned m3o_bits;
	unsigned dd_bits;
	unsigned clevel;

/* internal configuration */
	unsigned d_bits;
	long min_lookahead;
	long max_lookbehind;
	const char *compress_id;

/* counts */
	long lit_runs;
	long r0short_runs;
	long r0fast_runs;
	long r0long_runs;
	long m1_matches;
	long m2_matches;
	long m3_matches;
	long m4_matches;
	long r1_matches;

/* */
	long lit_run[R0MIN];
	long m2_match[M2_MAX_LEN + 1];
	long m3_match[M3_MAX_LEN + 1];
#if (M3O_BITS < 8)
	long lit_runs_after_m3_match;
	long lit_run_after_m3_match[LZO_SIZE(8-M3O_BITS)];
#endif

/* */
	long matches;
	long match_bytes;
	long literals;
	long literal_overhead;
	long literal_bytes;
	float literal_overhead_percent;

/* */
	long unused_dict_entries;
	float unused_dict_entries_percent;

/* */
	long in_len;
	long out_len;
}
lzo1b_stats_t;


void _lzo1b_stats_init(lzo1b_stats_t *lzo_stats);
void _lzo1b_stats_calc(lzo1b_stats_t *lzo_stats);

extern lzo1b_stats_t * const lzo1b_stats;

#define lzo_stats_t		lzo1b_stats_t
#define lzo_stats		lzo1b_stats

#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* already included */

/*
vi:ts=4:et
*/
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
