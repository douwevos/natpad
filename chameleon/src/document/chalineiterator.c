/*
   File:    chalineiterator.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 16, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chalineiterator.h"
#include "chalinelocationwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaLineIterator"
#include <logging/catlog.h>

struct _ChaLineIteratorPrivate {
	ChaRevisionWo *revision;
	ChaLineLocationWo *location;
	ChaLineLocationWo *next_location;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_iterator_iface_init(CatIIteratorInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaLineIterator, cha_line_iterator, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaLineIterator)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IITERATOR, l_iterator_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_line_iterator_class_init(ChaLineIteratorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_line_iterator_init(ChaLineIterator *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaLineIterator *instance = CHA_LINE_ITERATOR(object);
	ChaLineIteratorPrivate *priv = cha_line_iterator_get_instance_private(instance);
	cat_unref_ptr(priv->revision);
	cat_unref_ptr(priv->next_location);
	cat_unref_ptr(priv->location);
	G_OBJECT_CLASS(cha_line_iterator_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_line_iterator_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaLineIterator *cha_line_iterator_new(ChaRevisionWo *revision, unsigned long long start_row) {
	ChaLineIterator *result = g_object_new(CHA_TYPE_LINE_ITERATOR, NULL);
	cat_ref_anounce(result);
	ChaLineIteratorPrivate *priv = cha_line_iterator_get_instance_private(result);
	priv->revision = cat_ref_ptr(revision);
	priv->location = NULL;
	priv->next_location = cha_revision_wo_calculate_line_location(revision, start_row);
	priv->next_location = cha_line_location_wo_anchor(priv->next_location, 0);
	return result;
}


ChaLineIterator *cha_line_iterator_new_line_location(ChaRevisionWo *revision, ChaLineLocationWo *ll) {
	ChaLineIterator *result = g_object_new(CHA_TYPE_LINE_ITERATOR, NULL);
	cat_ref_anounce(result);
	ChaLineIteratorPrivate *priv = cha_line_iterator_get_instance_private(result);
	priv->revision = cat_ref_ptr(revision);
	priv->location = NULL;
	ChaLineLocationWo *new_ll = cha_line_location_wo_clone(ll, CAT_CLONE_DEPTH_FULL);
	if (cha_line_location_wo_bind_to_revision(new_ll, priv->revision)) {
		priv->next_location = cha_line_location_wo_anchor(new_ll, 0);
	} else {
		cat_unref_ptr(new_ll);
		priv->next_location = NULL;
	}
	return result;
}


/********************* start CatIIterator implementation *********************/

static gboolean l_has_next(CatIIterator *self) {
	ChaLineIteratorPrivate *priv = cha_line_iterator_get_instance_private((ChaLineIterator *) self);
	return priv->next_location != NULL;
}

static gpointer l_next(CatIIterator *self) {
	ChaLineIteratorPrivate *priv = cha_line_iterator_get_instance_private((ChaLineIterator *) self);
	cat_unref_ptr(priv->location);
	if (priv->next_location) {
		priv->location = priv->next_location;
		priv->next_location = NULL;
		ChaLineLocationWo *nll = cha_line_location_wo_create_editable(priv->location);
		cha_line_location_wo_add_to_page_line_index(nll, 1);
		if (cha_line_location_wo_bind_to_revision(nll, priv->revision)) {
			priv->next_location = cha_line_location_wo_anchor(nll, 0);
		} else {
			cat_unref_ptr(nll);
		}
	}
	return priv->location;
}

static gboolean l_is_last(CatIIterator *self) {
	ChaLineIteratorPrivate *priv = cha_line_iterator_get_instance_private((ChaLineIterator *) self);
	return priv->next_location==NULL;
}


static void l_iterator_iface_init(CatIIteratorInterface *iface) {
	iface->has_next = l_has_next;
	iface->is_last = l_is_last;
	iface->next = l_next;
}

/********************* end CatIIterator implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaLineIterator *instance = CHA_LINE_ITERATOR(self);
	ChaLineIteratorPrivate *priv = cha_line_iterator_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, location=%o]", iname, self, priv->location);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

