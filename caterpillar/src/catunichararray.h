/*
   File:    catunichararray.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 1, 2010
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

#ifndef CATUNICHARARRAY_H_
#define CATUNICHARARRAY_H_

#include "woo/catstringwo.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_UNICHAR_ARRAY            (cat_unichar_array_get_type())
#define CAT_UNICHAR_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_unichar_array_get_type(), CatUnicharArray))
#define CAT_UNICHAR_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_UNICHAR_ARRAY, CatUnicharArrayClass))
#define CAT_IS_UNICHAR_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_UNICHAR_ARRAY))
#define CAT_IS_UNICHAR_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_UNICHAR_ARRAY))
#define CAT_UNICHAR_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_UNICHAR_ARRAY, CatUnicharArrayClass))

struct _CatUnicharArray {
	GInitiallyUnowned parent;
	gunichar *data;
	int data_size;
	int size;
	unsigned int hash;
};

struct _CatUnicharArrayClass {
	GInitiallyUnownedClass parent_class;
};

typedef struct _CatUnicharArray         CatUnicharArray;

typedef struct _CatUnicharArrayClass    CatUnicharArrayClass;


GType cat_unichar_array_get_type(void);

CatUnicharArray *cat_unichar_array_new();

int cat_unichar_array_length(CatUnicharArray *char_array);
void cat_unichar_array_clear(CatUnicharArray *char_array, gboolean full);
gunichar cat_unichar_array_char_at(CatUnicharArray *char_array, int idx);
//int cat_string_compare(CatString *char_array, CatString *with) {
//int cat_string_case_compare(CatString *char_array, CatString *with) {
CatUnicharArray *cat_unichar_array_substring(CatUnicharArray *char_array, int offset, int length);
//void cat_string_append(CatString *this, CatString *other) {
void cat_unichar_array_append_uni_char(CatUnicharArray *char_array, gunichar uch);
//void cat_string_append_decimal(CatString *this, long val) {
//CatString *cat_string_format(CatString *fmt_txt, ...) {
//void cat_string_append_chars(CatString *this, const char *other) {
//void cat_string_append_chars_len(CatString *this, const char *other, const int other_length) {

void cat_unichar_array_remove(CatUnicharArray *char_array, int offset, int length);


gboolean cat_unichar_array_equal(const CatUnicharArray *char_array, const CatUnicharArray *with);
guint cat_unichar_array_hash(CatUnicharArray *char_array);

gchar *cat_unichar_array_to_gchars(const CatUnicharArray *char_array);
CatStringWo *cat_unichar_array_to_string(const CatUnicharArray *char_array);

gint cat_unichar_array_uni_char_last_index_of(CatUnicharArray *txt, gunichar ch);
gint cat_unichar_array_uni_char_index_of(CatUnicharArray *txt, gunichar ch);

/* Remove any leading or heading whitespace characters from the unichar-array.
 */
void cat_unichar_array_trim(CatUnicharArray *char_array);

/* Sets the length of the unichar-array. If the length is greater then the length of the unichar-array itself then
 * nothing is changed otherwise the array will be trimmed to the given length.
 */
void cat_unichar_array_set_length(CatUnicharArray *char_array, int length);

//static gchar cat_char_to_lower(gchar ch) {
//void cat_string_to_lowercase(CatString *txt) {

G_END_DECLS

#endif /* CATUNICHARARRAY_H_ */
