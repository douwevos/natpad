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
#include "../include/gvlcodeparsing/codes.h"
#include "../include/gvlcodeparsing/elements.h"
#include "../include/gvlcodeparsing/ElemInfo.h"
#include "../include/gvlcodeparsing/keyWords.h"

static StringElemInfoMap directives = NULL;
static StringElemInfoMap reservedWords = NULL;

static void cleanUp(void);
static StringElemInfoMap getDirectives(void);
static StringElemInfoMap getReservedWords(void);
static void init(void);

const struct keyWordsStruct KeyWords = {
  cleanUp,
  getDirectives,
  getReservedWords,
  init
};

static void cleanUp(void) {
  ___BTPUSH;
  directives->call->dump(directives);
  reservedWords->call->dump(reservedWords);
  reservedWords = NULL;
  directives = NULL;
  ___BTPOP;
}

static StringElemInfoMap getDirectives(void) {
  return directives;
}

static StringElemInfoMap getReservedWords(void) {
  return reservedWords;
}

static void init(void) {
  ___BTPUSH;
  directives = newStringElemInfoMap(29, 1.0);
  directives->call->add(directives, elem_define, newElemInfo(elem_define, GVLCP_CODE_DIRECTIVE_DEFINE, 0));
  directives->call->add(directives, elem_elif, newElemInfo(elem_elif, GVLCP_CODE_DIRECTIVE_ELIF, 0));
  directives->call->add(directives, elem_else, newElemInfo(elem_else, GVLCP_CODE_DIRECTIVE_ELSE, 0));
  directives->call->add(directives, elem_endif, newElemInfo(elem_endif, GVLCP_CODE_DIRECTIVE_ENDIF, 0));
  directives->call->add(directives, elem_error, newElemInfo(elem_error, GVLCP_CODE_DIRECTIVE_ERROR, 0));
  directives->call->add(directives, elem_if, newElemInfo(elem_if, GVLCP_CODE_DIRECTIVE_IF, 0));
  directives->call->add(directives, elem_ifdef, newElemInfo(elem_ifdef, GVLCP_CODE_DIRECTIVE_IFDEF, 0));
  directives->call->add(directives, elem_ifndef, newElemInfo(elem_ifndef, GVLCP_CODE_DIRECTIVE_IFNDEF, 0));
  directives->call->add(directives, elem_include, newElemInfo(elem_include, GVLCP_CODE_DIRECTIVE_INCLUDE, 0));
  directives->call->add(directives, elem_line, newElemInfo(elem_line, GVLCP_CODE_DIRECTIVE_LINE, 0));
  directives->call->add(directives, elem_pragma, newElemInfo(elem_pragma, GVLCP_CODE_DIRECTIVE_PRAGMA, 0));
  directives->call->add(directives, elem_undef, newElemInfo(elem_undef, GVLCP_CODE_DIRECTIVE_UNDEF, 0));

  reservedWords = newStringElemInfoMap(113, 1.0);
  reservedWords->call->add(reservedWords, elem_asm, newElemInfo(elem_asm, GVLCP_CODE_ASM, GVLCP_WARN_NOT_ANSI));
  reservedWords->call->add(reservedWords, elem_auto, newElemInfo(elem_auto, GVLCP_CODE_AUTO, 0));
  reservedWords->call->add(reservedWords, elem_break, newElemInfo(elem_break, GVLCP_CODE_BREAK, 0));
  reservedWords->call->add(reservedWords, elem_case, newElemInfo(elem_case, GVLCP_CODE_CASE, 0));
  reservedWords->call->add(reservedWords, elem_char, newElemInfo(elem_char, GVLCP_CODE_CHAR, 0));
  reservedWords->call->add(reservedWords, elem_const, newElemInfo(elem_const, GVLCP_CODE_CONST, 0));
  reservedWords->call->add(reservedWords, elem_continue, newElemInfo(elem_continue, GVLCP_CODE_CONTINUE, 0));
  reservedWords->call->add(reservedWords, elem_default, newElemInfo(elem_default, GVLCP_CODE_DEFAULT, 0));
  reservedWords->call->add(reservedWords, elem_defined, newElemInfo(elem_defined, GVLCP_CODE_DEFINED, 0));
  reservedWords->call->add(reservedWords, elem_do, newElemInfo(elem_do, GVLCP_CODE_DO, 0));
  reservedWords->call->add(reservedWords, elem_double, newElemInfo(elem_double, GVLCP_CODE_DOUBLE, 0));
  reservedWords->call->add(reservedWords, elem_else, newElemInfo(elem_else, GVLCP_CODE_ELSE, 0));
  reservedWords->call->add(reservedWords, elem_enum, newElemInfo(elem_enum, GVLCP_CODE_ENUM, 0));
  reservedWords->call->add(reservedWords, elem_extern, newElemInfo(elem_extern, GVLCP_CODE_EXTERN, 0));
  reservedWords->call->add(reservedWords, elem_float, newElemInfo(elem_float, GVLCP_CODE_FLOAT, 0));
  reservedWords->call->add(reservedWords, elem_for, newElemInfo(elem_for, GVLCP_CODE_FOR, 0));
  reservedWords->call->add(reservedWords, elem_fortran, newElemInfo(elem_fortran, GVLCP_CODE_FORTRAN, GVLCP_WARN_NOT_ANSI));
  reservedWords->call->add(reservedWords, elem_goto, newElemInfo(elem_goto, GVLCP_CODE_GOTO, 0));
  reservedWords->call->add(reservedWords, elem_if, newElemInfo(elem_if, GVLCP_CODE_IF, 0));
  reservedWords->call->add(reservedWords, elem_inline, newElemInfo(elem_inline, GVLCP_CODE_INLINE, GVLCP_WARN_NOT_ANSI));
  reservedWords->call->add(reservedWords, elem_int, newElemInfo(elem_int, GVLCP_CODE_INT, 0));
  reservedWords->call->add(reservedWords, elem_long, newElemInfo(elem_long, GVLCP_CODE_LONG, 0));
  reservedWords->call->add(reservedWords, elem_register, newElemInfo(elem_register, GVLCP_CODE_REGISTER, 0));
  reservedWords->call->add(reservedWords, elem_return, newElemInfo(elem_return, GVLCP_CODE_RETURN, 0));
  reservedWords->call->add(reservedWords, elem_short, newElemInfo(elem_short, GVLCP_CODE_SHORT, 0));
  reservedWords->call->add(reservedWords, elem_signed, newElemInfo(elem_signed, GVLCP_CODE_SIGNED, 0));
  reservedWords->call->add(reservedWords, elem_sizeof, newElemInfo(elem_sizeof, GVLCP_CODE_SIZEOF, 0));
  reservedWords->call->add(reservedWords, elem_static, newElemInfo(elem_static, GVLCP_CODE_STATIC, 0));
  reservedWords->call->add(reservedWords, elem_struct, newElemInfo(elem_struct, GVLCP_CODE_STRUCT, 0));
  reservedWords->call->add(reservedWords, elem_switch, newElemInfo(elem_switch, GVLCP_CODE_SWITCH, 0));
  reservedWords->call->add(reservedWords, elem_typedef, newElemInfo(elem_typedef, GVLCP_CODE_TYPEDEF, 0));
  reservedWords->call->add(reservedWords, elem_union, newElemInfo(elem_union, GVLCP_CODE_UNION, 0));
  reservedWords->call->add(reservedWords, elem_unsigned, newElemInfo(elem_unsigned, GVLCP_CODE_UNSIGNED, 0));
  reservedWords->call->add(reservedWords, elem_void, newElemInfo(elem_void, GVLCP_CODE_VOID, 0));
  reservedWords->call->add(reservedWords, elem_volatile, newElemInfo(elem_volatile, GVLCP_CODE_VOLATILE, 0));
  reservedWords->call->add(reservedWords, elem_while, newElemInfo(elem_while, GVLCP_CODE_WHILE, 0));
  reservedWords->call->add(reservedWords, elem___attribute__, newElemInfo(elem___attribute__, GVLCP_CODE_ATTRIBUTE, 0));
  reservedWords->call->add(reservedWords, elem___FILE__, newElemInfo(elem___FILE__, GVLCP_CODE_FILE, 0));
  reservedWords->call->add(reservedWords, elem___LINE__, newElemInfo(elem___LINE__, GVLCP_CODE_LINE, 0));

  reservedWords->call->add(reservedWords, elem_bool, newElemInfo(elem_bool, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_boolean, newElemInfo(elem_boolean, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_byte, newElemInfo(elem_byte, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_false, newElemInfo(elem_false, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_FALSE, newElemInfo(elem_FALSE, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_FILE, newElemInfo(elem_FILE, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gboolean, newElemInfo(elem_gboolean, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gchar, newElemInfo(elem_gchar, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gconstpointer, newElemInfo(elem_gconstpointer, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gdouble, newElemInfo(elem_gdouble, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gfloat, newElemInfo(elem_gfloat, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gint, newElemInfo(elem_gint, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gint8, newElemInfo(elem_gint8, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gint16, newElemInfo(elem_gint16, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gint32, newElemInfo(elem_gint32, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gint64, newElemInfo(elem_gint64, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gintptr, newElemInfo(elem_gintptr, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_glong, newElemInfo(elem_glong, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_goffset, newElemInfo(elem_goffset, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gpointer, newElemInfo(elem_gpointer, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gshort, newElemInfo(elem_gshort, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gsize, newElemInfo(elem_gsize, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gssize, newElemInfo(elem_gssize, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_guchar, newElemInfo(elem_guchar, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_guint, newElemInfo(elem_guint, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_guint8, newElemInfo(elem_guint8, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_guint16, newElemInfo(elem_guint16, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_guint32, newElemInfo(elem_guint32, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_guint64, newElemInfo(elem_guint64, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_guintptr, newElemInfo(elem_guintptr, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gulong, newElemInfo(elem_gulong, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_gushort, newElemInfo(elem_gushort, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_NULL, newElemInfo(elem_NULL, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_readonly, newElemInfo(elem_readonly, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_restrict, newElemInfo(elem_restrict, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_true, newElemInfo(elem_true, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_TRUE, newElemInfo(elem_TRUE, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem_va_list, newElemInfo(elem_va_list, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem__Bool, newElemInfo(elem__Bool, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem__G_va_list, newElemInfo(elem__G_va_list, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem___const, newElemInfo(elem___const, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem___restrict, newElemInfo(elem___restrict, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem___signed, newElemInfo(elem___signed, GVLCP_CODE_IDENTIFIER, 0));
  reservedWords->call->add(reservedWords, elem___volatile, newElemInfo(elem___volatile, GVLCP_CODE_IDENTIFIER, 0));
  ___BTPOP;
}
