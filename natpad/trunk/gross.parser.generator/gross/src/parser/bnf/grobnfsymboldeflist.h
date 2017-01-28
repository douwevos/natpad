//
///*
//   File:    grobnfsymboldeflist.h
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
//#ifndef PARSER_BNF_GROBNFSYMBOLDEFLIST_H_
//#define PARSER_BNF_GROBNFSYMBOLDEFLIST_H_
//
//#include "grobnfstacktype.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_BNF_SYMBOL_DEF_LIST              (gro_bnf_symbol_def_list_get_type())
//#define GRO_BNF_SYMBOL_DEF_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_symbol_def_list_get_type(), GroBnfSymbolDefList))
//#define GRO_BNF_SYMBOL_DEF_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_SYMBOL_DEF_LIST, GroBnfSymbolDefListClass))
//#define GRO_IS_BNF_SYMBOL_DEF_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_SYMBOL_DEF_LIST))
//#define GRO_IS_BNF_SYMBOL_DEF_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_SYMBOL_DEF_LIST))
//#define GRO_BNF_SYMBOL_DEF_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_SYMBOL_DEF_LIST, GroBnfSymbolDefListClass))
//
//
//typedef struct _GroBnfSymbolDefList               GroBnfSymbolDefList;
//typedef struct _GroBnfSymbolDefListPrivate        GroBnfSymbolDefListPrivate;
//typedef struct _GroBnfSymbolDefListClass          GroBnfSymbolDefListClass;
//
//
//struct _GroBnfSymbolDefList {
//	GObject parent;
//};
//
//struct _GroBnfSymbolDefListClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_bnf_symbol_def_list_get_type();
//
//GroBnfSymbolDefList *gro_bnf_symbol_def_list_new(CatArrayWo *name_list, GroBnfStackType *stack_type, gboolean is_terminal_list);
//
//
//GroBnfStackType *gro_bnf_symbol_def_list_get_stack_type(GroBnfSymbolDefList *list);
//CatArrayWo *gro_bnf_symbol_def_list_get_name_list(GroBnfSymbolDefList *list);
//gboolean gro_bnf_symbol_def_list_is_terminal_list(GroBnfSymbolDefList *list);
//
//G_END_DECLS
//
//#endif /* PARSER_BNF_GROBNFSYMBOLDEFLIST_H_ */
