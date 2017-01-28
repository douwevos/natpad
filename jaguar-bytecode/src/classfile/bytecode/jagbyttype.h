/*
   File:    jagbyttype.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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


#ifndef JAGBYTTYPE_H_
#define JAGBYTTYPE_H_


enum _JagBytType {
	JAG_BYT_TYPE_VOID,
	JAG_BYT_TYPE_INT,
	JAG_BYT_TYPE_LONG,
	JAG_BYT_TYPE_FLOAT,
	JAG_BYT_TYPE_DOUBLE,
	JAG_BYT_TYPE_BYTE,
	JAG_BYT_TYPE_CHAR,
	JAG_BYT_TYPE_SHORT,
	JAG_BYT_TYPE_BOOLEAN,
	JAG_BYT_TYPE_REFERENCE
};

typedef enum _JagBytType JagBytType;


const char *jag_byt_type_as_text(JagBytType byttype);

#endif /* JAGBYTTYPE_H_ */
