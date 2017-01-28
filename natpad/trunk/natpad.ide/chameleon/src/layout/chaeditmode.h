/*
   File:    chaeditmode.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 14, 2015
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

#ifndef LAYOUT_CHAEDITMODE_H_
#define LAYOUT_CHAEDITMODE_H_


enum _ChaEditMode {
	CHA_EDIT_MODE_OVERWRITE,
	CHA_EDIT_MODE_INSERT,
	CHA_EDIT_MODE_INSERT_CONTEXT,
};

typedef enum _ChaEditMode ChaEditMode;


#endif /* LAYOUT_CHAEDITMODE_H_ */
