/*
   File:    dramacromanager.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 21, 2016
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

#include "dramacromanager.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraMacroManager"
#include <logging/catlog.h>

struct _DraMacroManagerPrivate {
	gboolean do_record;
	CatArrayWo *recorded;
	DraEditor *editor;
};

static void l_uow_iface_init(ChaIUowListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraMacroManager, dra_macro_manager, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraMacroManager)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IUOW_LISTENER, l_uow_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_macro_manager_class_init(DraMacroManagerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_macro_manager_init(DraMacroManager *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraMacroManager *instance = DRA_MACRO_MANAGER(object);
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(instance);
	cat_unref_ptr(priv->editor);
	cat_unref_ptr(priv->recorded);
	G_OBJECT_CLASS(dra_macro_manager_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_macro_manager_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraMacroManager *dra_macro_manager_new() {
	DraMacroManager *result = g_object_new(DRA_TYPE_MACRO_MANAGER, NULL);
	cat_ref_anounce(result);
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(result);
	priv->do_record = FALSE;
	priv->editor = NULL;
	priv->recorded = cat_array_wo_new();
//	G_OBJECT_construct((GObject *) result);
	return result;
}


void dra_macro_manager_set_editor(DraMacroManager *macro_manager, DraEditor *new_editor) {
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(macro_manager);
	if (priv->editor == new_editor) {
		return;
	}

	if (priv->editor) {
		cha_editor_remove_uow_listener((ChaEditor *) priv->editor, (ChaIUowListener *) macro_manager);
		cat_unref_ptr(priv->editor);
	}


	if (new_editor) {
		priv->editor = cat_ref_ptr(new_editor);
		cha_editor_add_uow_listener((ChaEditor *) priv->editor, (ChaIUowListener *) macro_manager);
	}
}


void dra_macro_manager_start_recording(DraMacroManager *macro_manager) {
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(macro_manager);
	priv->do_record = TRUE;
	cat_array_wo_clear(priv->recorded);
}

void dra_macro_manager_stop_recording(DraMacroManager *macro_manager) {
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(macro_manager);
	priv->do_record = FALSE;

}

void dra_macro_manager_replay(DraMacroManager *macro_manager) {
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(macro_manager);
	if (priv->do_record) {
		return;
	}
	if (priv->editor==NULL) {
		return;
	}

	CatIIterator *iter = cat_array_wo_iterator(priv->recorded);
	while(cat_iiterator_has_next(iter)) {
		ChaUow *uow = (ChaUow *) cat_iiterator_next(iter);
		cha_editor_run(priv->editor, uow);
	}
	cat_unref_ptr(iter);
}



/********************* start ChaIUowListener implementation *********************/

static void l_uow_event(ChaIUowListener *self, ChaUow *uow, ChaDocumentView *document_view, ChaDocument *document) {
	DraMacroManager *macro_manager = DRA_MACRO_MANAGER(self);
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(macro_manager);
	if (priv->do_record) {
		cat_array_wo_append(priv->recorded, (GObject *) uow);
	}
}


static void l_uow_iface_init(ChaIUowListenerInterface *iface) {
	iface->uowEvent = l_uow_event;
}

/********************* end ChaIUowListener implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraMacroManager *instance = DRA_MACRO_MANAGER(self);
	DraMacroManagerPrivate *priv = dra_macro_manager_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

