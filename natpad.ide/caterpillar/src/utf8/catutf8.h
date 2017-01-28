/*
   File:    catutf8.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 26, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef CATUTF8_H_
#define CATUTF8_H_

#include "../woo/catstringwo.h"
#include <glib-object.h>

void cat_utf8_flip_case(CatStringWo *text);

#endif /* CATUTF8_H_ */
