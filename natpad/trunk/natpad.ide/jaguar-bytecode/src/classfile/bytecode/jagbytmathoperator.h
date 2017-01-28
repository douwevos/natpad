/*
   File:    jagbytmathoperator.h
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


#ifndef JAGBYTMATHOPERATOR_H_
#define JAGBYTMATHOPERATOR_H_


enum _JagBytMathOperator {
	JAG_BYT_MATH_OPERATOR_POSTFIX_INC,
	JAG_BYT_MATH_OPERATOR_POSTFIX_DEC,
	JAG_BYT_MATH_OPERATOR_PREFIX_INC,
	JAG_BYT_MATH_OPERATOR_PREFIX_DEC,
	JAG_BYT_MATH_OPERATOR_UNARY_MINUS,
	JAG_BYT_MATH_OPERATOR_UNARY_INVERT,
	JAG_BYT_MATH_OPERATOR_UNARY_NOT,

	JAG_BYT_MATH_OPERATOR_MULTIPLY,
	JAG_BYT_MATH_OPERATOR_DIVIDE,
	JAG_BYT_MATH_OPERATOR_ADD,
	JAG_BYT_MATH_OPERATOR_SUB,
	JAG_BYT_MATH_OPERATOR_REMAINDER,
	JAG_BYT_MATH_OPERATOR_SHIFT_LEFT,
	JAG_BYT_MATH_OPERATOR_SHIFT_RIGHT,
	JAG_BYT_MATH_OPERATOR_UNSIGNED_SHIFT_RIGHT,
	JAG_BYT_MATH_OPERATOR_LESS_THAN,
	JAG_BYT_MATH_OPERATOR_GREATER_THAN,
	JAG_BYT_MATH_OPERATOR_LESS_OR_EQUAL,
	JAG_BYT_MATH_OPERATOR_GREATER_OR_EQUAL,
	JAG_BYT_MATH_OPERATOR_INSTANCE_OF,
	JAG_BYT_MATH_OPERATOR_EQUAL,
	JAG_BYT_MATH_OPERATOR_NOT_EQUAL,
	JAG_BYT_MATH_OPERATOR_BITWISE_AND,
	JAG_BYT_MATH_OPERATOR_BITWISE_XOR,
	JAG_BYT_MATH_OPERATOR_BITWISE_OR,
	JAG_BYT_MATH_OPERATOR_LOGICAL_AND,
	JAG_BYT_MATH_OPERATOR_LOGICAL_OR,
};

typedef enum _JagBytMathOperator JagBytMathOperator;

const char *jag_byt_math_operator_to_text(JagBytMathOperator math_operator);


#endif /* JAGBYTMATHOPERATOR_H_ */
