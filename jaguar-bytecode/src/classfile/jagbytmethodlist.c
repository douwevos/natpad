/*
   File:    jagbytmethodlist.c
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

#include "jagbytmethodlist.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytMethodList"
#include <logging/catlog.h>

struct _JagBytMethodListPrivate {
	CatArrayWo *a_method_list;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytMethodList, jag_byt_method_list, G_TYPE_INITIALLY_UNOWNED)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_method_list_class_init(JagBytMethodListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_method_list_init(JagBytMethodList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytMethodList *instance = JAG_BYT_METHOD_LIST(object);
	JagBytMethodListPrivate *priv = jag_byt_method_list_get_instance_private(instance);
	cat_unref_ptr(priv->a_method_list);
	G_OBJECT_CLASS(jag_byt_method_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_method_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytMethodList *jag_byt_method_list_new(CatArrayWo *a_method_list) {
	JagBytMethodList *result = g_object_new(JAG_BYT_TYPE_METHOD_LIST, NULL);
	cat_ref_anounce(result);
	JagBytMethodListPrivate *priv = jag_byt_method_list_get_instance_private(result);
	priv->a_method_list = cat_ref_ptr(a_method_list);
	return result;
}

int jag_byt_method_list_count(JagBytMethodList *method_list) {
	JagBytMethodListPrivate *priv = jag_byt_method_list_get_instance_private(method_list);
	return cat_array_wo_size(priv->a_method_list);
}

JagBytMethod *jag_byt_method_list_get_at(JagBytMethodList *method_list, int index) {
	JagBytMethodListPrivate *priv = jag_byt_method_list_get_instance_private(method_list);
	return JAG_BYT_METHOD(cat_array_wo_get(priv->a_method_list, index));
}

CatIIterator *jag_byt_method_list_iterator(JagBytMethodList *method_list) {
	JagBytMethodListPrivate *priv = jag_byt_method_list_get_instance_private(method_list);
	return cat_array_wo_iterator(priv->a_method_list);
}
