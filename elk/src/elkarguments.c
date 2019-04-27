/*
   File:    elkarguments.c
   Project: elk
   Author:  Douwe Vos
   Date:    Dec 25, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "elkarguments.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkArguments"
#include <logging/catlog.h>

G_DEFINE_TYPE(ElkArguments, elk_arguments, G_TYPE_OBJECT)

static void l_dispose(GObject *object);

static void elk_arguments_class_init(ElkArgumentsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void elk_arguments_init(ElkArguments *obj) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%o", object);
	ElkArguments *instance = ELK_ARGUMENTS(object);
	cat_unref_ptr(instance->documents_to_load);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_arguments_parent_class)->finalize(object);
	cat_log_detail("disposed:%p", object);
}


ElkArguments *elk_arguments_new() {
	ElkArguments *result = g_object_new(ELK_TYPE_ARGUMENTS, NULL);
	cat_ref_anounce(result);
	result->documents_to_load = cat_array_wo_new();
	return result;
}


CatStringWo *elk_arguments_url_decode(CatStringWo *a_url_document) {
	cat_log_debug("encoding url: %s", cat_string_wo_getchars(a_url_document));
	int in_idx, out_idx;
	CatStringWo *e_out = cat_string_wo_new();
	char buf[3];
	buf[2] = '\0';

	int len = cat_string_wo_length(a_url_document);

	for(in_idx = out_idx = 0; in_idx < len;) {
		char ch = cat_string_wo_char_at(a_url_document, in_idx++);
		switch(ch) {
			case '%': {
				if(in_idx + 2 < len) {
					buf[0] = cat_string_wo_char_at(a_url_document, in_idx++);
					buf[1] = cat_string_wo_char_at(a_url_document, in_idx++);
					int val = strtoull(buf, NULL, 16);
					cat_log_detail("val=%d\n", val);
					cat_string_wo_append_char(e_out, (char) val);
				} else {
					in_idx += 2;
				}
			} break;
			case '+': {
				cat_string_wo_append_char(e_out, ' ');
			} break;
			default: {
				cat_string_wo_append_char(e_out, ch);
			} break;
		}
	}
	return e_out;
}

CatS elk_arg_file = CAT_S_DEF("file://");

void elk_arguments_parse_and_add_document(ElkArguments *arguments, CatStringWo *a_arg) {
	cat_log_debug("testing for url");
	int idx = cat_string_wo_index_of_string(a_arg, CAT_S(elk_arg_file));
	cat_log_debug("idx=%d", idx);
	if (idx>=0) {
		CatStringWo *e_url_document = cat_string_wo_new_sub(a_arg, idx, cat_string_wo_length(a_arg)-idx);
		CatStringWo *e_enc_url_document = elk_arguments_url_decode(e_url_document);
		e_enc_url_document = cat_string_wo_anchor(e_enc_url_document, 0);
		cat_array_wo_append(arguments->documents_to_load, G_OBJECT(e_enc_url_document));
	} else {
		cat_array_wo_append(arguments->documents_to_load, G_OBJECT(a_arg));
	}
}
