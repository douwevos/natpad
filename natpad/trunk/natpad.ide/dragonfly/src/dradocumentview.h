/*
   File:    dradocumentview.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Mar 27, 2015
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

#ifndef DRADOCUMENTVIEW_H_
#define DRADOCUMENTVIEW_H_

#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_DOCUMENT_VIEW              (dra_document_view_get_type())
#define DRA_DOCUMENT_VIEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_document_view_get_type(), DraDocumentView))
#define DRA_DOCUMENT_VIEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_DOCUMENT_VIEW, DraDocumentViewClass))
#define DRA_IS_DOCUMENT_VIEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_DOCUMENT_VIEW))
#define DRA_IS_DOCUMENT_VIEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_DOCUMENT_VIEW))
#define DRA_DOCUMENT_VIEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_DOCUMENT_VIEW, DraDocumentViewClass))


typedef struct _DraDocumentView               DraDocumentView;
typedef struct _DraDocumentViewPrivate        DraDocumentViewPrivate;
typedef struct _DraDocumentViewClass          DraDocumentViewClass;


struct _DraDocumentView {
	ChaDocumentView parent;
};

struct _DraDocumentViewClass {
	ChaDocumentViewClass parent_class;
};


GType dra_document_view_get_type();

DraDocumentView *dra_document_view_new(ChaDocument *document, PangoContext *pango_context, GtkWidget *widget, CatStringWo *a_line_info_key);

void dra_document_view_set_occurrences_rset(DraDocumentView *document_view, ChaOccurrencesResultSet *occurrences_rset);

G_END_DECLS

#endif /* DRADOCUMENTVIEW_H_ */
