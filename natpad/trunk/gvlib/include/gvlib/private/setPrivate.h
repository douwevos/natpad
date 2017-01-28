/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2010 Gerard Visser.

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

#ifndef ___SET_PRIVATE_INCLUDED
#define ___SET_PRIVATE_INCLUDED

#include "../defs.h"
#undef readonly
#define readonly
#include "../sequences/Set.h"
#undef readonly
#define readonly const

#include "../Object.h"
#include "../TypeData.h"
#include "listPrivate.h"

#define SET_TYPE_DATA ptrof(setTypeDataStruct)

extern const struct typeDataStruct setTypeDataStruct;

    void Set_append(Set this, Object val);
    void Set_appendNew(Set this, Object val);
    void Set_appendRange(Set this, const Object* sPtr, int sOff, int len);
    void Set_insert(Set this, int dOff, Object val);
    void Set_insertNew(Set this, int dOff, Object val);
TypeData Set_rtType(void);

#endif
