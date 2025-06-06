/* Copyright (c) 2013, Oracle and/or its affiliates. All rights reserved.

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

#ifndef MYSQL_SP_H
#define MYSQL_SP_H

/**
  @file mysql/psi/mysql_sp.h
  Instrumentation helpers for stored programs.
*/

#include "mysql/psi/psi.h"

#ifndef PSI_SP_CALL
#define PSI_SP_CALL(M) PSI_DYNAMIC_CALL(M)                               
#endif    

#ifdef HAVE_PSI_SP_INTERFACE
  #define MYSQL_START_SP(STATE, SP_SHARE) \
    inline_mysql_start_sp(STATE, SP_SHARE)
#else
  #define MYSQL_START_SP(STATE, SP_SHARE) \
    NULL
#endif


#ifdef HAVE_PSI_SP_INTERFACE
  #define MYSQL_END_SP(LOCKER) \
    inline_mysql_end_sp(LOCKER)
#else
  #define MYSQL_END_SP(LOCKER) \
    do {} while (0)
#endif

#ifdef HAVE_PSI_SP_INTERFACE
  #define MYSQL_DROP_SP(OT, SN, SNL, ON, ONL) \
    inline_mysql_drop_sp(OT, SN, SNL, ON, ONL)
#else
  #define MYSQL_DROP_SP(OT, SN, SNL, ON, ONL) \
    do {} while (0)
#endif

#ifdef HAVE_PSI_SP_INTERFACE
  #define MYSQL_GET_SP_SHARE(OT, SN, SNL, ON, ONL) \
    inline_mysql_get_sp_share(OT, SN, SNL, ON, ONL)
#else
  #define MYSQL_GET_SP_SHARE(OT, SN, SNL, ON, ONL) \
    NULL
#endif

#ifdef HAVE_PSI_SP_INTERFACE
static inline struct PSI_sp_locker*
inline_mysql_start_sp(PSI_sp_locker_state *state, PSI_sp_share *sp_share)
{
  return PSI_SP_CALL(start_sp)(state, sp_share);
}

static inline void inline_mysql_end_sp(PSI_sp_locker *locker)
{
  if (likely(locker != NULL))
    PSI_SP_CALL(end_sp)(locker);
}

static inline void 
inline_mysql_drop_sp(uint sp_type,
                     const char* schema_name, uint shcema_name_length,
                     const char* object_name, uint object_name_length)
{
  PSI_SP_CALL(drop_sp)(sp_type,
                       schema_name, shcema_name_length,
                       object_name, object_name_length);
}

static inline PSI_sp_share*
inline_mysql_get_sp_share(uint sp_type,
                          const char* schema_name, uint shcema_name_length,
                          const char* object_name, uint object_name_length)
{
  return PSI_SP_CALL(get_sp_share)(sp_type,
                                   schema_name, shcema_name_length,
                                   object_name, object_name_length);
}
#endif

#endif
//martysama0134's 2e58d0b8baeb072acdf3afc4a5d1999f
