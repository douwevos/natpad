/*
   File:    chadocumentview.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 31, 2015
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

#ifndef CHADOCUMENTVIEW_H_
#define CHADOCUMENTVIEW_H_

#include "chaeditmode.h"
#include "chapagelayout.h"
#include "chalinelayout.h"
#include "chasublinecache.h"
#include "chaidocumentviewlistener.h"
#include "../document/chadocument.h"
#include "../preferences/chapreferenceswo.h"
#include "../selection/chaselection.h"
#include "../selection/chaplainselection.h"
#include "../selection/chablockselection.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CHA_TYPE_DOCUMENT_VIEW              (cha_document_view_get_type())
#define CHA_DOCUMENT_VIEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_document_view_get_type(), ChaDocumentView))
#define CHA_DOCUMENT_VIEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_DOCUMENT_VIEW, ChaDocumentViewClass))
#define CHA_IS_DOCUMENT_VIEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_DOCUMENT_VIEW))
#define CHA_IS_DOCUMENT_VIEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_DOCUMENT_VIEW))
#define CHA_DOCUMENT_VIEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_DOCUMENT_VIEW, ChaDocumentViewClass))

typedef struct _ChaDocumentView               ChaDocumentView;
typedef struct _ChaDocumentViewPrivate        ChaDocumentViewPrivate;
typedef struct _ChaDocumentViewClass          ChaDocumentViewClass;

typedef struct _ChaDocumentViewContext ChaDocumentViewContext;

struct _ChaDocumentViewContext {
	int def_char_width;
	int ps_space_width;
	int digit_width;
	int tab_size;
	PangoTabArray *tab_array;
	gboolean is_mono_space;
	gboolean wrap_lines;
	gboolean show_line_numbers;
	gboolean show_whitespace;
	gboolean limit_cursor;
	int line_nr_view_width;
	short font_version;

	/* The width in pixels of the text. If -1 then text is not wrapped.
	 */
	int text_layout_width;

	/*
	 * The number of pixels visible on the screen. If wrap mode is of the value should be the same as the text_layout_width.
	 */
	int text_view_width;

	int line_height;
};

struct _ChaDocumentView {
	GObject parent;
};

struct _ChaDocumentViewClass {
	GObjectClass parent_class;

	void (*invalidateRevision)(ChaDocumentView *document_view, ChaRevisionWo *a_rev);

	ChaSubLineCache *(*createSubLineCache)(ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_idx);
	ChaLineLayout *(*createLineLayout)(ChaDocumentView *document_view);
	void (*onInvalidateLine)(ChaDocumentView *document_view, int page_index, int page_line_index, ChaLineWo *a_line, ChaLineLayout *line_layout);

	CatStringWo *(*createRichText)(ChaDocumentView *document_view, ChaPageWo *a_page, ChaLineWo *a_line, int l_index, int r_index);

};


GType cha_document_view_get_type();

void cha_document_view_construct(ChaDocumentView *document_view, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget);

void cha_document_view_set_in_scroll(ChaDocumentView *document_view, gboolean in_scrollt);


ChaDocumentView *cha_document_view_new(ChaDocument *document, PangoContext *pango_context, GtkWidget *widget);

void cha_document_view_set_preferences(ChaDocumentView *document_view, ChaPreferencesWo *a_preferences);
ChaPrefsColorMapWo *cha_document_view_get_color_map(ChaDocumentView *document_view);

void cha_document_view_set_edit_mode(ChaDocumentView *document_view, ChaEditMode edit_mode);

ChaEditMode cha_document_view_get_edit_mode(ChaDocumentView *document_view);


void cha_document_view_add_listener(ChaDocumentView *document_view, ChaIDocumentViewListener *listener);
void cha_document_view_remove_listener(ChaDocumentView *document_view, ChaIDocumentViewListener *listener);
void cha_document_view_notify_selection_changed(ChaDocumentView *document_view);

CatStringWo *cha_document_view_get_slot_key(ChaDocumentView *document_view, int *slot_index);

/**
 * Get multiple settings from the given document_view. All parameter are optional except for document_view.
 */
void cha_document_view_get_values(ChaDocumentView *document_view, gboolean *wrap_lines, gboolean *is_mono_space, gboolean *is_big_mode, int *def_char_width, int *line_height);

const ChaDocumentViewContext cha_document_view_get_context(ChaDocumentView *document_view);

int cha_document_view_get_visible_lines(ChaDocumentView *document_view);

void cha_document_view_set_adjustments(ChaDocumentView *document_view, GtkAdjustment *hadjustment, GtkAdjustment *vadjustment);

gboolean cha_document_view_layout_page(ChaDocumentView *document_view, ChaPageWo *a_page, ChaPageLayoutContext *page_layout_context, ChaPageLayout *page_layout, gboolean test_for_concurrent_update);
gboolean cha_document_view_page_set_layout(ChaDocumentView *document_view, ChaPageWo *a_page, ChaPageLayout *a_new_page_layout, ChaPageLayoutContext *page_layout_context, gboolean refresh);

void cha_document_view_set_has_focus(ChaDocumentView *document_view, gboolean has_focus);

long long int cha_document_view_set_view_y(ChaDocumentView *document_view, long long int y_pos);
long long int cha_document_view_set_view_x(ChaDocumentView *document_view, long long int x_pos);

long long int cha_document_view_get_view_y(ChaDocumentView *document_view);
long long int cha_document_view_get_view_x(ChaDocumentView *document_view);

int cha_document_view_get_view_height(ChaDocumentView *document_view);
long long int cha_document_view_get_layout_height(ChaDocumentView *document_view);

void cha_document_view_move_view_to_focus(ChaDocumentView *document_view, gboolean do_center);
void cha_document_view_move_view_vertical(ChaDocumentView *document_view, int lines);

void cha_document_view_set_view_size(ChaDocumentView *document_view, int view_width, int view_height);

int cha_document_view_get_marked_layout_x_cursor(ChaDocumentView *document_view);
void cha_document_view_mark_layout_x_cursor(ChaDocumentView *document_view, ChaRevisionWo *revision, ChaCursorWo *cursor);

ChaLineLocationWo *cha_document_view_cursor_at_marker(ChaDocumentView *document_view, int x, int y, long long *view_line_y);
ChaCursorWo *cha_document_view_cursor_at_xy(ChaDocumentView *document_view, int x, int y);

gboolean cha_document_view_get_cursor_screen_location(ChaDocumentView *document_view, int *cursor_x, int *cursor_y, int *line_height);

gboolean cha_document_view_set_cursor_blink_up(ChaDocumentView *document_view, gboolean blink_up);

ChaSelection *cha_document_view_get_selection(ChaDocumentView *document_view);
ChaSelection *cha_document_view_get_or_create_selection(ChaDocumentView *document_view, ChaRevisionWo *a_revision, ChaCursorWo *cursor, gboolean block);
void cha_document_view_clear_selection(ChaDocumentView *document_view, ChaRevisionWo *a_revision);
gboolean cha_document_view_remove_selection(ChaDocumentView *document_view);

gboolean cha_document_view_set_plain_selection(ChaDocumentView *document_view, ChaCursorWo *start, ChaCursorWo *end);


ChaLineLayout *cha_document_view_get_line_layout_ref(ChaDocumentView *document_view, ChaLineWo *a_line);

ChaDocument *cha_document_view_get_document(ChaDocumentView *document_view);
ChaRevisionWo *cha_document_view_get_revision(ChaDocumentView *document_view);
PangoContext *cha_document_view_get_pango_context(ChaDocumentView *document_view);

gboolean cha_document_view_check_cache_boundaries(ChaDocumentView *document_view);

void cha_document_view_invalidate_lines(ChaDocumentView *document_view);
void cha_document_view_draw(ChaDocumentView *document_view, cairo_t *cr);


void cha_document_view_set_preedit(ChaDocumentView *document_view, const CatStringWo *predit_text, PangoAttrList *preedit_attrs, int cursor_pos);
const CatStringWo *cha_document_view_get_preedit(ChaDocumentView *document_view);


G_END_DECLS

#endif /* CHADOCUMENTVIEW_H_ */
