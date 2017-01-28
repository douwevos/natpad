/*
   File:    catutf8.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 26, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#include "catutf8.h"
#include "../catunichararray.h"
#include "../catlib.h"


void cat_utf8_flip_case(CatStringWo *e_text) {
	const char *source = cat_string_wo_getchars(e_text);

	const gchar *text_in = source;
	const gchar *text_end = source+cat_string_wo_length(e_text);


	gboolean not_end = TRUE;

	CatUnicharArray *tmp_buf = cat_unichar_array_new();
	cat_ref_sink_ptr(tmp_buf);
	gunichar character = 0;

	while(not_end) {
		const gchar *text_next = text_in+1;
		if (*text_in) {
			text_next = g_utf8_find_next_char(text_in, text_end+1);
		}
		if (text_next<=text_end && text_next!=NULL) {
			character = g_utf8_get_char_validated(text_in, text_end-text_in);
			text_in = text_next;
			if (g_unichar_isupper(character)) {
				character = g_unichar_tolower(character);
			} else if (g_unichar_islower(character)) {
				character = g_unichar_toupper(character);
			}
			cat_unichar_array_append_uni_char(tmp_buf, character);
		} else {
			character = 0;
			not_end = FALSE;
		}
	}
	CatStringWo *fixed_utf8_string = cat_unichar_array_to_string(tmp_buf);
	cat_string_wo_clear(e_text);
	cat_string_wo_append_chars_len(e_text, cat_string_wo_getchars(fixed_utf8_string), cat_string_wo_length(fixed_utf8_string));
	cat_unref_ptr(fixed_utf8_string);
}
