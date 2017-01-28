/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef ___CHE_CODES_INCLUDED
#define ___CHE_CODES_INCLUDED

#include <gvlcodeparsing/codes.h>

#define CHE_ERROR   3
#define CHE_WARNING 2

#define CHE_CODE_ERROR               0x10000 /* For extra errors in highlight mode. */
#define CHE_CODE_MASK                0xFFFF

#define CHE_CODE_ADJECTIVE           11
#define CHE_CODE_COMMENT             4
#define CHE_CODE_DECLAR_FUNC         9
#define CHE_CODE_DECLAR_MACRO        CHE_CODE_DECLAR_VAR
#define CHE_CODE_DECLAR_MACRO_PARAMS CHE_CODE_DECLAR_FUNC
#define CHE_CODE_DECLAR_TYPE         14/**/
#define CHE_CODE_DECLAR_VAR          8
#define CHE_CODE_DIRECTIVE           5
#define CHE_CODE_FALSE_NULL_TRUE     3
#define CHE_CODE_HALFTYPE_IDENTIFIER 12
#define CHE_CODE_HALFTYPE_PREFIX     13
#define CHE_CODE_KEY_WORD            10
#define CHE_CODE_NONE                2
#define CHE_CODE_NUMBER              1
#define CHE_CODE_STATEMENT           CHE_CODE_KEY_WORD
#define CHE_CODE_STRING              6
#define CHE_CODE_TYPE                7

/* Extension of <gvlcodeparsing/codes.h>
   It is uncertain whether any of these will be added to codes.h */

#define GVLCP_CODE_RESTRICT             (GVLCP_CODE_ADJECTIVE | GVLCP_AREA_5_8)

#define GVLCP_CODE_FALSE_NULL_TRUE      (GVLCP_CODE_BUILT_IN_MACRO | GVLCP_AREA_5_3)

#define GVLCP_CODE_GDOUBLE              (GVLCP_CODE_TYPE | GVLCP_CODE_FLOATING_POINT | GVLCP_CODE_SIZE_64)
#define GVLCP_CODE_GINT16               (GVLCP_CODE_TYPE | GVLCP_CODE_INTEGER | GVLCP_CODE_SIZE_16)
#define GVLCP_CODE_GINT32               (GVLCP_CODE_TYPE | GVLCP_CODE_INTEGER | GVLCP_CODE_SIZE_32)
#define GVLCP_CODE_GINT64               (GVLCP_CODE_TYPE | GVLCP_CODE_INTEGER | GVLCP_CODE_SIZE_64)

#define GVLCP_CODE_TYPE_FILE            (GVLCP_CODE_TYPE | GVLCP_AREA_5_1)
#define GVLCP_CODE_USER_TYPE            (GVLCP_CODE_TYPE | GVLCP_AREA_5_2)
#define GVLCP_CODE_VA_LIST              (GVLCP_CODE_TYPE | GVLCP_AREA_5_3)

#endif
