//
///*
//   File:    grobnfsymbolpart.h
//   Project: gross
//   Author:  Douwe Vos
//   Date:    May 22, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#ifndef PARSER_BNF_GROBNFSYMBOLPART_H_
//#define PARSER_BNF_GROBNFSYMBOLPART_H_
//
//#include "grobnfstring.h"
//#include "grobnfproductionpart.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_BNF_SYMBOL_PART              (gro_bnf_symbol_part_get_type())
//#define GRO_BNF_SYMBOL_PART(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_symbol_part_get_type(), GroBnfSymbolPart))
//#define GRO_BNF_SYMBOL_PART_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_SYMBOL_PART, GroBnfSymbolPartClass))
//#define GRO_IS_BNF_SYMBOL_PART(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_SYMBOL_PART))
//#define GRO_IS_BNF_SYMBOL_PART_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_SYMBOL_PART))
//#define GRO_BNF_SYMBOL_PART_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_SYMBOL_PART, GroBnfSymbolPartClass))
//
//
//typedef struct _GroBnfSymbolPart               GroBnfSymbolPart;
//typedef struct _GroBnfSymbolPartPrivate        GroBnfSymbolPartPrivate;
//typedef struct _GroBnfSymbolPartClass          GroBnfSymbolPartClass;
//
//
//struct _GroBnfSymbolPart {
//	GroBnfProductionPart parent;
//};
//
//struct _GroBnfSymbolPartClass {
//	GroBnfProductionPartClass parent_class;
//};
//
//
//GType gro_bnf_symbol_part_get_type();
//
//GroBnfSymbolPart *gro_bnf_symbol_part_new(GroBnfString *symbol_name, GroBnfString *label);
//
//GroBnfString *gro_bnf_symbol_part_get_symbol_string(GroBnfSymbolPart *part);
//GroBnfString *gro_bnf_symbol_part_get_label(GroBnfSymbolPart *part);
//
//G_END_DECLS
//
//#endif /* PARSER_BNF_GROBNFSYMBOLPART_H_ */
