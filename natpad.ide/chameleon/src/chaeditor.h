/*
   File:    chaeditor.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 23, 2015
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

#ifndef CHAEDITOR_H_
#define CHAEDITOR_H_

#include "document/chadocument.h"
#include "layout/chadocumentview.h"
#include "uow/chauow.h"
#include "uow/chaiuowlistener.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_EDITOR              (cha_editor_get_type())
#define CHA_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_editor_get_type(), ChaEditor))
#define CHA_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_EDITOR, ChaEditorClass))
#define CHA_IS_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_EDITOR))
#define CHA_IS_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_EDITOR))
#define CHA_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_EDITOR, ChaEditorClass))

typedef enum {
	CHA_EDITOR_FLAG_DEFAULT_KEY_HANDLING             = 1 << 1,
	CHA_EDITOR_FLAG_DEFAULT_MOUSE_HANDLING           = 1 << 2,
	CHA_EDITOR_FLAG_NO_DEFAULT_BLINKER               = 1 << 3
} ChaEditorFlags;

typedef struct _ChaEditor               ChaEditor;
typedef struct _ChaEditorPrivate        ChaEditorPrivate;
typedef struct _ChaEditorClass          ChaEditorClass;


struct _ChaSearchService;

struct _ChaEditor {
	GtkWidget parent;
};

struct _ChaEditorClass {
	GtkWidgetClass parent_class;
	ChaDocumentView *(*initializeDocumentView)(ChaEditor *editor, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget);
	void (*markerClicked)(ChaEditor *editor, ChaLineLocationWo *location, long long y_marker_view);
	void (*markerOver)(ChaEditor *editor, ChaLineLocationWo *location, long long y_marker_view);
	void (*markerOut)(ChaEditor *editor);
};


GType cha_editor_get_type();


void cha_editor_construct(ChaEditor *editor, ChaDocument *document, ChaEditorFlags flags);

ChaEditor *cha_editor_new(ChaDocument *document);

ChaDocument *cha_editor_get_document(ChaEditor *editor);
ChaDocumentView *cha_editor_get_document_view(ChaEditor *editor);

void cha_editor_set_preferences(ChaEditor *editor, ChaPreferencesWo *a_prefs);
ChaPreferencesWo *cha_editor_get_preferences(const ChaEditor *editor);

void cha_editor_set_search_service(ChaEditor *editor, struct _ChaSearchService *search_service);
struct _ChaSearchService *cha_editor_get_search_service(ChaEditor *editor);

gboolean cha_editor_set_cursor_blink_up(ChaEditor *editor, gboolean blink_up);

gboolean cha_editor_im_filter_keypress(ChaEditor *editor, GdkEventKey *eev);
void cha_editor_im_reset_context(ChaEditor *editor);


void cha_editor_run(ChaEditor *editor, ChaUow *uow);

void cha_editor_notify_uow_ran(ChaEditor *editor, ChaUow *uow);

void cha_editor_add_uow_listener(ChaEditor *editor, ChaIUowListener *listener);
void cha_editor_remove_uow_listener(ChaEditor *editor, ChaIUowListener *listener);

G_END_DECLS

#endif /* CHAEDITOR_H_ */
