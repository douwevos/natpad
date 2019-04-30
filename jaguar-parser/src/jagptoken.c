/*
   File:    jagptoken.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 1, 2017
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

#include "jagptoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPToken"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPToken, jagp_token, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_token_class_init(JagPTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_token_init(JagPToken *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPToken *instance = JAGP_TOKEN(object);
	cat_unref_ptr(instance->value);
	cat_unref_ptr(instance->cur_start);
	cat_unref_ptr(instance->cur_end);
	G_OBJECT_CLASS(jagp_token_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_token_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPToken *jagp_token_new() {
	JagPToken *result = g_object_new(JAGP_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	cat_ref_ptr(result);
	result->value = NULL;
	result->cur_start = NULL;
	result->cur_end = NULL;
	return result;
}


GObject *jagp_token_get_value(JagPToken *token) {
	return token->value;
}

JagPComment *jagp_token_comment(JagPToken *token, JagPCommentStyle comment_style) {
	return jagp_comment_new();	// TODO
}

const char *l_descr(JagPTokenKind kind) {
	switch(kind) {
		case JAGP_KIND_EOF : return "eof";
		case JAGP_KIND_ERROR : return "error";
		case JAGP_KIND_IDENTIFIER : return "identifier";
		case JAGP_KIND_ABSTRACT : return "abstract";
		case JAGP_KIND_ASSERT : return "assert";
		case JAGP_KIND_BOOLEAN : return "boolean";
		case JAGP_KIND_BREAK : return "break";
		case JAGP_KIND_BYTE : return "byte";
		case JAGP_KIND_CASE : return "case";
		case JAGP_KIND_CATCH : return "catch";
		case JAGP_KIND_CHAR : return "char";
		case JAGP_KIND_CLASS : return "class";
		case JAGP_KIND_CONST : return "const";
		case JAGP_KIND_CONTINUE : return "continue";
		case JAGP_KIND_DEFAULT : return "default";
		case JAGP_KIND_DO : return "do";
		case JAGP_KIND_DOUBLE : return "double";
		case JAGP_KIND_ELSE : return "else";
		case JAGP_KIND_ENUM : return "enum";
		case JAGP_KIND_EXTENDS : return "extends";
		case JAGP_KIND_FINAL : return "final";
		case JAGP_KIND_FINALLY : return "finally";
		case JAGP_KIND_FLOAT : return "float";
		case JAGP_KIND_FOR : return "for";
		case JAGP_KIND_GOTO : return "goto";
		case JAGP_KIND_IF : return "if";
		case JAGP_KIND_IMPLEMENTS : return "implements";
		case JAGP_KIND_IMPORT : return "import";
		case JAGP_KIND_INSTANCEOF : return "instanceof";
		case JAGP_KIND_INT : return "int";
		case JAGP_KIND_INTERFACE : return "interface";
		case JAGP_KIND_LONG : return "long";
		case JAGP_KIND_NATIVE : return "native";
		case JAGP_KIND_NEW : return "new";
		case JAGP_KIND_PACKAGE : return "package";
		case JAGP_KIND_PRIVATE : return "private";
		case JAGP_KIND_PROTECTED : return "protected";
		case JAGP_KIND_PUBLIC : return "public";
		case JAGP_KIND_RETURN : return "return";
		case JAGP_KIND_SHORT : return "short";
		case JAGP_KIND_STATIC : return "static";
		case JAGP_KIND_STRICTFP : return "strictfp";
		case JAGP_KIND_SUPER : return "super";
		case JAGP_KIND_SWITCH : return "switch";
		case JAGP_KIND_SYNCHRONIZED : return "synchronized";
		case JAGP_KIND_THIS : return "this";
		case JAGP_KIND_THROW : return "throw";
		case JAGP_KIND_THROWS : return "throws";
		case JAGP_KIND_TRANSIENT : return "transient";
		case JAGP_KIND_TRY : return "try";
		case JAGP_KIND_VOID : return "void";
		case JAGP_KIND_VOLATILE : return "volatile";
		case JAGP_KIND_WHILE : return "while";
		case JAGP_KIND_INTLITERAL : return "intliteral";
		case JAGP_KIND_LONGLITERAL : return "longliteral";
		case JAGP_KIND_FLOATLITERAL : return "floatliteral";
		case JAGP_KIND_DOUBLELITERAL : return "doubleliteral";
		case JAGP_KIND_CHARLITERAL : return "charliteral";
		case JAGP_KIND_STRINGLITERAL : return "stringliteral";
		case JAGP_KIND_TRUE : return "true";
		case JAGP_KIND_FALSE : return "false";
		case JAGP_KIND_NULL : return "null";
		case JAGP_KIND_UNDERSCORE : return "underscore";
		case JAGP_KIND_ARROW : return "arrow";
		case JAGP_KIND_COLCOL : return "colcol";
		case JAGP_KIND_LPAREN : return "lparen";
		case JAGP_KIND_RPAREN : return "rparen";
		case JAGP_KIND_LBRACE : return "lbrace";
		case JAGP_KIND_RBRACE : return "rbrace";
		case JAGP_KIND_LBRACKET : return "lbracket";
		case JAGP_KIND_RBRACKET : return "rbracket";
		case JAGP_KIND_SEMI : return "semi";
		case JAGP_KIND_COMMA : return "comma";
		case JAGP_KIND_DOT : return "dot";
		case JAGP_KIND_ELLIPSIS : return "ellipsis";
		case JAGP_KIND_EQ : return "eq";
		case JAGP_KIND_GT : return "gt";
		case JAGP_KIND_LT : return "lt";
		case JAGP_KIND_BANG : return "bang";
		case JAGP_KIND_TILDE : return "tilde";
		case JAGP_KIND_QUES : return "ques";
		case JAGP_KIND_COLON : return "colon";
		case JAGP_KIND_EQEQ : return "eqeq";
		case JAGP_KIND_LTEQ : return "lteq";
		case JAGP_KIND_GTEQ : return "gteq";
		case JAGP_KIND_BANGEQ : return "bangeq";
		case JAGP_KIND_AMPAMP : return "ampamp";
		case JAGP_KIND_BARBAR : return "barbar";
		case JAGP_KIND_PLUSPLUS : return "plusplus";
		case JAGP_KIND_SUBSUB : return "subsub";
		case JAGP_KIND_PLUS : return "plus";
		case JAGP_KIND_SUB : return "sub";
		case JAGP_KIND_STAR : return "star";
		case JAGP_KIND_SLASH : return "slash";
		case JAGP_KIND_AMP : return "amp";
		case JAGP_KIND_BAR : return "bar";
		case JAGP_KIND_CARET : return "caret";
		case JAGP_KIND_PERCENT : return "percent";
		case JAGP_KIND_LTLT : return "ltlt";
		case JAGP_KIND_GTGT : return "gtgt";
		case JAGP_KIND_GTGTGT : return "gtgtgt";
		case JAGP_KIND_PLUSEQ : return "pluseq";
		case JAGP_KIND_SUBEQ : return "subeq";
		case JAGP_KIND_STAREQ : return "stareq";
		case JAGP_KIND_SLASHEQ : return "slasheq";
		case JAGP_KIND_AMPEQ : return "ampeq";
		case JAGP_KIND_BAREQ : return "bareq";
		case JAGP_KIND_CARETEQ : return "careteq";
		case JAGP_KIND_PERCENTEQ : return "percenteq";
		case JAGP_KIND_LTLTEQ : return "ltlteq";
		case JAGP_KIND_GTGTEQ : return "gtgteq";
		case JAGP_KIND_GTGTGTEQ : return "gtgtgteq";
		case JAGP_KIND_MONKEYS_AT : return "monkeys_at";
		case JAGP_KIND_CUSTOM : return "custom";
	}
	return "???";
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPToken *instance = JAGP_TOKEN(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	if (instance->value) {
		cat_string_wo_format(append_to, "%s[%s(%d), value=%O, start=%O]", iname, l_descr(instance->kind), instance->kind, instance->value, instance->cur_start);
	} else {
		cat_string_wo_format(append_to, "%s[%s(%d), start=%O]", iname, l_descr(instance->kind), instance->kind, instance->cur_start);
	}
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
