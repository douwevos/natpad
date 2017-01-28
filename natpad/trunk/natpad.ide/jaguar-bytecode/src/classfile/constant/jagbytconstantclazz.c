/*
   File:    jagbytconstantclazz.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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

#include "jagbytconstantclazz.h"
#include "jagbytconstantpool.h"
#include "jagbytconstantutf8text.h"
#include "jagbyticonstant.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantClazz"
#include <logging/catlog.h>

struct _JagBytConstantClazzPrivate {
	int name_index;
	gboolean is_resolved;
	JagAstDeclarationType *parsed_descriptor;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantClazz, jag_byt_constant_clazz, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_constant_clazz_class_init(JagBytConstantClazzClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytConstantClazzPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_constant_clazz_init(JagBytConstantClazz *instance) {
	JagBytConstantClazzPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_CONSTANT_CLAZZ, JagBytConstantClazzPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytConstantClazz *instance = JAG_BYT_CONSTANT_CLAZZ(object);
	JagBytConstantClazzPrivate *priv = instance->priv;
	cat_unref_ptr(priv->parsed_descriptor);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantClazz *jag_byt_constant_clazz_new(int name_index) {
	JagBytConstantClazz *result = g_object_new(JAG_BYT_TYPE_CONSTANT_CLAZZ, NULL);
	cat_ref_anounce(result);
	JagBytConstantClazzPrivate *priv = result->priv;
	priv->name_index = name_index;
	priv->is_resolved = FALSE;
	return result;
}

int jag_byt_constant_clazz_get_name_index(JagBytConstantClazz *constant_clazz) {
	return constant_clazz->priv->name_index;
}


JagBytName *jag_byt_constant_clazz_get_refrence_type_name(JagBytConstantClazz *constant_clazz) {
	JagBytConstantClazzPrivate *priv = JAG_BYT_CONSTANT_CLAZZ_GET_PRIVATE(constant_clazz);
	return jag_ast_declaration_type_get_reference_type_name(priv->parsed_descriptor);
}

JagAstDeclarationType *jag_byt_constant_clazz_get_parsed_descriptor(JagBytConstantClazz *constant_clazz) {
	JagBytConstantClazzPrivate *priv = JAG_BYT_CONSTANT_CLAZZ_GET_PRIVATE(constant_clazz);
	return priv->parsed_descriptor;
}



static JagAstDeclarationType *l_parse_descriptor(CatStringWo *a_text) {
	int arrayCnt = 0;
	JagAstDeclarationType *result = NULL;
	if (cat_string_wo_length(a_text)>0 && cat_string_wo_char_at(a_text, 0)=='[') {
		int idx = 0;
		gboolean parsingName = FALSE;
		CatStringWo *e_buf = cat_string_wo_new();
		while(result==NULL && idx<cat_string_wo_length(a_text)) {
			char ch = cat_string_wo_char_at(a_text, idx++);

			if (parsingName) {
				if (ch==';') {
					e_buf = cat_string_wo_anchor(e_buf, 0);
					JagBytName *name = jag_byt_name_new(e_buf, TRUE);
					result = jag_ast_declaration_type_new_reference(name, arrayCnt);
					cat_unref_ptr(name);
				} else {
					cat_string_wo_append_char(e_buf, ch);
				}
			} else {
				switch(ch) {
					case '[' : {
						arrayCnt++;
					} break;
					case 'L' : {
						parsingName = TRUE;
					} break;
					case 'B' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_BYTE, arrayCnt); break;
					case 'C' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_CHAR, arrayCnt); break;
					case 'D' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_DOUBLE, arrayCnt); break;
					case 'F' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_FLOAT, arrayCnt); break;
					case 'I' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_INT, arrayCnt); break;
					case 'J' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_LONG, arrayCnt);  break;
					case 'S' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_SHORT, arrayCnt);  break;
					case 'Z' : result = jag_ast_declaration_type_new(JAG_BYT_TYPE_BOOLEAN, arrayCnt);  break;
					default : {
						// TODO: handle invalid descrptior
					} break;
				}
			}
		}
		cat_unref_ptr(e_buf);
	} else {
		JagBytName *name = jag_byt_name_new(a_text, TRUE);
		result = jag_ast_declaration_type_new_reference(name, arrayCnt);
		cat_unref_ptr(name);
	}
	return result;
}


/********************* start JagBytIConstantInterface implementation *********************/

static gboolean l_constant_is_resolved(JagBytIConstant *self) {
	JagBytConstantClazzPrivate *priv = JAG_BYT_CONSTANT_CLAZZ_GET_PRIVATE(self);
	return priv->is_resolved;
}

static gboolean l_constant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack) {
	JagBytConstantClazzPrivate *priv = JAG_BYT_CONSTANT_CLAZZ_GET_PRIVATE(self);
	JagBytConstantUtf8Text *constant_utf_text = (JagBytConstantUtf8Text *) jag_byt_constant_pool_get_unresolved(constantPool, priv->name_index-1);
	CatStringWo *a_text = jag_byt_constant_utf8_text_get_value(constant_utf_text);
	priv->parsed_descriptor = l_parse_descriptor(a_text);
//	if (parsedDescriptor==null) {
//		throw new RuntimeException("text="+constantUtfText.text);
//	}
	priv->is_resolved = TRUE;
	return TRUE;
}


static void l_constant_iface_init(JagBytIConstantInterface *iface) {
	iface->isResolved = l_constant_is_resolved;
	iface->tryResolve = l_constant_try_resolve;
}


/********************* end JagBytIConstantInterface implementation *********************/
