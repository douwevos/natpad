///*
//   File:    jgiparsercontext.h
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Jun 10, 2014
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2014 Douwe Vos.
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
//#ifndef JGIPARSERCONTEXT_H_
//#define JGIPARSERCONTEXT_H_
//
//#include "../../parser/generated/runtime/jagparsercontext.h"
//#include "../../parser/generated/runtime/jagiscanner.h"
//#include <caterpillar.h>
//#include <moose.h>
//#include <viper.h>
//
//G_BEGIN_DECLS
//
//#define JGI_TYPE_PARSER_CONTEXT              (jgi_parser_context_get_type())
//#define JGI_PARSER_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jgi_parser_context_get_type(), JgiParserContext))
//#define JGI_PARSER_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JGI_TYPE_PARSER_CONTEXT, JgiParserContextClass))
//#define JGI_IS_PARSER_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JGI_TYPE_PARSER_CONTEXT))
//#define JGI_IS_PARSER_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JGI_TYPE_PARSER_CONTEXT))
//#define JGI_PARSER_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JGI_TYPE_PARSER_CONTEXT, JgiParserContextClass))
//#define JGI_PARSER_CONTEXT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JGI_TYPE_PARSER_CONTEXT, JgiParserContextPrivate))
//
//
//typedef struct _JgiParserContext               JgiParserContext;
//typedef struct _JgiParserContextPrivate        JgiParserContextPrivate;
//typedef struct _JgiParserContextClass          JgiParserContextClass;
//
//
//struct _JgiParserContext {
//	JagParserContext parent;
//	JgiParserContextPrivate *priv;
//};
//
//struct _JgiParserContextClass {
//	JagParserContextClass parent_class;
//};
//
//
//GType jgi_parser_context_get_type();
//
//JgiParserContext *jgi_parser_context_new(JagIScanner *scanner, VipISequence *moo_sequence);
//
//MooNodeWo *jgi_parser_context_get_root_node(JgiParserContext *parser_context);
//
//void jgi_parser_context_enlist_tokens(JgiParserContext *parser_context, int row, int column);
//int jgi_parser_context_get_focus_index(JgiParserContext *parser_context);
//CatArrayWo *jgi_parser_context_get_token_array(JgiParserContext *parser_context);
//
//
//void jgi_parser_context_dump_stack_with_focus(JgiParserContext *parser_context);
//
//
//G_END_DECLS
//
//#endif /* JGIPARSERCONTEXT_H_ */
