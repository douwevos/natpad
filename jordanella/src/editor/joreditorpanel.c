/*
   File:    joreditorpanel.c
   Project: jordanella
   Author:  Douwe Vos
   Date:    Feb 6, 2016
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

#include "joreditorpanel.h"
#include "jorcontexteditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JorEditorPanel"
#include <logging/catlog.h>

struct _JorEditorPanelPrivate {
	JorEditorConnector *connector;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JorEditorPanel, jor_editor_panel, ELK_TYPE_EDITOR_PANEL,
		G_ADD_PRIVATE(JorEditorPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static DraContextEditor *l_create_context_editor(DraEditorPanel *editor_panel);

static void jor_editor_panel_class_init(JorEditorPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraEditorPanelClass *dra_class = DRA_EDITOR_PANEL_CLASS(clazz);
	dra_class->createContextEditor = l_create_context_editor;
}

static void jor_editor_panel_init(JorEditorPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JorEditorPanel *instance = JOR_EDITOR_PANEL(object);
	JorEditorPanelPrivate *priv = jor_editor_panel_get_instance_private(instance);
	cat_unref_ptr(priv->connector);
	G_OBJECT_CLASS(jor_editor_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jor_editor_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JorEditorPanel *jor_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, JorEditorConnector *connector) {
	JorEditorPanel *result = g_object_new(JOR_TYPE_EDITOR_PANEL, NULL);
	cat_ref_anounce(result);
	JorEditorPanelPrivate *priv = jor_editor_panel_get_instance_private(result);
	priv->connector = cat_ref_ptr(connector);
	elk_editor_panel_construct((ElkEditorPanel *) result, panel_owner, document_bin, (DraIConnectorRequestFactory *) connector);
	return result;
}

static DraContextEditor *l_create_context_editor(DraEditorPanel *editor_panel) {
	JorEditorPanelPrivate *priv = jor_editor_panel_get_instance_private((JorEditorPanel *) editor_panel);
	return (DraContextEditor *) jor_context_editor_new(editor_panel, priv->connector);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
