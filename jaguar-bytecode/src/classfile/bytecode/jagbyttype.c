/*
   File:    jagbyttype.c
   Project: jaguar-bytecode
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

#include "jagbyttype.h"

const char *jag_byt_type_as_text(JagBytType byttype) {
	switch(byttype) {
		case JAG_BYT_TYPE_BOOLEAN : return "boolean";
		case JAG_BYT_TYPE_BYTE : return "byte";
		case JAG_BYT_TYPE_SHORT : return "short";
		case JAG_BYT_TYPE_INT : return "int";
		case JAG_BYT_TYPE_CHAR : return "char";
		case JAG_BYT_TYPE_LONG : return "long";
		case JAG_BYT_TYPE_FLOAT : return "float";
		case JAG_BYT_TYPE_DOUBLE : return "double";
		case JAG_BYT_TYPE_VOID : return "void";
		case JAG_BYT_TYPE_REFERENCE : return "<reference>";
	}
	return "<invalid>";
}

