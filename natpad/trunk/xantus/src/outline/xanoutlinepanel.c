/*
   File:    xanoutlinepanel.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 12, 2017
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

#include "xanoutlinepanel.h"
#include "dom/xandomnode.h"
#include <turtle.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "XanOutlinePanel"
#include <logging/catlog.h>

struct _XanOutlinePanelPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(XanOutlinePanel, xan_outline_panel, LEA_TYPE_PANEL,
		G_ADD_PRIVATE(XanOutlinePanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void xan_outline_panel_class_init(XanOutlinePanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void xan_outline_panel_init(XanOutlinePanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	XanOutlinePanel *instance = XAN_OUTLINE_PANEL(object);
//	XanOutlinePanelPrivate *priv = xan_outline_panel_get_instance_private(instance);
	G_OBJECT_CLASS(xan_outline_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_outline_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


XanOutlinePanel *xan_outline_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, XanEditorConnector *connector) {
	XanOutlinePanel *result = g_object_new(XAN_TYPE_OUTLINE_PANEL, NULL);
	cat_ref_anounce(result);
	XanOutlinePanelPrivate *priv = xan_outline_panel_get_instance_private(result);
	XanDomNode *dom_root = xan_dom_node_new();
	TurViewer *viewer = tur_viewer_new((TurINode *) dom_root);
	lea_panel_construct((LeaPanel *) result, panel_owner, NULL, cat_string_wo_new_with("Xantus"), viewer);


	DraPanelOwner *dra_panel_owner = DRA_PANEL_OWNER(panel_owner);
	DraConnectorMap *connector_map = dra_panel_owner_get_connector_map(dra_panel_owner);


	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	XanOutlinePanel *instance = XAN_OUTLINE_PANEL(self);
	XanOutlinePanelPrivate *priv = xan_outline_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
