/*
   File:    cathashmapwoprivate.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 11, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef CATHASHMAPWOPRIVATE_H_
#define CATHASHMAPWOPRIVATE_H_

#include "cathashmapwo.h"
#include "catwoprivate.h"

struct _CatHashMapWoInfo {
	struct CatWoInfo parent;
	int bucket_count;
	CatArrayWo **buckets;
};


#endif /* CATHASHMAPWOPRIVATE_H_ */
