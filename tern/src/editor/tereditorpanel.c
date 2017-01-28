/*
   File:    tereditorpanel.c
   Project: tern
   Author:  Douwe Vos
   Date:    Jul 14, 2015
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

#include "tereditorpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerEditorPanel"
#include <logging/catlog.h>

struct _TerEditorPanelPrivate {
	CatStringWo *a_clazz_name;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerEditorPanel, ter_editor_panel, ELK_TYPE_EDITOR_PANEL,
		G_ADD_PRIVATE(TerEditorPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_editor_panel_class_init(TerEditorPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_editor_panel_init(TerEditorPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	TerEditorPanel *instance = TER_EDITOR_PANEL(object);
//	TerEditorPanelPrivate *priv = ter_editor_panel_get_instance_private(instance);
	G_OBJECT_CLASS(ter_editor_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_editor_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerEditorPanel *ter_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, CatStringWo *a_clazz_name, TerEditorConnector *editor_connector) {
	TerEditorPanel *result = g_object_new(TER_TYPE_EDITOR_PANEL, NULL);
	cat_ref_anounce(result);
	TerEditorPanelPrivate *priv = ter_editor_panel_get_instance_private(result);
	priv->a_clazz_name = cat_ref_ptr(a_clazz_name);
	elk_editor_panel_construct((ElkEditorPanel *) result, panel_owner, document_bin, (DraIConnectorRequestFactory *) editor_connector);
	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TerEditorPanel *instance = TER_EDITOR_PANEL(self);
	TerEditorPanelPrivate *priv = ter_editor_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p clazz_name=%o]", iname, self, priv->a_clazz_name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

