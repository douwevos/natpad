
/*
   File:    groglibctypes.h
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 18, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include <glib-object.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef GROGLIBCTYPES_H_
#define GROGLIBCTYPES_H_

void *gro_ref_ptr(void *v);

#define gro_unref_ptr(obj) { const void *l_obj = obj; obj=NULL; gro_unref_ptr_intern((void *) l_obj); }

void gro_unref_ptr_intern(void *v);

void gro_free_ptr(void *v);

#define GROSS_DEBUG

#ifdef GROSS_DEBUG
#define gro_log(format, args...) \
	printf(format "\n", ##args);
#else
#define gro_log(format, args...) \
//		printf(format "\n", ##args);
#endif


#endif /* GROGLIBCTYPES_H_ */
