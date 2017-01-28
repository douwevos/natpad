/*
   File:    leabigdrawingpanel.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 23, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#include "leabigdrawingpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaBigDrawingPanel"
#include <logging/catlog.h>

struct _LeaBigDrawingPanelPrivate {
	void *dummy;
};


G_DEFINE_TYPE(LeaBigDrawingPanel, lea_big_drawing_panel, LEA_TYPE_PANEL)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_big_drawing_panel_class_init(LeaBigDrawingPanelClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaBigDrawingPanelPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_big_drawing_panel_init(LeaBigDrawingPanel *instance) {
	LeaBigDrawingPanelPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, LEA_TYPE_BIG_DRAWING_PANEL, LeaBigDrawingPanelPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	LeaBigDrawingPanel *instance = LEA_BIG_DRAWING_PANEL(object);
//	LeaBigDrawingPanelPrivate *priv = instance->priv;
	G_OBJECT_CLASS(lea_big_drawing_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_big_drawing_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaBigDrawingPanel *lea_big_drawing_panel_new() {
	LeaBigDrawingPanel *result = g_object_new(LEA_TYPE_BIG_DRAWING_PANEL, NULL);
	cat_ref_anounce(result);
//	LeaBigDrawingPanelPrivate *priv = result->priv;
//	LEA_PANEL_construct((LeaPanel *) result);
	return result;
}









