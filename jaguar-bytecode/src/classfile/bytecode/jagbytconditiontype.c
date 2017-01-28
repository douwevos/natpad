/*
   File:    jagbytconditiontype.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 3, 2012
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

#include "jagbytconditiontype.h"

JagBytConditionType jag_byt_condition_type_opposite(JagBytConditionType condition_type) {
	switch(condition_type) {
		case JAG_BYT_CONDITION_TYPE_EQUAL : return JAG_BYT_CONDITION_TYPE_NOT_EQUAL;
		case JAG_BYT_CONDITION_TYPE_NOT_EQUAL : return JAG_BYT_CONDITION_TYPE_EQUAL;
		case JAG_BYT_CONDITION_TYPE_LESS_OR_EQUAL : return JAG_BYT_CONDITION_TYPE_GREATER_THEN;
		case JAG_BYT_CONDITION_TYPE_LESS_THEN : return JAG_BYT_CONDITION_TYPE_GREATER_OR_EQUAL;
		case JAG_BYT_CONDITION_TYPE_GREATER_THEN : return JAG_BYT_CONDITION_TYPE_LESS_OR_EQUAL;
		case JAG_BYT_CONDITION_TYPE_GREATER_OR_EQUAL : return JAG_BYT_CONDITION_TYPE_LESS_THEN;
	}
	return JAG_BYT_CONDITION_TYPE_EQUAL;
}


const char *jag_byt_condition_type_text(JagBytConditionType condition_type) {
	switch(condition_type) {
		case JAG_BYT_CONDITION_TYPE_EQUAL : return "==";
		case JAG_BYT_CONDITION_TYPE_NOT_EQUAL : return "!=";
		case JAG_BYT_CONDITION_TYPE_LESS_OR_EQUAL : return "<=";
		case JAG_BYT_CONDITION_TYPE_LESS_THEN : return "<";
		case JAG_BYT_CONDITION_TYPE_GREATER_THEN : return ">";
		case JAG_BYT_CONDITION_TYPE_GREATER_OR_EQUAL : return ">=";
	}
	return "<?condition-type?>";
}
