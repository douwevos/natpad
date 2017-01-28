/*
   File:    natdate.h
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


#ifndef NATDATE_H_
#define NATDATE_H_

#include <glib-object.h>
#include <stdint.h>

int64_t cat_date_from_timeval(GTimeVal *val);

int64_t cat_date_current_time();

int64_t cat_monotonic_time();

#endif /* NATDATE_H_ */
