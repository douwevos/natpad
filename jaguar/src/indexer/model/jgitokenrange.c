/*
   File:    jgitokenrange.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 16, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "jgitokenrange.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JgiTokenRange"
#include <logging/catlog.h>

struct _JgiTokenRangePrivate {
	CatArrayWo *e_token_list;
	int first_index;
	int last_index;
};

#define JGI_TYPE_TOKEN_RANGE_ITER            (jgi_token_range_iter_get_type())
#define JGI_TOKEN_RANGE_ITER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jgi_token_range_iter_get_type(), JgiTokenRangeIter))
#define JGI_TOKEN_RANGE_ITER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JGI_TYPE_TOKEN_RANGE_ITER, JgiTokenRangeIterClass))
#define JGI_IS_TOKEN_RANGE_ITER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JGI_TYPE_TOKEN_RANGE_ITER))
#define JGI_IS_TOKEN_RANGE_ITER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JGI_TYPE_TOKEN_RANGE_ITER))
#define JGI_TOKEN_RANGE_ITER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JGI_TYPE_TOKEN_RANGE_ITER, JgiTokenRangeIterClass))


typedef struct _JgiTokenRangeIter	      JgiTokenRangeIter;
typedef struct _JgiTokenRangeIterClass	  JgiTokenRangeIterClass;


struct _JgiTokenRangeIter {
	GObject parent;
	JgiTokenRange *token_range;
	int offset;
};

struct _JgiTokenRangeIterClass {
	GObjectClass parent_class;
};


GType jgi_token_range_iter_get_type(void);

static void l_iterator_interface_init(CatIIteratorInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JgiTokenRangeIter, jgi_token_range_iter, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IITERATOR, l_iterator_interface_init);
});

static void l_dispose_iter(GObject *object);

static void jgi_token_range_iter_class_init(JgiTokenRangeIterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose_iter;
}

static void jgi_token_range_iter_init(JgiTokenRangeIter *array_iter) {
}

static void l_dispose_iter(GObject *object) {
	JgiTokenRangeIter *array_iter = JGI_TOKEN_RANGE_ITER(object);
	cat_unref_ptr(array_iter->token_range);
}


G_DEFINE_TYPE_WITH_PRIVATE(JgiTokenRange, jgi_token_range, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jgi_token_range_class_init(JgiTokenRangeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jgi_token_range_init(JgiTokenRange *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JgiTokenRange *instance = JGI_TOKEN_RANGE(object);
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(instance);
	cat_unref_ptr(priv->e_token_list);
	G_OBJECT_CLASS(jgi_token_range_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jgi_token_range_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JgiTokenRange *jgi_token_range_new(CatArrayWo *e_token_list, int first_index, int last_index) {
	JgiTokenRange *result = g_object_new(JGI_TYPE_TOKEN_RANGE, NULL);
	cat_ref_anounce(result);
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(result);
	priv->e_token_list = cat_ref_ptr(e_token_list);
	priv->first_index = first_index;
	priv->last_index = last_index;
	return result;
}

int jgi_token_range_get_first_index(JgiTokenRange *token_range) {
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(token_range);
	return priv->first_index;
}

int jgi_token_range_get_last_index(JgiTokenRange *token_range) {
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(token_range);
	return priv->last_index;
}

JagPToken *jgi_token_range_get_first(JgiTokenRange *token_range) {
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(token_range);
	int f = priv->first_index;
	return (JagPToken *) cat_array_wo_get(priv->e_token_list, f);
}

JagPToken *jgi_token_range_get_last(JgiTokenRange *token_range) {
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(token_range);
	int f = priv->last_index;
	return (JagPToken *) cat_array_wo_get(priv->e_token_list, f);
}

CatArrayWo *e_jgi_token_range_get_raw_tokens(JgiTokenRange *token_range) {
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(token_range);
	return priv->e_token_list;
}

void jgi_token_range_dump(JgiTokenRange *token_range) {
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(token_range);
	int idx;
	for(idx=priv->first_index; idx<=priv->last_index; idx++) {
		JagPToken *token = (JagPToken *) cat_array_wo_get(priv->e_token_list, idx);
		cat_log_print("DUMP", "## [%d] %o", idx, token);
	}
}

CatIIterator *jgi_token_range_iterator(JgiTokenRange *token_range) {
	if (token_range==NULL) {
		return NULL;
	}
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(token_range);
	JgiTokenRangeIter *result = g_object_new(JGI_TYPE_TOKEN_RANGE_ITER, NULL);
	cat_ref_anounce(result);
	result->offset = priv->first_index;
	result->token_range = cat_ref_ptr(token_range);
	return (CatIIterator *) result;
}




static gboolean l_iter_has_next(CatIIterator *self) {
	if (self==NULL) {
		return FALSE;
	}
	JgiTokenRangeIter *this = JGI_TOKEN_RANGE_ITER(self);
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(this->token_range);
	int li = priv->last_index;
	cat_log_trace("_has_next:this->offset=%d", this->offset);
	return this->offset<=li;
}

static gboolean l_iter_is_last(CatIIterator *self) {
	JgiTokenRangeIter *this = JGI_TOKEN_RANGE_ITER(self);
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(this->token_range);
	int li = priv->last_index;
	return this->offset==li;
}

static gpointer l_iter_next(CatIIterator *self) {
	JgiTokenRangeIter *this = JGI_TOKEN_RANGE_ITER(self);
	cat_log_trace("_iter_next:this->offset=%d", this->offset);
	JgiTokenRangePrivate *priv = jgi_token_range_get_instance_private(this->token_range);
	CatArrayWo *e_token_list = priv->e_token_list;
	return cat_array_wo_get(e_token_list, this->offset++);
}

static void l_iterator_interface_init(CatIIteratorInterface *iface) {
	iface->has_next = l_iter_has_next;
	iface->next = l_iter_next;
	iface->is_last = l_iter_is_last;
}

