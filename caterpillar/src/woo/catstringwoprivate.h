/*
   File:    catstringwoprivate.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 9, 2014
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

#ifndef CATSTRINGWOPRIVATE_H_
#define CATSTRINGWOPRIVATE_H_

#include "catwoprivate.h"
#include "catstringwo.h"

struct CatStringWoInfo {
	struct CatWoInfo parent;
	gchar *data;
	int size;
};

void cat_string_wo_construct(CatStringWo *string, gboolean editable, gchar *data, int data_size, int length);

#endif /* CATSTRINGWOPRIVATE_H_ */
