/*
   File:    jagbyttrycatchlist.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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

#include "jagbyttrycatchlist.h"
#include "jagbytcatch.h"
#include "jagbytfinally.h"
#include "../ast/jagastdeclarationtype.h"
#include "attribute/jagbytattributecodeexception.h"
#include "constant/jagbytconstantclazz.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytTryCatchList"
#include <logging/catlog.h>

struct _JagBytTryCatchListPrivate {
	CatArrayWo *e_list;
};

G_DEFINE_TYPE (JagBytTryCatchList, jag_byt_try_catch_list, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_try_catch_list_class_init(JagBytTryCatchListClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytTryCatchListPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_try_catch_list_init(JagBytTryCatchList *instance) {
	JagBytTryCatchListPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_TRY_CATCH_LIST, JagBytTryCatchListPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytTryCatchList *instance = JAG_BYT_TRY_CATCH_LIST(object);
	JagBytTryCatchListPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_list);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static int l_try_catch_comp(const JagBytTryCatch *ptra, const JagBytTryCatch *ptrb) {
	if(ptra==ptrb) {
		return 0;
	}


	int start_a = jag_byt_try_catch_get_start_pc((JagBytTryCatch *) ptra);
	int start_b = jag_byt_try_catch_get_start_pc((JagBytTryCatch *) ptrb);

	if (start_a<start_b) {
		return -1;
	}

	if (start_a>start_b) {
		return 1;
	}

	int end_a = jag_byt_try_catch_get_end_pc((JagBytTryCatch *) ptra);
	int end_b = jag_byt_try_catch_get_end_pc((JagBytTryCatch *) ptrb);

	if (end_a<end_b) {
		return 1;
	}

	if (end_a>end_b) {
		return -1;
	}

	return 0;
}

JagBytTryCatchList *jag_byt_try_catch_list_new(CatArrayWo *e_attr_code_exceptions, JagBytIConstantProvider *constant_provider) {
	JagBytTryCatchList *result = g_object_new(JAG_BYT_TYPE_TRY_CATCH_LIST, NULL);
	cat_ref_anounce(result);
	JagBytTryCatchListPrivate *priv = result->priv;
	priv->e_list = cat_array_wo_new();

	CatArrayWo *e_todo_list = cat_array_wo_new();
	cat_array_wo_append_all(e_todo_list, e_attr_code_exceptions);

	while(cat_array_wo_size(e_todo_list)>0) {
		JagBytAttributeCodeException *main_code_exception = NULL;
		CatArrayWo *e_catchList = cat_array_wo_new();
//		int m_handler_pc = -1;
		int m_start_pc = -1;
		int m_end_pc = -1;
		int idx;
		for(idx=0; idx<cat_array_wo_size(e_todo_list); idx++) {
			JagBytAttributeCodeException *exception = (JagBytAttributeCodeException *) cat_array_wo_get(e_todo_list, idx);
			int e_handler_pc = jag_byt_attribute_code_exception_get_handler_pc(exception);
			int e_start_pc = jag_byt_attribute_code_exception_get_start_pc(exception);

			if (e_handler_pc==e_start_pc) {
				cat_array_wo_remove(e_todo_list, idx, NULL);
				idx--;
				continue;
			}
			int e_end_pc = jag_byt_attribute_code_exception_get_end_pc(exception);
			if (main_code_exception==NULL) {
				cat_array_wo_remove(e_todo_list, idx, (GObject **) (&main_code_exception));
				idx--;
//				m_handler_pc = jag_byt_attribute_code_exception_get_handler_pc(main_code_exception);
				m_start_pc = jag_byt_attribute_code_exception_get_start_pc(main_code_exception);
				m_end_pc = jag_byt_attribute_code_exception_get_end_pc(main_code_exception);
			} else if (m_start_pc==e_start_pc && m_end_pc==e_end_pc) {
				cat_array_wo_remove(e_todo_list, idx, NULL);
				idx--;
			} else {
				continue;
			}

			if (jag_byt_attribute_code_exception_get_catch_type(exception)<=0) {
				JagBytFinally *fin = jag_byt_finally_new(e_handler_pc);
				cat_array_wo_append(e_catchList, (GObject *) fin);
				cat_unref_ptr(fin);
			} else {
				JagBytIConstant *iconstant = jag_byt_iconstant_provider_get(constant_provider, jag_byt_attribute_code_exception_get_catch_type(exception)-1);
				if (JAG_BYT_IS_CONSTANT_CLAZZ(iconstant)) {
					JagBytConstantClazz *constant_clazz = JAG_BYT_CONSTANT_CLAZZ(iconstant);
					JagAstDeclarationType *declaration_type = jag_byt_constant_clazz_get_parsed_descriptor(constant_clazz);
					JagBytCatch *catch = jag_byt_catch_new(declaration_type, e_handler_pc);
					cat_array_wo_append(e_catchList, (GObject *) catch);
					cat_unref_ptr(catch);
				}
			}
		}

		if (main_code_exception) {
			JagBytTryCatch *try_catch = jag_byt_try_catch_new(m_start_pc, m_end_pc, e_catchList);
			cat_array_wo_append(priv->e_list, (GObject *) try_catch);
			cat_unref_ptr(try_catch);
		}
		cat_unref_ptr(main_code_exception);
		cat_unref(e_catchList);
	}

	cat_array_wo_sort(priv->e_list, (GCompareFunc) l_try_catch_comp);
	cat_unref_ptr(e_todo_list);
	return result;
}



int jag_byt_try_catch_list_count(JagBytTryCatchList *try_catch_list) {
	JagBytTryCatchListPrivate *priv = JAG_BYT_TRY_CATCH_LIST_GET_PRIVATE(try_catch_list);
	return cat_array_wo_size(priv->e_list);
}


JagBytTryCatch *jag_byt_try_catch_list_get(JagBytTryCatchList *try_catch_list, int index) {
	JagBytTryCatchListPrivate *priv = JAG_BYT_TRY_CATCH_LIST_GET_PRIVATE(try_catch_list);
	return (JagBytTryCatch *) cat_array_wo_get(priv->e_list, index);
}

