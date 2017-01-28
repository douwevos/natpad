/*
   File:    cushighlightermap.c
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 6, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "cushighlightermap.h"
#include "cushighlighter.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CusHighlighterMap"
#include <logging/catlog.h>

struct _CusHighlighterMapPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CusHighlighterMap, cus_highlighter_map, DRA_TYPE_HIGHLIGHTER_MAP,
		G_ADD_PRIVATE(CusHighlighterMap)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static DraHighlighter *l_create_connector(DraHighlighterMap *map, WorService *wor_service, ChaDocument *document, CatStringWo *a_markup_key);

static void cus_highlighter_map_class_init(CusHighlighterMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraHighlighterMapClass *map_class = DRA_HIGHLIGHTER_MAP_CLASS(clazz);
	map_class->createConnector = l_create_connector;
}

static void cus_highlighter_map_init(CusHighlighterMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	CusHighlighterMap *instance = CUS_HIGHLIGHTER_MAP(object);
//	CusHighlighterMapPrivate *priv = cus_highlighter_map_get_instance_private(instance);
	G_OBJECT_CLASS(cus_highlighter_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cus_highlighter_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CusHighlighterMap *cus_highlighter_map_new(WorService *wor_service) {
	CusHighlighterMap *result = g_object_new(CUS_TYPE_HIGHLIGHTER_MAP, NULL);
	cat_ref_anounce(result);
	CusHighlighterMapPrivate *priv = cus_highlighter_map_get_instance_private(result);
	dra_highlighter_map_construct((DraHighlighterMap *) result, wor_service, (CatStringWo *) cat_string_wo_new_data("Cuscus"));
	return result;
}


static DraHighlighter *l_create_connector(DraHighlighterMap *map, WorService *wor_service, ChaDocument *document, CatStringWo *a_markup_key) {
	return (DraHighlighter *) cus_highlighter_new(wor_service, document, a_markup_key);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CusHighlighterMap *instance = CUS_HIGHLIGHTER_MAP(self);
	CusHighlighterMapPrivate *priv = cus_highlighter_map_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

