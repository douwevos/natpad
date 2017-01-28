/*
   File:    jagbytconstantpool.c
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

#include "jagbytconstantpool.h"
#include "jagbyticonstantprovider.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantPool"
#include <logging/catlog.h>

struct _JagBytConstantPoolPrivate {
	CatArrayWo *a_pool;
	int this_class_index;
};

static void l_constant_provider_iface_init(JagBytIConstantProviderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantPool, jag_byt_constant_pool, G_TYPE_INITIALLY_UNOWNED, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT_PROVIDER, l_constant_provider_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_constant_pool_class_init(JagBytConstantPoolClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytConstantPoolPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_constant_pool_init(JagBytConstantPool *instance) {
	JagBytConstantPoolPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_CONSTANT_POOL, JagBytConstantPoolPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytConstantPool *instance = JAG_BYT_CONSTANT_POOL(object);
	JagBytConstantPoolPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_pool);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantPool *jag_byt_constant_pool_new(CatArrayWo *a_new_pool) {
	JagBytConstantPool *result = g_object_new(JAG_BYT_TYPE_CONSTANT_POOL, NULL);
	cat_ref_anounce(result);
	JagBytConstantPoolPrivate *priv = result->priv;
	priv->a_pool = cat_ref_ptr(a_new_pool);
	priv->this_class_index = -1;

	cat_log_on_detail({
		CatIIterator *iter = cat_array_wo_iterator(a_new_pool);
		while(cat_iiterator_has_next(iter)) {
			JagBytIConstant *iconst = (JagBytIConstant *) cat_iiterator_next(iter);
			cat_log_detail("iconst:%p - %s", iconst, g_type_name_from_instance((GTypeInstance *) iconst));
		}
		cat_unref_ptr(iter);
	});

	return result;
}


void jag_byt_constant_pool_set_this_class_index(JagBytConstantPool *pool, int this_class_index) {
	JagBytConstantPoolPrivate *priv = JAG_BYT_CONSTANT_POOL_GET_PRIVATE(pool);
	priv->this_class_index = this_class_index;
}


int jag_byt_constant_pool_size(JagBytConstantPool *pool) {
	return cat_array_wo_size(pool->priv->a_pool);
}

JagBytIConstant *jag_byt_constant_pool_get_this_class_constant(JagBytConstantPool *pool) {
	JagBytConstantPoolPrivate *priv = JAG_BYT_CONSTANT_POOL_GET_PRIVATE(pool);
	return jag_byt_constant_pool_get_unresolved(pool, priv->this_class_index-1);
}

JagBytIConstant *jag_byt_constant_pool_get_at(JagBytConstantPool *pool, int index) {
	return (JagBytIConstant *) cat_array_wo_get(pool->priv->a_pool, index);
}

JagBytIConstant *jag_byt_constant_pool_get_unresolved(JagBytConstantPool *pool, int index) {
	JagBytConstantPoolPrivate *priv = JAG_BYT_CONSTANT_POOL_GET_PRIVATE(pool);
	return (JagBytIConstant *) cat_array_wo_get(priv->a_pool, index);
}


JagBytIConstant *jag_byt_constant_pool_get_resolved(JagBytConstantPool *pool, int index) {
	JagBytConstantPoolPrivate *priv = JAG_BYT_CONSTANT_POOL_GET_PRIVATE(pool);
	JagBytIConstant *result = (JagBytIConstant *) cat_array_wo_get(priv->a_pool, index);
	jag_byt_constant_pool_resolve(pool, result);
	return result;
}

gboolean jag_byt_constant_pool_resolve(JagBytConstantPool *pool, JagBytIConstant *constant) {
//	JagBytConstantPoolPrivate *priv = JAG_BYT_CONSTANT_POOL_GET_PRIVATE(pool);
	if (jag_byt_iconstant_is_resolved(constant)) {
		return TRUE;
	}

	CatArrayWo *e_resolve_stack = cat_array_wo_new();
	cat_array_wo_append(e_resolve_stack, (GObject *) constant);
	while(cat_array_wo_size(e_resolve_stack)>0) {
		JagBytIConstant *stackt_constant = (JagBytIConstant *) cat_array_wo_get_last(e_resolve_stack);
		gboolean wasResolved = jag_byt_iconstant_is_resolved(stackt_constant);
		if (!wasResolved) {
			wasResolved = jag_byt_iconstant_try_resolve(stackt_constant, pool, e_resolve_stack);
		}
		if (wasResolved) {
			cat_array_wo_remove_last(e_resolve_stack, NULL);
		}
	}
	cat_unref_ptr(e_resolve_stack);
	return TRUE;
}



CatStringWo *jag_byt_descriptor_as_source(CatStringWo *a_descriptor, CatStringWo *e_source, int in_idx, int *out_idx) {
	if (e_source==NULL) {
		e_source = cat_string_wo_new();
	}
	int len = cat_string_wo_length(a_descriptor);

	int dimcount = 0;
	for(; in_idx<len; in_idx++) {
		char ch = cat_string_wo_char_at(a_descriptor, in_idx);
		switch(ch) {
			case 'B' : {
				cat_string_wo_append_chars(e_source, "byte");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'C' : {
				cat_string_wo_append_chars(e_source, "char");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'D' : {
				cat_string_wo_append_chars(e_source, "double");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'F' : {
				cat_string_wo_append_chars(e_source, "float");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'I' : {
				cat_string_wo_append_chars(e_source, "int");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'J' : {
				cat_string_wo_append_chars(e_source, "long");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'S' : {
				cat_string_wo_append_chars(e_source, "short");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'Z' : {
				cat_string_wo_append_chars(e_source, "boolean");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'V' : {
				cat_string_wo_append_chars(e_source, "void");
				*out_idx = in_idx+1;
				in_idx = len;
			} break;
			case 'L' : {
				in_idx++;
				int endidx = cat_string_wo_index_of_from(a_descriptor, ';', in_idx);
				if (endidx>=0) {
					CatStringWo *e_ref_type = cat_string_wo_new_sub(a_descriptor, in_idx, endidx-in_idx);
					cat_string_wo_replace_all_char(e_ref_type, '/', '.');
					cat_string_wo_append(e_source, e_ref_type);
					cat_unref_ptr(e_ref_type);
					endidx++;
					*out_idx = endidx;
				} else {
					cat_string_wo_append_chars(e_source, "???");
					*out_idx = len;
				}
				in_idx = len;
			} break;
			case '[' : {
				dimcount++;
			} break;
			default : {
				*out_idx = len;
				in_idx = len;
			} break;
		}
	}

	while(dimcount>0) {
		cat_string_wo_append_chars(e_source, "[]");
		dimcount--;
	}

	return e_source;
}







/********************* start JagBytIConstantProvider implementation *********************/

static JagBytIConstant *l_constant_provider_get(JagBytIConstantProvider *self, int index) {
	JagBytConstantPool *pool = JAG_BYT_CONSTANT_POOL(self);
	return jag_byt_constant_pool_get_resolved(pool, index);
}

static CatStringWo *l_constant_provider_get_constant_utf8_text(JagBytIConstantProvider *self, int index) {
	CatStringWo *a_result = NULL;
	JagBytConstantPool *pool = JAG_BYT_CONSTANT_POOL(self);
	JagBytIConstant *iconstant = jag_byt_constant_pool_get_resolved(pool, index);
	if (JAG_BYT_IS_CONSTANT_UTF8_TEXT(iconstant)) {
		JagBytConstantUtf8Text *utf8text = JAG_BYT_CONSTANT_UTF8_TEXT(iconstant);
		a_result = jag_byt_constant_utf8_text_get_value(utf8text);
	}
	return a_result;
}


static void l_constant_provider_iface_init(JagBytIConstantProviderInterface *iface) {
	iface->get = l_constant_provider_get;
	iface->getConstantUtf8Text = l_constant_provider_get_constant_utf8_text;
}

/********************* end JagBytIConstantProvider implementation *********************/



