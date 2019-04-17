/*
   File:    chapreferenceswo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 19, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef CHAPREFERENCESWO_H_
#define CHAPREFERENCESWO_H_

#include <caterpillar.h>
#include "chaprefscolormapwo.h"

G_BEGIN_DECLS

#define CHA_TYPE_PREFERENCES_WO              (cha_preferences_wo_get_type())
#define CHA_PREFERENCES_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_preferences_wo_get_type(), ChaPreferencesWo))
#define CHA_PREFERENCES_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PREFERENCES_WO, ChaPreferencesWoClass))
#define CHA_IS_PREFERENCES_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PREFERENCES_WO))
#define CHA_IS_PREFERENCES_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PREFERENCES_WO))
#define CHA_PREFERENCES_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PREFERENCES_WO, ChaPreferencesWoClass))


typedef struct _ChaPreferencesWo               ChaPreferencesWo;
typedef struct _ChaPreferencesWoPrivate        ChaPreferencesWoPrivate;
typedef struct _ChaPreferencesWoClass          ChaPreferencesWoClass;


struct _ChaPreferencesWo {
	CatWo parent;
};

struct _ChaPreferencesWoClass {
	CatWoClass parent_class;
};


GType cha_preferences_wo_get_type();

void cha_preferences_wo_construct(ChaPreferencesWo *prefs);
ChaPreferencesWo *cha_preferences_wo_new();

gboolean cha_preferences_wo_get_blink_cursor(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_show_whitespace(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_wrap_lines(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_show_line_numbers(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_limit_cursor(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_highlight_current_line(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_block_selection(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_deprecated_scrolling(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_escape_key(const ChaPreferencesWo *prefs);
int cha_preferences_wo_get_tab_size(const ChaPreferencesWo *prefs);

CatStringWo *cha_preferences_wo_get_font_name(const ChaPreferencesWo *prefs);
gboolean cha_preferences_wo_get_big_mode_force_ascii(const ChaPreferencesWo *prefs);
CatStringWo *cha_preferences_wo_get_big_mode_font_name(const ChaPreferencesWo *prefs);
int cha_preferences_wo_get_font_weight(const ChaPreferencesWo *prefs);

ChaPrefsColorMapWo *cha_preferences_wo_get_color_map(const ChaPreferencesWo *prefs);
CatHashMapWo *cha_preferences_wo_get_template_map(const ChaPreferencesWo *prefs);



void cha_preferences_wo_set_blink_cursor(ChaPreferencesWo *e_prefs, gboolean blink_cursor);
void cha_preferences_wo_set_show_whitespace(ChaPreferencesWo *e_prefs, gboolean show_whitespace);
void cha_preferences_wo_set_wrap_lines(ChaPreferencesWo *e_prefs, gboolean wrap_lines);
void cha_preferences_wo_set_show_line_numbers(ChaPreferencesWo *e_prefs, gboolean show_line_numbers);
void cha_preferences_wo_set_limit_cursor(ChaPreferencesWo *e_prefs, gboolean limit_cursor);
void cha_preferences_wo_set_highlight_current_line(ChaPreferencesWo *e_prefs, gboolean highlight_current_line);
void cha_preferences_wo_set_block_selection(ChaPreferencesWo *e_prefs, gboolean block_selection);
void cha_preferences_wo_set_deprecated_scrolling(ChaPreferencesWo *e_prefs, gboolean deprecated_scrolling);
void cha_preferences_wo_set_tab_size(ChaPreferencesWo *e_prefs, int tab_size);

void cha_preferences_wo_set_font_name(ChaPreferencesWo *e_prefs, CatStringWo *font_name);
void cha_preferences_wo_set_big_mode_force_ascii(ChaPreferencesWo *e_prefs, gboolean force_ascii);
void cha_preferences_wo_set_big_mode_font_name(ChaPreferencesWo *e_prefs, CatStringWo *font_name);
void cha_preferences_wo_set_font_weight(ChaPreferencesWo *e_prefs, int font_weight);

ChaPrefsColorMapWo *cha_preferences_wo_get_editable_color_map(ChaPreferencesWo *e_prefs);
CatHashMapWo *cha_preferences_wo_get_editable_template_map(ChaPreferencesWo *e_prefs);

CAT_WO_BASE_H(ChaPreferencesWo, cha_preferences_wo)

G_END_DECLS

#endif /* CHAPREFERENCESWO_H_ */
