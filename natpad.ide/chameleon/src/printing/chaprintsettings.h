/*
   File:    chaprintsettings.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 4, 2015
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

#ifndef PRINTING_CHAPRINTSETTINGS_H_
#define PRINTING_CHAPRINTSETTINGS_H_

#include "../preferences/chaprefscolormapwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_PRINT_SETTINGS              (cha_print_settings_get_type())
#define CHA_PRINT_SETTINGS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_print_settings_get_type(), ChaPrintSettings))
#define CHA_PRINT_SETTINGS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PRINT_SETTINGS, ChaPrintSettingsClass))
#define CHA_IS_PRINT_SETTINGS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PRINT_SETTINGS))
#define CHA_IS_PRINT_SETTINGS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PRINT_SETTINGS))
#define CHA_PRINT_SETTINGS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PRINT_SETTINGS, ChaPrintSettingsClass))


typedef struct _ChaPrintSettings               ChaPrintSettings;
typedef struct _ChaPrintSettingsPrivate        ChaPrintSettingsPrivate;
typedef struct _ChaPrintSettingsClass          ChaPrintSettingsClass;


struct _ChaPrintSettings {
	GObject parent;
};

struct _ChaPrintSettingsClass {
	GObjectClass parent_class;
};


GType cha_print_settings_get_type();

ChaPrintSettings *cha_print_settings_new();

void cha_print_settings_set_color_map(ChaPrintSettings *print_settings, ChaPrefsColorMapWo *new_color_map);
ChaPrefsColorMapWo *cha_print_settings_get_color_map(ChaPrintSettings *print_settings);

void cha_print_settings_set_wrap_lines(ChaPrintSettings *print_settings, gboolean wrap_lines);
gboolean cha_print_settings_get_wrap_lines(ChaPrintSettings *print_settings);

void cha_print_settings_set_auto_indent_wrapped_lines(ChaPrintSettings *print_settings, gboolean auto_indent_wrapped_lines);
gboolean cha_print_settings_get_auto_indent_wrapped_lines(ChaPrintSettings *print_settings);


void cha_print_settings_set_font_name(ChaPrintSettings *print_settings, CatStringWo *font_name);
CatStringWo *cha_print_settings_get_font_name(ChaPrintSettings *print_settings);


void cha_print_settings_set_font_size(ChaPrintSettings *print_settings, double font_size);
double cha_print_settings_get_font_size(ChaPrintSettings *print_settings);


void cha_print_settings_set_font_is_bold(ChaPrintSettings *print_settings, gboolean is_bold);
gboolean cha_print_settings_get_font_is_bold(ChaPrintSettings *print_settings);

void cha_print_settings_set_font_is_italic(ChaPrintSettings *print_settings, gboolean is_italic);
gboolean cha_print_settings_get_font_is_italic(ChaPrintSettings *print_settings);




void cha_print_settings_set_print_line_numbers(ChaPrintSettings *print_settings, gboolean print_line_numbers);
gboolean cha_print_settings_print_line_numbers(ChaPrintSettings *print_settings);

void cha_print_settings_set_line_number_color(ChaPrintSettings *print_settings, GdkRGBA color);
GdkRGBA cha_print_settings_get_line_number_color(ChaPrintSettings *print_settings);


void cha_print_settings_set_print_markup(ChaPrintSettings *print_settings, gboolean do_print_markup);
gboolean cha_print_settings_get_print_markup(ChaPrintSettings *print_settings);


void cha_print_settings_set_print_header_and_footer(ChaPrintSettings *print_settings, gboolean do_print_header_and_footer);
gboolean cha_print_settings_get_print_header_and_footer(ChaPrintSettings *print_settings);


void cha_print_settings_set_header_text(ChaPrintSettings *print_settings, CatStringWo *header_text);
CatStringWo *cha_print_settings_get_header_text(ChaPrintSettings *print_settings);


void cha_print_settings_set_footer_text(ChaPrintSettings *print_settings, CatStringWo *footer_text);
CatStringWo *cha_print_settings_get_footer_text(ChaPrintSettings *print_settings);

G_END_DECLS

#endif /* PRINTING_CHAPRINTSETTINGS_H_ */
