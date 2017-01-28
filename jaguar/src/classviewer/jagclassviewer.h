///*
//   File:    jagclassviewer.h
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
//
//#ifndef JAGCLASSVIEWER_H_
//#define JAGCLASSVIEWER_H_
//
//#include <caterpillar.h>
//#include <leafhopper.h>
//#include <elk.h>
//#include <elkdeprecated.h>
//#include <astarte.h>
//#include <worm.h>
//#include <moose.h>
//
//G_BEGIN_DECLS
//
//#define JAG_TYPE_CLASS_VIEWER              (jag_class_viewer_get_type())
//#define JAG_CLASS_VIEWER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_class_viewer_get_type(), JagClassViewer))
//#define JAG_CLASS_VIEWER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_CLASS_VIEWER, JagClassViewerClass))
//#define JAG_IS_CLASS_VIEWER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_CLASS_VIEWER))
//#define JAG_IS_CLASS_VIEWER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_CLASS_VIEWER))
//#define JAG_CLASS_VIEWER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_CLASS_VIEWER, JagClassViewerClass))
//#define JAG_CLASS_VIEWER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_CLASS_VIEWER, JagClassViewerPrivate))
//
//typedef struct _JagClassViewer               JagClassViewer;
//typedef struct _JagClassViewerPrivate        JagClassViewerPrivate;
//typedef struct _JagClassViewerClass          JagClassViewerClass;
//
//
//struct _JagClassViewer {
//	ElkEditor parent;
//	JagClassViewerPrivate *priv;
//};
//
//struct _JagClassViewerClass {
//	ElkEditorClass parent_class;
//};
//
//
//GType jag_class_viewer_get_type();
//
//JagClassViewer *jag_class_viewer_new(ElkIService *elk_iservice, CatStringWo *a_markup_hook_key, AstDocument *document);
//
//G_END_DECLS
//
//
//#endif /* JAGCLASSVIEWER_H_ */
