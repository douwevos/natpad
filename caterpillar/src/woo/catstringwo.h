/*
   File:    catstringwo.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 9, 2014
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

#ifndef CATSTRINGWO_H_
#define CATSTRINGWO_H_

#include "catwo.h"
#include "catarraywo.h"

G_BEGIN_DECLS

#define CAT_TYPE_STRING_WO              (cat_string_wo_get_type())
#define CAT_STRING_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_string_wo_get_type(), CatStringWo))
#define CAT_STRING_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_STRING_WO, CatStringWoClass))
#define CAT_IS_STRING_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_STRING_WO))
#define CAT_IS_STRING_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_STRING_WO))
#define CAT_STRING_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_STRING_WO, CatStringWoClass))


typedef struct _CatStringWo               CatStringWo;
typedef struct _CatStringWoPrivate        CatStringWoPrivate;
typedef struct _CatStringWoClass          CatStringWoClass;


struct _CatStringWo {
	CatWo parent;
};

struct _CatStringWoClass {
	CatWoClass parent_class;
};


typedef struct _CatS CatS;

struct _CatS {
	const char *txt;
	CatStringWo *s;
};

#define CAT_S_DEF(t) { t, NULL }
#define CAT_S(f) cat_string_wo_get_static(&f)


CatStringWo *cat_string_wo_get_static(CatS *static_string);

extern CatS cat_s_empty_string;


GType cat_string_wo_get_type();

void cat_string_wo_construct(CatStringWo *string, gboolean editable, gchar *data, int data_size, int length);

gboolean cat_string_wo_to_valid_utf8(CatStringWo *e_string_wo, const char *text, int length);

CatStringWo *cat_string_wo_new() __attribute_warn_unused_result__;
CatStringWo *cat_string_wo_new_with(const gchar *data) __attribute_warn_unused_result__;
CatStringWo *cat_string_wo_new_with_len(const gchar *data, int length) __attribute_warn_unused_result__;
CatStringWo *cat_string_wo_new_anchored(const gchar *data, int length) __attribute_warn_unused_result__;
CatStringWo *cat_string_wo_new_nocopy(gchar *data, int length) __attribute_warn_unused_result__;
const CatStringWo *cat_string_wo_new_data(const gchar *data);
const CatStringWo *cat_string_wo_new_data_len(const gchar *data, int length);
CatStringWo *cat_string_wo_new_sub(const CatStringWo *source, int offset, int length) __attribute_warn_unused_result__;


CatStringWo *cat_string_wo_create_editable(CatStringWo *instance) __attribute_warn_unused_result__;

/** Read functions **/

int cat_string_wo_length(const CatStringWo *string_wo);
gchar cat_string_wo_char_at(const CatStringWo *string_wo, int idx);

const char *cat_string_wo_getchars(const CatStringWo *string_wo);
guint cat_string_wo_hash(const CatStringWo *string_wo);

int cat_string_wo_compare(const CatStringWo *string_wo, const CatStringWo *string_with);
int cat_string_wo_case_compare(const CatStringWo *string_wo, const CatStringWo *string_with);

gboolean cat_string_wo_endswith_chars_len(const CatStringWo *string_wo, const gchar *chars, const int chars_len);
gboolean cat_string_wo_endswith(const CatStringWo *string_wo, const CatStringWo *data);
gboolean cat_string_wo_endswith_char(const CatStringWo *string_wo, const gchar ch);
gboolean cat_string_wo_beginswith_chars_len(const CatStringWo *string_wo, const gchar *chars, const int chars_len);
gboolean cat_string_wo_beginswith(const CatStringWo *string_wo, const CatStringWo *data);
gboolean cat_string_wo_beginswith_char(const CatStringWo *string_wo, const gchar ch);

gint cat_string_wo_index_of(const CatStringWo *string_wo, gchar ch);
gint cat_string_wo_index_of_from(const CatStringWo *string_wo, gchar ch, int from);
gint cat_string_wo_last_index_of(const CatStringWo *string_wo, gchar ch);

gint cat_string_wo_index_of_string(const CatStringWo *string_wo, const CatStringWo *str);
gint cat_string_wo_index_of_string_from(const CatStringWo *string_wo, const CatStringWo *str, int from);


/** Modify functions **/

void cat_string_wo_clear(CatStringWo *e_string_wo);

void cat_string_wo_append(CatStringWo *e_string_wo, const CatStringWo *string_2_append);
void cat_string_wo_append_char(CatStringWo *e_string_wo, gchar char_2_app);
void cat_string_wo_append_chars(CatStringWo *e_string_wo, const char *chars_2_app);
void cat_string_wo_append_chars_len(CatStringWo *e_string_wo, const char *chars_2_app, int length);
int cat_string_wo_append_unichar(CatStringWo *e_string_wo, gunichar unichar);

void cat_string_wo_insert(CatStringWo *e_string_wo, const CatStringWo *to_insert, int offset);

void cat_string_wo_remove_range(CatStringWo *e_string_wo, int offset, int length);

void cat_string_wo_replace_all_char(CatStringWo *e_string_wo, char replace, char with);

void cat_string_wo_replace_range_with(CatStringWo *e_string_wo, int offset, int length, const CatStringWo *replacement);

void cat_string_wo_limit(CatStringWo *e_string_wo, int offset, int length);

void cat_string_wo_set_capacity(CatStringWo *e_string_wo, int min_capacity);


/** util functions **/

void cat_string_wo_append_decimal(CatStringWo *e_string_wo, long long val);
void cat_string_wo_append_hexadecimal(CatStringWo *e_string_wo, unsigned long long val, int digits);
void cat_string_wo_format(CatStringWo *e_string_wo, const char *text, ...);
void cat_string_wo_vformat(CatStringWo *e_string_wo, const char *text, va_list args);

void cat_string_wo_to_uppercase(CatStringWo *e_string_wo);
void cat_string_wo_to_lowercase(CatStringWo *e_string_wo);

int cat_string_wo_count_occurrence(const CatStringWo *instance, char ch);
CatArrayWo *cat_string_wo_split(const CatStringWo *input, char ch, gboolean include_empty) __attribute_warn_unused_result__;

gunichar cat_string_wo_unichar_at(const CatStringWo *string_wo, int *index);
gunichar cat_string_wo_unichar_before(const CatStringWo *string_wo, int *index);
int cat_string_wo_to_utf8_pos(const CatStringWo *instance, int offset);
int cat_string_wo_from_utf8_pos(const CatStringWo *instance, int utf8_offset);

int cat_string_wo_unichar_next_offset(const CatStringWo *string_wo, int offset);

CatStringWo *cat_string_wo_anchor(CatStringWo *instance, int version) __attribute_warn_unused_result__;
CatStringWo *cat_string_wo_get_original(CatStringWo *instance);
gboolean cat_string_wo_is_anchored(const CatStringWo *instance);

gboolean cat_string_wo_equal(const CatStringWo *instance_a, const CatStringWo *instance_b);
CatStringWo *cat_string_wo_clone(const CatStringWo *source, CatCloneDepth clone_depth) __attribute_warn_unused_result__;

G_END_DECLS

#endif /* CATSTRINGWO_H_ */
