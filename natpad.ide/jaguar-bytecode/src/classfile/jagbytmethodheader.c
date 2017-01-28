/*
   File:    jagbytmethodheader.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#include "jagbytmethodheader.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytMethodHeader"
#include <logging/catlog.h>

struct _JagBytMethodHeaderPrivate {
	JagAstDeclarationType *returnType;
	CatStringWo *a_methodName;
	gboolean isConstructor;
	JagAstArgumentList *argument_list;
	JagBytExceptions *exceptions;
};

G_DEFINE_TYPE (JagBytMethodHeader, jag_byt_method_header, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_method_header_class_init(JagBytMethodHeaderClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytMethodHeaderPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_method_header_init(JagBytMethodHeader *instance) {
	JagBytMethodHeaderPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_METHOD_HEADER, JagBytMethodHeaderPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytMethodHeader *instance = JAG_BYT_METHOD_HEADER(object);
	JagBytMethodHeaderPrivate *priv = instance->priv;
	cat_unref_ptr(priv->argument_list);
	cat_unref_ptr(priv->exceptions);
	cat_unref_ptr(priv->a_methodName);
	cat_unref_ptr(priv->returnType);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}



JagBytMethodHeader *jag_byt_method_header_new_full(CatStringWo *a_method_name, gboolean is_constructor, JagAstDeclarationType *return_type, JagAstArgumentList *argument_list) {
	JagBytMethodHeader *result = g_object_new(JAG_BYT_TYPE_METHOD_HEADER, NULL);
	cat_ref_anounce(result);
	JagBytMethodHeaderPrivate *priv = result->priv;
	priv->a_methodName = cat_ref_ptr(a_method_name);
	priv->returnType = is_constructor ? NULL : cat_ref_ptr(return_type);
	priv->argument_list = cat_ref_ptr(argument_list);
	priv->isConstructor = is_constructor;
	priv->exceptions = NULL;
	return result;
}


static CatS l_s_txt_init = CAT_S_DEF("<init>");

JagBytMethodHeader *jag_byt_method_header_new(JagBytName *class_name, CatStringWo *a_method_name, CatStringWo *a_description) {
	cat_log_debug("methodName=%o, description=%o", a_method_name, a_description);

	gboolean isConstructor = FALSE;
	if (cat_string_wo_equal(a_method_name, CAT_S(l_s_txt_init))) {
		a_method_name = jag_byt_name_get_short_type_name(class_name);
		isConstructor = TRUE;
	}
	int idx = 0;
	gboolean isParameter = TRUE;
	JagAstDeclarationType *returnType = NULL;
	JagAstArgumentList *argument_list = jag_ast_argument_list_new();
	if (cat_string_wo_char_at(a_description, idx++)=='(') {
		gboolean expectRef = FALSE;
		int referenceCount = 0;
		JagBytType dPrimitive = JAG_BYT_TYPE_VOID;
		CatStringWo *e_extBuf = cat_string_wo_new();
		while(idx<cat_string_wo_length(a_description)) {
			gboolean storeAndCreateNew = FALSE;
			char ch = cat_string_wo_char_at(a_description, idx);
			if (expectRef) {
				if (ch==';') {
					storeAndCreateNew = TRUE;
				} else {
					cat_string_wo_append_char(e_extBuf, ch);
				}
			} else {
				switch(ch) {
					case 'B' : {
						dPrimitive = JAG_BYT_TYPE_BYTE;
						storeAndCreateNew = TRUE;
					} break;
					case 'C': {
						dPrimitive = JAG_BYT_TYPE_CHAR;
						storeAndCreateNew = TRUE;
					} break;
					case 'D': {
						dPrimitive = JAG_BYT_TYPE_DOUBLE;
						storeAndCreateNew = TRUE;
					} break;
					case 'F': {
						dPrimitive = JAG_BYT_TYPE_FLOAT;
						storeAndCreateNew = TRUE;
					} break;
					case 'I': {
						dPrimitive = JAG_BYT_TYPE_INT;
						storeAndCreateNew = TRUE;
					} break;
					case 'J': {
						dPrimitive = JAG_BYT_TYPE_LONG;
						storeAndCreateNew = TRUE;
					} break;
					case 'L': {
						expectRef = TRUE;
					} break;
					case 'S': {
						dPrimitive = JAG_BYT_TYPE_SHORT;
						storeAndCreateNew = TRUE;
					} break;
					case 'Z': {
						dPrimitive = JAG_BYT_TYPE_BOOLEAN;
						storeAndCreateNew = TRUE;
					} break;
					case 'V': {
						dPrimitive = JAG_BYT_TYPE_VOID;
						storeAndCreateNew = TRUE;
					} break;
					case '[': {
						referenceCount++;
					} break;
					case ')' : {
						isParameter = FALSE;
					} break;

				}

			}
			if (storeAndCreateNew) {
				JagAstDeclarationType *desc = NULL;
				if (expectRef) {
					JagBytName *referencTypeName = jag_byt_name_new(cat_string_wo_anchor(e_extBuf, 0), TRUE);
					e_extBuf = cat_string_wo_new();
					desc = jag_ast_declaration_type_new_reference(referencTypeName, referenceCount);
					cat_unref_ptr(referencTypeName);
				} else {
					desc = jag_ast_declaration_type_new(dPrimitive, referenceCount);
				}
				if (isParameter) {
					CatStringWo *e_name = cat_string_wo_new_with("arg");
					cat_string_wo_append_decimal(e_name, jag_ast_argument_list_count(argument_list));
					JagAstArgument *argument = jag_ast_argument_new(desc, cat_string_wo_anchor(e_name, TRUE));
					cat_unref_ptr(desc);
					jag_ast_argument_list_add(argument_list, argument);
					cat_unref_ptr(argument);
				} else {
					cat_unref_ptr(returnType);
					returnType = desc;
					break;
				}
				referenceCount = 0;
				expectRef = FALSE;
				dPrimitive = JAG_BYT_TYPE_VOID;
			}
			idx++;
		}
		cat_unref_ptr(e_extBuf);
	}

	JagBytMethodHeader *result = g_object_new(JAG_BYT_TYPE_METHOD_HEADER, NULL);
	cat_ref_anounce(result);
	JagBytMethodHeaderPrivate *priv = result->priv;
	priv->a_methodName = a_method_name;
	priv->returnType = isConstructor ? NULL : cat_ref_ptr(returnType);
	cat_unref_ptr(returnType);
	priv->argument_list = argument_list;
	priv->isConstructor = isConstructor;
	priv->exceptions = NULL;
	return result;
}


void jag_byt_method_header_set_exceptions(JagBytMethodHeader *method_header, JagBytExceptions *excpetions) {
	JagBytMethodHeaderPrivate *priv = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header);
	cat_ref_swap(priv->exceptions, excpetions);
}

JagBytExceptions *jag_byt_method_header_get_exceptions(JagBytMethodHeader *method_header) {
	JagBytMethodHeaderPrivate *priv = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header);
	return priv->exceptions;
}

JagAstArgumentList *jag_byt_method_header_get_argument_list(JagBytMethodHeader *method_header) {
	JagBytMethodHeaderPrivate *priv = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header);
	return priv->argument_list;
}


JagAstDeclarationType *jag_byt_method_header_get_return_type(JagBytMethodHeader *method_header) {
	JagBytMethodHeaderPrivate *priv = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header);
	return priv->returnType;
}

CatStringWo *jag_byt_method_header_get_method_name(JagBytMethodHeader *method_header) {
	JagBytMethodHeaderPrivate *priv = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header);
	return priv->a_methodName;
}


gboolean jag_byt_method_header_is_constructor(JagBytMethodHeader *method_header) {
	JagBytMethodHeaderPrivate *priv = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header);
	return priv->isConstructor;
}


gboolean jag_byt_method_header_equal(JagBytMethodHeader *method_header_a, JagBytMethodHeader *method_header_b) {
	if (method_header_a == method_header_b) {
		return TRUE;
	}

	if (method_header_a==NULL || method_header_b==NULL) {
		return FALSE;
	}
	JagBytMethodHeaderPrivate *priv_a = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header_a);
	JagBytMethodHeaderPrivate *priv_b = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header_b);
	if ((priv_a->isConstructor!=priv_b->isConstructor) ||
			!cat_string_wo_equal(priv_a->a_methodName, priv_b->a_methodName) ||
			!jag_ast_declaration_type_equal(priv_a->returnType, priv_b->returnType)) {
		return FALSE;
	}

	if (!jag_ast_argument_list_equal(priv_a->argument_list, priv_b->argument_list)) {
		return FALSE;
	}
	if (!jag_byt_exceptions_equal(priv_a->exceptions, priv_b->exceptions)) {
		return FALSE;
	}

	return TRUE;
}


CatStringWo *jag_byt_method_header_as_text(JagBytMethodHeader *method_header, CatStringWo *e_dump_buffer) {
	JagBytMethodHeaderPrivate *priv = JAG_BYT_METHOD_HEADER_GET_PRIVATE(method_header);
	if (e_dump_buffer==NULL) {
		e_dump_buffer = cat_string_wo_new();
	}
	if (!priv->isConstructor) {
		jag_ast_declaration_type_as_text(priv->returnType, e_dump_buffer);	// TODO implement
		cat_string_wo_append_char(e_dump_buffer, ' ');
	}
	cat_string_wo_append_chars(e_dump_buffer, cat_string_wo_getchars(priv->a_methodName));
	cat_string_wo_append_char(e_dump_buffer, '(');
	if (priv->argument_list) {
		jag_ast_argument_list_as_text(priv->argument_list, e_dump_buffer);
	}
	cat_string_wo_append_char(e_dump_buffer, ')');
	return e_dump_buffer;
}


//	@Override
//	public String toString() {
//		StringBuilder buf = new StringBuilder();
//		if (!isConstructor) {
//			buf.append(returnType).append(" ");
//		}
//		buf.append(methodName).append("(");
//		boolean isFirst = true;
//		for(Argument arg : args) {
//			if (isFirst) {
//				isFirst = false;
//			} else {
//				buf.append(", ");
//			}
//			buf.append(arg);
//		}
//		buf.append(")");
//		return buf.toString();
//	}


