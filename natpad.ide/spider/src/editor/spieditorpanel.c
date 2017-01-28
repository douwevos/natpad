
/*
   File:    spieditorpanel.c
   Project: spider
   Author:  Douwe Vos
   Date:    Jul 18, 2016
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

#include "spieditorpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SpiEditorPanel"
#include <logging/catlog.h>

struct _SpiEditorPanelPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(SpiEditorPanel, spi_editor_panel, ELK_TYPE_EDITOR_PANEL,
		G_ADD_PRIVATE(SpiEditorPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void spi_editor_panel_class_init(SpiEditorPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void spi_editor_panel_init(SpiEditorPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(spi_editor_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(spi_editor_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


SpiEditorPanel *spi_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, SpiEditorConnector *connector) {
	SpiEditorPanel *result = g_object_new(SPI_TYPE_EDITOR_PANEL, NULL);
	cat_ref_anounce(result);
	elk_editor_panel_construct((ElkEditorPanel *) result, panel_owner, document_bin, (DraIConnectorRequestFactory *) connector);
	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p accessor=%O]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
