/*
   File:    chaprintsettings.c
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

#include "chaprintsettings.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPrintSettings"
#include <logging/catlog.h>

struct _ChaPrintSettingsPrivate {
	ChaPrefsColorMapWo *color_map;
	gboolean wrap_lines;
	gboolean auto_indent_wrapped_lines;
	CatStringWo *font_name;
	double font_size;
	gboolean font_is_bold;
	gboolean font_is_italic;
	gboolean print_line_numbers;
	GdkRGBA line_number_color;
	gboolean print_markup;

	gboolean print_header_and_footer;
	CatStringWo *header_text;
	CatStringWo *footer_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPrintSettings, cha_print_settings, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaPrintSettings)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_print_settings_class_init(ChaPrintSettingsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_print_settings_init(ChaPrintSettings *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaPrintSettings *instance = CHA_PRINT_SETTINGS(object);
//	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(instance);
	G_OBJECT_CLASS(cha_print_settings_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_print_settings_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPrintSettings *cha_print_settings_new() {
	ChaPrintSettings *result = g_object_new(CHA_TYPE_PRINT_SETTINGS, NULL);
	cat_ref_anounce(result);
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(result);
	priv->color_map = cha_prefs_color_map_wo_new();
	priv->wrap_lines = TRUE;
	priv->auto_indent_wrapped_lines = FALSE;
	priv->font_name = (CatStringWo *) cat_string_wo_new_data("sans");
	priv->font_size = 10;
	priv->font_is_bold = FALSE;
	priv->font_is_italic = FALSE;
	priv->print_line_numbers = TRUE;
	priv->print_markup = TRUE;
	priv->print_header_and_footer = TRUE;
	priv->header_text = (CatStringWo *) cat_string_wo_new_data("Page [%page%] from [%page-count%]");;
	priv->footer_text = (CatStringWo *) cat_string_wo_new_data("Page [%page%] from [%page-count%]");;
	return result;
}


void cha_print_settings_set_color_map(ChaPrintSettings *print_settings, ChaPrefsColorMapWo *new_color_map) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	cat_ref_swap(priv->color_map, new_color_map);
}


ChaPrefsColorMapWo *cha_print_settings_get_color_map(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->color_map;
}



void cha_print_settings_set_wrap_lines(ChaPrintSettings *print_settings, gboolean wrap_lines) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->wrap_lines = wrap_lines;
}

gboolean cha_print_settings_get_wrap_lines(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->wrap_lines;
}



void cha_print_settings_set_auto_indent_wrapped_lines(ChaPrintSettings *print_settings, gboolean auto_indent_wrapped_lines) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->auto_indent_wrapped_lines = auto_indent_wrapped_lines;
}

gboolean cha_print_settings_get_auto_indent_wrapped_lines(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->auto_indent_wrapped_lines;
}

void cha_print_settings_set_font_name(ChaPrintSettings *print_settings, CatStringWo *font_name) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	cat_ref_swap(priv->font_name, font_name);
}


CatStringWo *cha_print_settings_get_font_name(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->font_name;
}



void cha_print_settings_set_font_size(ChaPrintSettings *print_settings, double font_size) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->font_size = font_size;
}


double cha_print_settings_get_font_size(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->font_size;
}


void cha_print_settings_set_font_is_bold(ChaPrintSettings *print_settings, gboolean is_bold) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->font_is_bold = is_bold;
}


gboolean cha_print_settings_get_font_is_bold(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->font_is_bold;
}

void cha_print_settings_set_font_is_italic(ChaPrintSettings *print_settings, gboolean is_italic) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->font_is_italic = is_italic;
}


gboolean cha_print_settings_get_font_is_italic(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->font_is_italic;
}



void cha_print_settings_set_print_line_numbers(ChaPrintSettings *print_settings, gboolean print_line_numbers) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->print_line_numbers = print_line_numbers;
}


gboolean cha_print_settings_print_line_numbers(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->print_line_numbers;
}



void cha_print_settings_set_line_number_color(ChaPrintSettings *print_settings, GdkRGBA line_number_color) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->line_number_color = line_number_color;
}

GdkRGBA cha_print_settings_get_line_number_color(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->line_number_color;
}



void cha_print_settings_set_print_markup(ChaPrintSettings *print_settings, gboolean do_print_markup) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->print_markup = do_print_markup;
}

gboolean cha_print_settings_get_print_markup(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->print_markup;
}






void cha_print_settings_set_print_header_and_footer(ChaPrintSettings *print_settings, gboolean do_print_header_and_footer) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	priv->print_header_and_footer = do_print_header_and_footer;
}

gboolean cha_print_settings_get_print_header_and_footer(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->print_header_and_footer;
}



void cha_print_settings_set_header_text(ChaPrintSettings *print_settings, CatStringWo *header_text) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	cat_ref_sink_swap(priv->header_text, header_text);
}

CatStringWo *cha_print_settings_get_header_text(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->header_text;
}



void cha_print_settings_set_footer_text(ChaPrintSettings *print_settings, CatStringWo *footer_text) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	cat_ref_sink_swap(priv->footer_text, footer_text);
}

CatStringWo *cha_print_settings_get_footer_text(ChaPrintSettings *print_settings) {
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(print_settings);
	return priv->footer_text;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPrintSettings *instance = CHA_PRINT_SETTINGS(self);
	ChaPrintSettingsPrivate *priv = cha_print_settings_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p auto_indent_wrapped_lines=%d]", iname, self, priv->auto_indent_wrapped_lines);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
