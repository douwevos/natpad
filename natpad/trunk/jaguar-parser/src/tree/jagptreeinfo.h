/*
   File:    jagptreeinfo.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 5, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef TREE_JAGPTREEINFO_H_
#define TREE_JAGPTREEINFO_H_

#include "../jagpcursor.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

/** Operator precedences values.
 */
#define jagp_tree_info_notExpression   -1   // not an expression
#define jagp_tree_info_noPrec           0           // no enclosing expression
#define jagp_tree_info_assignPrec       1
#define jagp_tree_info_assignopPrec     2
#define jagp_tree_info_condPrec         3
#define jagp_tree_info_orPrec           4
#define jagp_tree_info_andPrec          5
#define jagp_tree_info_bitorPrec        6
#define jagp_tree_info_bitxorPrec       7
#define jagp_tree_info_bitandPrec       8
#define jagp_tree_info_eqPrec           9
#define jagp_tree_info_ordPrec          10
#define jagp_tree_info_shiftPrec        11
#define jagp_tree_info_addPrec          12
#define jagp_tree_info_mulPrec          13
#define jagp_tree_info_prefixPrec       14
#define jagp_tree_info_postfixPrec      15
#define jagp_tree_info_precCount        16

JagPJCExpression *jagp_tree_info_type_in(JagPJCExpression *tree);
gboolean jagp_tree_info_is_expression_statement(JagPJCExpression *tree);

JagPCursor *jagp_tree_info_get_start_cursor(JagPJCTree *tree);
JagPCursor *jagp_tree_info_get_end_cursor(JagPJCTree *tree);

int jagp_tree_info_op_prec(JagPTag op);

#endif /* TREE_JAGPTREEINFO_H_ */
