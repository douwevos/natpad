/*
   File:    elkchaprefsextractor.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jun 26, 2015
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

#include "elkchaprefsextractor.h"
#include "elkpreferenceswo.h"
#include <chameleon.h>
#include <cow.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkChaPrefsExtractor"
#include <logging/catlog.h>


static void l_accessor_iface_init(CowIEntryAccessorInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkChaPrefsExtractor, elk_cha_prefs_extractor, G_TYPE_OBJECT, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(COW_TYPE_IENTRY_ACCESSOR, l_accessor_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_cha_prefs_extractor_class_init(ElkChaPrefsExtractorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_cha_prefs_extractor_init(ElkChaPrefsExtractor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(elk_cha_prefs_extractor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_cha_prefs_extractor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkChaPrefsExtractor *elk_cha_prefs_extractor_new() {
	ElkChaPrefsExtractor *result = g_object_new(ELK_TYPE_CHA_PREFS_EXTRACTOR, NULL);
	cat_ref_anounce(result);
	return result;
}



/********************* start CowIEntryAccessor implementation *********************/

static gpointer l_get(CowIEntryAccessor *self, gpointer config) {
	if (config==NULL) {
		return NULL;
	}
	ElkPreferencesWo *e_prefs = ELK_PREFERENCES_WO(config);
	ChaPreferencesWo *cha_prefs = (ChaPreferencesWo *) elk_preferences_wo_get_sub(e_prefs, CAT_S(elk_s_editor_prefs));
	return cha_prefs;
}

static gboolean l_set(CowIEntryAccessor *self, gpointer config, gpointer entry) {
	if (config==NULL) {
		return FALSE;
	}
	ElkPreferencesWo *e_prefs = ELK_PREFERENCES_WO(config);
	elk_preferences_wo_set_sub(e_prefs, CAT_S(elk_s_editor_prefs), (GObject *) entry);
	return TRUE;
}


static void l_accessor_iface_init(CowIEntryAccessorInterface *iface) {
	iface->get = l_get;
	iface->set = l_set;
}

/********************* end CowIEntryAccessor implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	ElkChaPrefsExtractor *instance = ELK_CHA_PREFS_EXTRACTOR(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

