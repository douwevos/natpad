//
///*
//   File:    grobnfstacktype.h
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
//#ifndef PARSER_BNF_GROBNFSTACKTYPE_H_
//#define PARSER_BNF_GROBNFSTACKTYPE_H_
//
//#include "grobnfstring.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_BNF_STACK_TYPE              (gro_bnf_stack_type_get_type())
//#define GRO_BNF_STACK_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_stack_type_get_type(), GroBnfStackType))
//#define GRO_BNF_STACK_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_STACK_TYPE, GroBnfStackTypeClass))
//#define GRO_IS_BNF_STACK_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_STACK_TYPE))
//#define GRO_IS_BNF_STACK_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_STACK_TYPE))
//#define GRO_BNF_STACK_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_STACK_TYPE, GroBnfStackTypeClass))
//
//
//typedef struct _GroBnfStackType               GroBnfStackType;
//typedef struct _GroBnfStackTypePrivate        GroBnfStackTypePrivate;
//typedef struct _GroBnfStackTypeClass          GroBnfStackTypeClass;
//
//
//struct _GroBnfStackType {
//	GObject parent;
//};
//
//struct _GroBnfStackTypeClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_bnf_stack_type_get_type();
//
//GroBnfStackType *gro_bnf_stack_type_new(GroBnfString *as_text);
//
//CatStringWo *gro_bnf_stack_type_get_as_text(GroBnfStackType *stack_type);
//
//G_END_DECLS
//
//#endif /* PARSER_BNF_GROBNFSTACKTYPE_H_ */
