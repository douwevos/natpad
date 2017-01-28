/*
   File:    grobnfrhssymbolpart.h
   Project: gross
   Author:  Douwe Vos
   Date:    Nov 17, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef PARSER_FILE_GROBNFRHSSYMBOLPART_H_
#define PARSER_FILE_GROBNFRHSSYMBOLPART_H_

#include "grobnfstring.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_RHS_SYMBOL_PART              (gro_bnf_rhs_symbol_part_get_type())
#define GRO_BNF_RHS_SYMBOL_PART(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_symbol_part_get_type(), GroBnfRhsSymbolPart))
#define GRO_BNF_RHS_SYMBOL_PART_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS_SYMBOL_PART, GroBnfRhsSymbolPartClass))
#define GRO_IS_BNF_RHS_SYMBOL_PART(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS_SYMBOL_PART))
#define GRO_IS_BNF_RHS_SYMBOL_PART_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS_SYMBOL_PART))
#define GRO_BNF_RHS_SYMBOL_PART_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS_SYMBOL_PART, GroBnfRhsSymbolPartClass))


typedef struct _GroBnfRhsSymbolPart               GroBnfRhsSymbolPart;
typedef struct _GroBnfRhsSymbolPartPrivate        GroBnfRhsSymbolPartPrivate;
typedef struct _GroBnfRhsSymbolPartClass          GroBnfRhsSymbolPartClass;


struct _GroBnfRhsSymbolPart {
	GObject parent;
};

struct _GroBnfRhsSymbolPartClass {
	GObjectClass parent_class;
};


GType gro_bnf_rhs_symbol_part_get_type();

GroBnfRhsSymbolPart *gro_bnf_rhs_symbol_part_new(GroBnfString *sym_id, GroBnfString *label);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFRHSSYMBOLPART_H_ */
