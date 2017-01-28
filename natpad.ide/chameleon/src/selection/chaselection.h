/*
   File:    chaselection.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 20, 2015
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

#ifndef CHASELECTION_H_
#define CHASELECTION_H_

#include "../layout/chalinelayout.h"
#include "../document/chalinewo.h"
#include "../document/chacursorwo.h"
#include "../document/charevisionwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

CatS cha_selection_cd_plain_text;
CatS cha_selection_cd_rich_text;
CatS cha_selection_cd_block_text;

#define CHA_TYPE_SELECTION              (cha_selection_get_type())
#define CHA_SELECTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_selection_get_type(), ChaSelection))
#define CHA_SELECTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SELECTION, ChaSelectionClass))
#define CHA_IS_SELECTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SELECTION))
#define CHA_IS_SELECTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SELECTION))
#define CHA_SELECTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SELECTION, ChaSelectionClass))


typedef struct _ChaSelection               ChaSelection;
typedef struct _ChaSelectionPrivate        ChaSelectionPrivate;
typedef struct _ChaSelectionClass          ChaSelectionClass;

struct _ChaDocumentView;

struct _ChaSelection {
	GObject parent;
};

struct _ChaSelectionClass {
	GObjectClass parent_class;
	void (*preApply)(ChaSelection *selection, ChaLineLayout *e_line_layout_start, ChaLineLayout *e_line_layout_end, int ps_space_width);
	void (*apply)(ChaSelection *selection, PangoContext *pango_context, ChaLineWo *a_line, ChaLineLayout *e_line_layout, int page_index, int line_index);

	CatStringWo *(*getAsText)(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only);
	CatHashMapWo *(*getForClipboard)(ChaSelection *selection, struct _ChaDocumentView *document_view, gboolean marked_lines_only);
	ChaCursorWo *(*delete)(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision);

	ChaCursorWo *(*paste)(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, CatStringWo *text);

	void (*changeCase)(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, gboolean to_upper);


};


GType cha_selection_get_type();

void cha_selection_construct(ChaSelection *selection, CatAtomicInteger *sequence, ChaCursorWo *cursor);

ChaSelection *cha_selection_new(ChaCursorWo *cursor);

gboolean cha_selection_set_start(ChaSelection *selection, ChaCursorWo *cursor);
gboolean cha_selection_set_end(ChaSelection *selection, ChaCursorWo *cursor);

ChaCursorWo *cha_selection_get_start(ChaSelection *selection);
ChaCursorWo *cha_selection_get_end(ChaSelection *selection);

void cha_selection_get_top(ChaSelection *selection, int *page_index, int *page_line_index, int *cursor_x);
void cha_selection_get_bottom(ChaSelection *selection, int *page_index, int *page_line_index, int *cursor_x);

ChaCursorWo *cha_selection_delete(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision);

G_END_DECLS

#endif /* CHASELECTION_H_ */
