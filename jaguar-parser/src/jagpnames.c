/*
   File:    jagpnames.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Jul 15, 2017
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

#include "jagpnames.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPNames"
#include <logging/catlog.h>

struct _JagPNamesPrivate {
	CatHashMapWo *map;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPNames, jagp_names, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPNames)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_names_class_init(JagPNamesClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_names_init(JagPNames *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPNames *instance = JAGP_NAMES(object);
//	JagPNamesPrivate *priv = jagp_names_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_names_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_names_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPNames *jagp_names_new() {
	JagPNames *result = g_object_new(JAGP_TYPE_NAMES, NULL);
	cat_ref_anounce(result);
	JagPNamesPrivate *priv = jagp_names_get_instance_private(result);
	priv->map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	return result;
}

JagPName *jagp_names_get(JagPNames *names, CatStringWo *text) {
	JagPNamesPrivate *priv = jagp_names_get_instance_private(names);
	JagPName *result = (JagPName *) cat_hash_map_wo_get(priv->map, text);
	if (result == NULL) {
		CatStringWo *ct = cat_string_wo_clone(text, CAT_CLONE_DEPTH_AS_ANCHORED);
		result = jagp_name_new(text);
		cat_hash_map_wo_put(priv->map, (GObject *) ct, (GObject *) result);
		cat_unref(ct);
		cat_unref(result);
	}
	return result;
}

JagPName *jagp_names_by_chars(JagPNames *names, const char *txt) {
	CatStringWo *tmp = cat_string_wo_new_data(txt);
	JagPName *result = jagp_names_get(names, tmp);
	cat_unref_ptr(tmp);
	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPNames *instance = JAGP_NAMES(self);
	JagPNamesPrivate *priv = jagp_names_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
