
/*
   File:    grolalrstatemap.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 2, 2016
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

#include "grolalrstatemap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroLalrStateMap"
#include <logging/catlog.h>

struct _GroLalrStateMapPrivate {
	CatHashMapWo *internal_map;	/* <GroLalrItemSet, GroLalrState> */
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroLalrStateMap, gro_lalr_state_map, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroLalrStateMap)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_lalr_state_map_class_init(GroLalrStateMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_lalr_state_map_init(GroLalrStateMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroLalrStateMap *instance = GRO_LALR_STATE_MAP(object);
//	GroLalrStateMapPrivate *priv = gro_lalr_state_map_get_instance_private(instance);
	G_OBJECT_CLASS(gro_lalr_state_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_lalr_state_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroLalrStateMap *gro_lalr_state_map_new() {
	GroLalrStateMap *result = g_object_new(GRO_TYPE_LALR_STATE_MAP, NULL);
	cat_ref_anounce(result);
	GroLalrStateMapPrivate *priv = gro_lalr_state_map_get_instance_private(result);
	priv->internal_map = cat_hash_map_wo_new((GHashFunc) gro_lalr_item_set_hash, (GEqualFunc) gro_lalr_item_set_equal);
	return result;
}

int gro_lalr_state_map_count(const GroLalrStateMap *state_map) {
	GroLalrStateMapPrivate *priv = gro_lalr_state_map_get_instance_private((GroLalrStateMap *) state_map);
	return cat_hash_map_wo_size(priv->internal_map);
}

void gro_lalr_state_map_add(GroLalrStateMap *state_map, GroLalrState *lalr_state) {
	GroLalrStateMapPrivate *priv = gro_lalr_state_map_get_instance_private(state_map);
	GroLalrItemSet *item_set = gro_lalr_state_get_item_set(lalr_state);
//	cat_log_debug("lalr_state=%o", lalr_state);
	if (cat_hash_map_wo_get(priv->internal_map, item_set)!=NULL) {
		cat_log_error("trying to add an LALR-State twice:%o", lalr_state);
		return;
	}
	cat_hash_map_wo_put(priv->internal_map, (GObject *) item_set, (GObject *) lalr_state);
}

GroLalrStateIterator *gro_lalr_state_map_iterator(GroLalrStateMap *state_map) {
	GroLalrStateMapPrivate *priv = gro_lalr_state_map_get_instance_private(state_map);
	CatArrayWo *values = cat_hash_map_wo_enlist_values(priv->internal_map, NULL);
	CatIIterator *result = cat_array_wo_iterator(values);
	cat_unref_ptr(values);
	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroLalrStateMap *instance = GRO_LALR_STATE_MAP(self);
	GroLalrStateMapPrivate *priv = gro_lalr_state_map_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, map=%O]", iname, self, priv->internal_map);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
