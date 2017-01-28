/*
   File:    chalinelayout.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 23, 2015
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

#ifndef CHALINELAYOUT_H_
#define CHALINELAYOUT_H_

#include "../document/chalinewo.h"
#include "../preferences/chaprefscolormapwo.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CHA_TYPE_LINE_LAYOUT              (cha_line_layout_get_type())
#define CHA_LINE_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_line_layout_get_type(), ChaLineLayout))
#define CHA_LINE_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_LINE_LAYOUT, ChaLineLayoutClass))
#define CHA_IS_LINE_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_LINE_LAYOUT))
#define CHA_IS_LINE_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_LINE_LAYOUT))
#define CHA_LINE_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_LINE_LAYOUT, ChaLineLayoutClass))

typedef struct _ChaLineLayout               ChaLineLayout;
typedef struct _ChaLineLayoutPrivate        ChaLineLayoutPrivate;
typedef struct _ChaLineLayoutClass          ChaLineLayoutClass;


struct _ChaLineLayout {
	GObject parent;
};

struct _ChaLineLayoutClass {
	GObjectClass parent_class;
	void (*applyExtraAttributes)(ChaLineLayout *line_layout, PangoAttrList *pango_attr_list, ChaPrefsColorMapWo *color_map);
};

GType cha_line_layout_get_type();


void cha_line_layout_construct(ChaLineLayout *line_layout);

ChaLineLayout *cha_line_layout_new();

void cha_line_layout_get_width_and_sub_line_count(ChaLineLayout *line_layout, int *width, int *sub_line_count);

void cha_line_layout_mark_dirty(ChaLineLayout *line_layout);

PangoLayout *cha_line_layout_get_pango_layout(ChaLineLayout *line_layout);

void cha_line_layout_update(ChaLineLayout *line_layout, PangoContext *pango_context, ChaPrefsColorMapWo *color_map);
PangoLayout *cha_line_layout_update_pango_layout(ChaLineLayout *line_layout, PangoContext *pango_context);

void cha_line_layout_mark_for_repaint(ChaLineLayout *line_layout);

void cha_line_layout_mark_attributes_dirty(ChaLineLayout *line_layout);


int cha_line_layout_get_layout_version(const ChaLineLayout *line_layout);

gboolean cha_line_layout_set_text(ChaLineLayout *line_layout, CatStringWo *a_text, ChaLineEnd line_end, gboolean do_wrap, int tab_size, int view_width, short font_version);

gboolean cha_line_layout_set_preedit(ChaLineLayout *line_layout, const CatStringWo *preedit_text, PangoAttrList *predit_attrs, int preedit_cursor, int x_cursor_bytes);


const CatStringWo *cha_line_layout_get_text(const ChaLineLayout *line_layout);

gboolean cha_line_Layout_set_y_position(ChaLineLayout *line_layout, long long int ypos);

void cha_line_layout_lock(ChaLineLayout *line_layout, gboolean main_context);
void cha_line_layout_unlock(ChaLineLayout *line_layout);


void cha_line_layout_hold(ChaLineLayout *line_layout);
void cha_line_layout_release(ChaLineLayout *line_layout);


int cha_line_layout_get_selection_version(ChaLineLayout *line_layout);
CatArrayWo *cha_line_layout_get_selection_markings(ChaLineLayout *line_layout);
gboolean cha_line_layout_get_selection_eol(ChaLineLayout *line_layout);

void cha_line_layout_set_selection(ChaLineLayout *line_layout, CatArrayWo *c_selection_attributes, gboolean selection_eol, CatArrayWo *c_selection_markings, int selection_version);

G_END_DECLS

#endif /* CHALINELAYOUT_H_ */
