/*
   File:    jagbytmathoperator.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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

#include "jagbytmathoperator.h"

const char *jag_byt_math_operator_to_text(JagBytMathOperator math_operator) {
	switch(math_operator) {
		case JAG_BYT_MATH_OPERATOR_POSTFIX_INC : return "++";
		case JAG_BYT_MATH_OPERATOR_POSTFIX_DEC : return "--";
		case JAG_BYT_MATH_OPERATOR_PREFIX_INC : return "++";
		case JAG_BYT_MATH_OPERATOR_PREFIX_DEC : return "--";
		case JAG_BYT_MATH_OPERATOR_UNARY_MINUS : return "-";
		case JAG_BYT_MATH_OPERATOR_UNARY_INVERT : return "~";
		case JAG_BYT_MATH_OPERATOR_UNARY_NOT : return "!";

		case JAG_BYT_MATH_OPERATOR_MULTIPLY : return "*";
		case JAG_BYT_MATH_OPERATOR_DIVIDE : return "/";
		case JAG_BYT_MATH_OPERATOR_ADD : return "+";
		case JAG_BYT_MATH_OPERATOR_SUB : return "-";
		case JAG_BYT_MATH_OPERATOR_REMAINDER : return "%";
		case JAG_BYT_MATH_OPERATOR_SHIFT_LEFT : return "<<";
		case JAG_BYT_MATH_OPERATOR_SHIFT_RIGHT : return ">>";
		case JAG_BYT_MATH_OPERATOR_UNSIGNED_SHIFT_RIGHT : return ">>>";
		case JAG_BYT_MATH_OPERATOR_LESS_THAN : return "<";
		case JAG_BYT_MATH_OPERATOR_GREATER_THAN : return ">";
		case JAG_BYT_MATH_OPERATOR_LESS_OR_EQUAL : return "=<";
		case JAG_BYT_MATH_OPERATOR_GREATER_OR_EQUAL : return "=>";
		case JAG_BYT_MATH_OPERATOR_INSTANCE_OF : return "instanceof";
		case JAG_BYT_MATH_OPERATOR_EQUAL : return "==";
		case JAG_BYT_MATH_OPERATOR_NOT_EQUAL : return "!=";
		case JAG_BYT_MATH_OPERATOR_BITWISE_AND : return "&";
		case JAG_BYT_MATH_OPERATOR_BITWISE_XOR : return "^";
		case JAG_BYT_MATH_OPERATOR_BITWISE_OR : return "|";
		case JAG_BYT_MATH_OPERATOR_LOGICAL_AND : return "&&";
		case JAG_BYT_MATH_OPERATOR_LOGICAL_OR : return "||";
	}
	return "?";
}

