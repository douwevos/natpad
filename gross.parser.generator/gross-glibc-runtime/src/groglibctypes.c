
/*
   File:    groglibctypes.c
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

#include "groglibctypes.h"

void *gro_ref_ptr(void *v) {
	if (v) {
		g_object_ref(v);
	}
	return v;
}

void gro_unref_ptr_intern(void *v) {
	if (v) {
		g_object_unref(v);
	}
}


void gro_free_ptr(void *v) {
	void *k = v;
	if (v) {
		v = NULL;
		g_free(k);
	}
}
