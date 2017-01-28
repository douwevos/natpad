/*
   File:    natdate.c
   Project: Natpad
   Author:  Douwe Vos
   Date:    Feb 11, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

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

#include "natdate.h"
#include <sys/time.h>



int64_t cat_date_from_timeval(GTimeVal *val) {
	return ((int64_t) val->tv_sec)*1000l+val->tv_usec/1000l;
}


int64_t cat_date_current_time() {
	GTimeVal val;
	g_get_current_time(&val);
	return cat_date_from_timeval(&val);
}

int64_t cat_monotonic_time() {
	return g_get_monotonic_time();
}

