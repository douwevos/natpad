///*
//   File:    jagclassviewer.c
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Mar 8, 2012
//   Web:     http://www.natpad.net/
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2012 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "jagclassviewer.h"
//#include <caterpillar.h>
//
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "JagClassViewer"
//#include <logging/catlog.h>
//
//struct _JagClassViewerPrivate {
//	void *dummy;
//};
//
//G_DEFINE_TYPE (JagClassViewer, jag_class_viewer, ELK_TYPE_EDITOR)
//
//static gpointer parent_class = NULL;
//
//static void _dispose(GObject *object);
//static void _finalize(GObject *object);
//
//static void jag_class_viewer_class_init(JagClassViewerClass *clazz) {
//	parent_class = g_type_class_peek_parent(clazz);
//	g_type_class_add_private(clazz, sizeof(JagClassViewerPrivate));
//
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = _dispose;
//	object_class->finalize = _finalize;
//
////	ElkEditorClass *sub_clazz = ELK_EDITOR_CLASS(clazz);
//}
//
//static void jag_class_viewer_init(JagClassViewer *instance) {
//	JagClassViewerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_CLASS_VIEWER, JagClassViewerPrivate);
//	instance->priv = priv;
//}
//
//static void _dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	JagClassViewer *instance = JAG_CLASS_VIEWER(object);
////	JagClassViewerPrivate *priv = instance->priv;
//	G_OBJECT_CLASS(parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void _finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//JagClassViewer *jag_class_viewer_new(ElkIService *elk_iservice, CatStringWo *a_markup_hook_key, AstDocument *document) {
//	JagClassViewer *result = g_object_new(JAG_TYPE_CLASS_VIEWER, NULL);
//	cat_ref_anounce(result);
////	JagClassViewerPrivate *priv = result->priv;
//
//	elk_editor_construct(ELK_EDITOR(result), elk_iservice, NULL, a_markup_hook_key, document, FALSE);
//	return result;
//}
//
//
//
//
//
//
//