///*
//   File:    cusparsercontextext.h
//   Project: cuscus
//   Author:  Douwe Vos
//   Date:    May 16, 2016
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
//#ifndef PARSER_CUSPARSERCONTEXTEXT_H_
//#define PARSER_CUSPARSERCONTEXTEXT_H_
//
//#include "generated/runtime/cusparsercontext.h"
//#include "generated/runtime/cusiscanner.h"
//#include "../model/custerminal.h"
//#include "../model/cussymbol.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define CUS_TYPE_PARSER_CONTEXT_EXT              (cus_parser_context_ext_get_type())
//#define CUS_PARSER_CONTEXT_EXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_parser_context_ext_get_type(), CusParserContextExt))
//#define CUS_PARSER_CONTEXT_EXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_PARSER_CONTEXT_EXT, CusParserContextExtClass))
//#define CUS_IS_PARSER_CONTEXT_EXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_PARSER_CONTEXT_EXT))
//#define CUS_IS_PARSER_CONTEXT_EXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_PARSER_CONTEXT_EXT))
//#define CUS_PARSER_CONTEXT_EXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_PARSER_CONTEXT_EXT, CusParserContextExtClass))
//
//
//typedef struct _CusParserContextExt               CusParserContextExt;
//typedef struct _CusParserContextExtPrivate        CusParserContextExtPrivate;
//typedef struct _CusParserContextExtClass          CusParserContextExtClass;
//
//
//struct _CusParserContextExt {
//	CusParserContext parent;
//};
//
//struct _CusParserContextExtClass {
//	CusParserContextClass parent_class;
//};
//
//
//GType cus_parser_context_ext_get_type();
//
//CusParserContextExt *cus_parser_context_ext_new(CusIScanner *scanner);
//
//CusTerminal *cus_parser_context_ext_create_term(CusParserContext *parser_context, CatStringWo *name);
//
//void cus_parser_context_ext_set_stack_type(CusParserContext *parser_context, CatStringWo *stack_type, CatArrayWo *symbol_list);
//
//CusSymbol *cus_parser_context_ext_find_symbol(CusParserContext *parser_context, CatStringWo *symname);
//
//G_END_DECLS
//
//#endif /* PARSER_CUSPARSERCONTEXTEXT_H_ */
