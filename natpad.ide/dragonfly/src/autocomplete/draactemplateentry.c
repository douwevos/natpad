/*
   File:    draactemplateentry.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 20, 2015
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

#include "draactemplateentry.h"
#include "draaccontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraAcTemplateEntry"
#include <logging/catlog.h>

struct _DraAcTemplateEntryPrivate {
	CatStringWo *a_template_label;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraAcTemplateEntry, dra_ac_template_entry, DRA_TYPE_AC_ENTRY,
		G_ADD_PRIVATE(DraAcTemplateEntry)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run(DraAcEntry *entry, struct _DraAcContext *ac_context);

static void dra_ac_template_entry_class_init(DraAcTemplateEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAcEntryClass *ac_class = DRA_AC_ENTRY_CLASS(clazz);
	ac_class->run = l_run;
}

static void dra_ac_template_entry_init(DraAcTemplateEntry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraAcTemplateEntry *instance = DRA_AC_TEMPLATE_ENTRY(object);
//	DraAcTemplateEntryPrivate *priv = dra_ac_template_entry_get_instance_private(instance);
	G_OBJECT_CLASS(dra_ac_template_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_ac_template_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraAcTemplateEntry *dra_ac_template_entry_new(CatStringWo *a_txt, CatStringWo *a_template_label) {
	DraAcTemplateEntry *result = g_object_new(DRA_TYPE_AC_TEMPLATE_ENTRY, NULL);
	cat_ref_anounce(result);
	DraAcTemplateEntryPrivate *priv = dra_ac_template_entry_get_instance_private(result);
	dra_ac_entry_construct((DraAcEntry *) result, a_txt);
	priv->a_template_label = cat_string_wo_clone(a_template_label, CAT_CLONE_DEPTH_NONE);
	return result;
}


static void l_run(DraAcEntry *entry, struct _DraAcContext *ac_context) {
	DraAcTemplateEntryPrivate *priv = dra_ac_template_entry_get_instance_private((DraAcTemplateEntry *) entry);
	ChaEditor *editor = dra_ac_context_get_editor(ac_context);
	ChaDocumentView *document_view = cha_editor_get_document_view(editor);
	ChaCursorWo *a_start_cursor = dra_ac_context_get_start_cursor(ac_context);
	ChaCursorWo *a_end_cursor = dra_ac_context_get_filter_cursor(ac_context);
	cha_document_view_set_plain_selection(document_view, a_start_cursor, a_end_cursor);
	ChaUow *uow = (ChaUow *) cha_uow_insert_template_new(priv->a_template_label);
	cha_editor_run((ChaEditor *) editor, uow);
	cat_unref_ptr(uow);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraAcTemplateEntry *instance = DRA_AC_TEMPLATE_ENTRY(self);
	DraAcTemplateEntryPrivate *priv = dra_ac_template_entry_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p template_label=%o]", iname, self, priv->a_template_label);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
