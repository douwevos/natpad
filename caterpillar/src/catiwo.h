/*
   File:    catiwo.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    May 9, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef CATIWO_H_
#define CATIWO_H_


typedef enum _CatWoState CatWoState;

enum _CatWoState {
	/* The object is Read-Only*/
	CAT_WO_READONLY,

	/* The object is writable but wasn't changed yet */
	CAT_WO_UNMODIFIED,

	/* the object is writable and was changed */
	CAT_WO_MODIFIED
};

#endif /* CATIWO_H_ */
