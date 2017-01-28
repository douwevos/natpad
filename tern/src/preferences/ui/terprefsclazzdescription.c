/*
   File:    terprefsclazzdescription.c
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 26, 2014
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

#include "terprefsclazzdescription.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerPrefsClazzDescription"
#include <logging/catlog.h>

struct _TerPrefsClazzDescriptionPrivate {
	TerPrefsClazzWo *prefs_clazz;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerPrefsClazzDescription, ter_prefs_clazz_description, COW_TYPE_PANEL_DESCRIPTION,
		G_ADD_PRIVATE(TerPrefsClazzDescription)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_prefs_clazz_description_class_init(TerPrefsClazzDescriptionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_prefs_clazz_description_init(TerPrefsClazzDescription *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerPrefsClazzDescription *instance = TER_PREFS_CLAZZ_DESCRIPTION(object);
	TerPrefsClazzDescriptionPrivate *priv = ter_prefs_clazz_description_get_instance_private(instance);
	cat_unref_ptr(priv->prefs_clazz);
	G_OBJECT_CLASS(ter_prefs_clazz_description_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_prefs_clazz_description_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerPrefsClazzDescription *ter_prefs_clazz_description_new(TerPrefsClazzWo *prefs_clazz) {
	TerPrefsClazzDescription *result = g_object_new(TER_TYPE_PREFS_CLAZZ_DESCRIPTION, NULL);
	cat_ref_anounce(result);
	TerPrefsClazzDescriptionPrivate *priv = ter_prefs_clazz_description_get_instance_private(result);
	priv->prefs_clazz = cat_ref_ptr(prefs_clazz);
	CatStringWo *clazz_name = ter_prefs_clazz_wo_get_name(prefs_clazz);
	cow_panel_description_construct((CowPanelDescription *) result, cat_ref_ptr(clazz_name));
	return result;
}


void ter_prefs_clazz_description_set(TerPrefsClazzDescription *description, TerPrefsClazzWo *prefs_clazz) {
	TerPrefsClazzDescriptionPrivate *priv = ter_prefs_clazz_description_get_instance_private(description);
	cat_ref_swap(priv->prefs_clazz, prefs_clazz);
}

TerPrefsClazzWo *ter_prefs_clazz_description_get(TerPrefsClazzDescription *description) {
	TerPrefsClazzDescriptionPrivate *priv = ter_prefs_clazz_description_get_instance_private(description);
	return priv->prefs_clazz;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	TerPrefsClazzDescription *instance = TER_PREFS_CLAZZ_DESCRIPTION(self);
	TerPrefsClazzDescriptionPrivate *priv = ter_prefs_clazz_description_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p prefs_clazz=%o]", iname, self, priv->prefs_clazz);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

