/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2010 Gerard Visser.

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

#include <gvlib/errors.h>
#include <gvlcodeparsing/elements.h>
#include <gvlcodeparsing/ElemInfo.h>
#include "cheExtraKeyWords.h"
#include "../checodes.h"

static StringElemInfoMap keyWords = NULL;

static void cleanUp(void);
static StringElemInfoMap get(void);
static void init(void);

const struct cheExtraKeyWordsStruct CheExtraKeyWords = {
  cleanUp,
  get,
  init
};

static void cleanUp(void) {
  ___BTPUSH;
  keyWords->call->dump(keyWords);
  keyWords = NULL;
  ___BTPOP;
}

static StringElemInfoMap get(void) {
  return keyWords;
}

static void init(void) {
  ___BTPUSH;
  keyWords = newStringElemInfoMap(61, 1.0);
  keyWords->call->add(keyWords, elem_bool, newElemInfo(elem_bool, GVLCP_CODE_CHAR, 0));
  keyWords->call->add(keyWords, elem_boolean, newElemInfo(elem_boolean, GVLCP_CODE_GINT32, 0));
  keyWords->call->add(keyWords, elem_byte, newElemInfo(elem_byte, GVLCP_CODE_CHAR, 0));
  keyWords->call->add(keyWords, elem_false, newElemInfo(elem_false, GVLCP_CODE_FALSE_NULL_TRUE, 0));
  keyWords->call->add(keyWords, elem_FALSE, newElemInfo(elem_FALSE, GVLCP_CODE_FALSE_NULL_TRUE, 0));
  keyWords->call->add(keyWords, elem_FILE, newElemInfo(elem_FILE, GVLCP_CODE_TYPE_FILE, 0));
  keyWords->call->add(keyWords, elem_gboolean, newElemInfo(elem_gboolean, GVLCP_CODE_GINT32, 0));
  keyWords->call->add(keyWords, elem_gchar, newElemInfo(elem_gchar, GVLCP_CODE_CHAR, 0));
  keyWords->call->add(keyWords, elem_gconstpointer, newElemInfo(elem_gconstpointer, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_gdouble, newElemInfo(elem_gdouble, GVLCP_CODE_GDOUBLE, 0));
  keyWords->call->add(keyWords, elem_gfloat, newElemInfo(elem_gfloat, GVLCP_CODE_FLOAT, 0));
  keyWords->call->add(keyWords, elem_gint, newElemInfo(elem_gint, GVLCP_CODE_GINT32, 0));
  keyWords->call->add(keyWords, elem_gint16, newElemInfo(elem_gint16, GVLCP_CODE_GINT16, 0));
  keyWords->call->add(keyWords, elem_gint32, newElemInfo(elem_gint32, GVLCP_CODE_GINT32, 0));
  keyWords->call->add(keyWords, elem_gint64, newElemInfo(elem_gint64, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_gint8, newElemInfo(elem_gint8, GVLCP_CODE_CHAR, 0));
  /* gintptr corresponds to the C99 type intptr_t. (WORD SIZE) */
  keyWords->call->add(keyWords, elem_gintptr, newElemInfo(elem_gintptr, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_glong, newElemInfo(elem_glong, GVLCP_CODE_GINT64, 0));
  /* goffset corresponds to the C99 type off64_t. */
  keyWords->call->add(keyWords, elem_goffset, newElemInfo(elem_goffset, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_gpointer, newElemInfo(elem_gpointer, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_gshort, newElemInfo(elem_gshort, GVLCP_CODE_GINT16, 0));
  /* gsize corresponds to the C99 type size_t. (WORD SIZE) */
  keyWords->call->add(keyWords, elem_gsize, newElemInfo(elem_gsize, GVLCP_CODE_GINT64, 0));
  /* gssize corresponds to the type ssize_t defined on most platforms. (WORD SIZE) */
  keyWords->call->add(keyWords, elem_gssize, newElemInfo(elem_gssize, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_guchar, newElemInfo(elem_guchar, GVLCP_CODE_CHAR, 0));
  keyWords->call->add(keyWords, elem_guint, newElemInfo(elem_guint, GVLCP_CODE_GINT32, 0));
  keyWords->call->add(keyWords, elem_guint16, newElemInfo(elem_guint16, GVLCP_CODE_GINT16, 0));
  keyWords->call->add(keyWords, elem_guint32, newElemInfo(elem_guint32, GVLCP_CODE_GINT32, 0));
  keyWords->call->add(keyWords, elem_guint64, newElemInfo(elem_guint64, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_guint8, newElemInfo(elem_guint8, GVLCP_CODE_CHAR, 0));
  /* guintptr corresponds to the C99 type uintptr_t. (WORD SIZE) */
  keyWords->call->add(keyWords, elem_guintptr, newElemInfo(elem_guintptr, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_gulong, newElemInfo(elem_gulong, GVLCP_CODE_GINT64, 0));
  keyWords->call->add(keyWords, elem_gushort, newElemInfo(elem_gushort, GVLCP_CODE_GINT16, 0));
  keyWords->call->add(keyWords, elem_NULL, newElemInfo(elem_NULL, GVLCP_CODE_FALSE_NULL_TRUE, 0));
  keyWords->call->add(keyWords, elem_readonly, newElemInfo(elem_readonly, GVLCP_CODE_CONST, 0));
  keyWords->call->add(keyWords, elem_restrict, newElemInfo(elem_restrict, GVLCP_CODE_RESTRICT, 0));
  keyWords->call->add(keyWords, elem_true, newElemInfo(elem_true, GVLCP_CODE_FALSE_NULL_TRUE, 0));
  keyWords->call->add(keyWords, elem_TRUE, newElemInfo(elem_TRUE, GVLCP_CODE_FALSE_NULL_TRUE, 0));
  keyWords->call->add(keyWords, elem_va_list, newElemInfo(elem_va_list, GVLCP_CODE_VA_LIST, 0));
  keyWords->call->add(keyWords, elem__Bool, newElemInfo(elem__Bool, GVLCP_CODE_CHAR, 0));
  keyWords->call->add(keyWords, elem__G_va_list, newElemInfo(elem__G_va_list, GVLCP_CODE_VA_LIST, 0));
  keyWords->call->add(keyWords, elem___const, newElemInfo(elem___const, GVLCP_CODE_CONST, 0));
  keyWords->call->add(keyWords, elem___restrict, newElemInfo(elem___restrict, GVLCP_CODE_RESTRICT, 0));
  keyWords->call->add(keyWords, elem___signed, newElemInfo(elem___signed, GVLCP_CODE_SIGNED, 0));
  keyWords->call->add(keyWords, elem___volatile, newElemInfo(elem___volatile, GVLCP_CODE_VOLATILE, 0));
  ___BTPOP;
}
