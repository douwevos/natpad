/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

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

#include <gvlib/defs.h>
#include <gvlib/private/stringPrivate.h>
#include "../include/gvlcodeparsing/elements.h"

static struct private_stringStruct elem_21_ = {
  0,
  ptrof(predefStringFunctions),
  "!",
  1,
  -1
};
const String elem_21 = (String)ptrof(elem_21_);

static struct private_stringStruct elem_213D_ = {
  0,
  ptrof(predefStringFunctions),
  "!=",
  2,
  -1
};
const String elem_213D = (String)ptrof(elem_213D_);

static struct private_stringStruct elem_23_ = {
  0,
  ptrof(predefStringFunctions),
  "#",
  1,
  -1
};
const String elem_23 = (String)ptrof(elem_23_);

static struct private_stringStruct elem_2323_ = {
  0,
  ptrof(predefStringFunctions),
  "##",
  2,
  -1
};
const String elem_2323 = (String)ptrof(elem_2323_);

static struct private_stringStruct elem_25_ = {
  0,
  ptrof(predefStringFunctions),
  "%",
  1,
  -1
};
const String elem_25 = (String)ptrof(elem_25_);

static struct private_stringStruct elem_253D_ = {
  0,
  ptrof(predefStringFunctions),
  "%=",
  2,
  -1
};
const String elem_253D = (String)ptrof(elem_253D_);

static struct private_stringStruct elem_26_ = {
  0,
  ptrof(predefStringFunctions),
  "&",
  1,
  -1
};
const String elem_26 = (String)ptrof(elem_26_);

static struct private_stringStruct elem_2626_ = {
  0,
  ptrof(predefStringFunctions),
  "&&",
  2,
  -1
};
const String elem_2626 = (String)ptrof(elem_2626_);

static struct private_stringStruct elem_263D_ = {
  0,
  ptrof(predefStringFunctions),
  "&=",
  2,
  -1
};
const String elem_263D = (String)ptrof(elem_263D_);

static struct private_stringStruct elem_28_ = {
  0,
  ptrof(predefStringFunctions),
  "(",
  1,
  -1
};
const String elem_28 = (String)ptrof(elem_28_);

static struct private_stringStruct elem_29_ = {
  0,
  ptrof(predefStringFunctions),
  ")",
  1,
  -1
};
const String elem_29 = (String)ptrof(elem_29_);

static struct private_stringStruct elem_2A_ = {
  0,
  ptrof(predefStringFunctions),
  "*",
  1,
  -1
};
const String elem_2A = (String)ptrof(elem_2A_);

static struct private_stringStruct elem_2A3D_ = {
  0,
  ptrof(predefStringFunctions),
  "*=",
  2,
  -1
};
const String elem_2A3D = (String)ptrof(elem_2A3D_);

static struct private_stringStruct elem_2B_ = {
  0,
  ptrof(predefStringFunctions),
  "+",
  1,
  -1
};
const String elem_2B = (String)ptrof(elem_2B_);

static struct private_stringStruct elem_2B2B_ = {
  0,
  ptrof(predefStringFunctions),
  "++",
  2,
  -1
};
const String elem_2B2B = (String)ptrof(elem_2B2B_);

static struct private_stringStruct elem_2B3D_ = {
  0,
  ptrof(predefStringFunctions),
  "+=",
  2,
  -1
};
const String elem_2B3D = (String)ptrof(elem_2B3D_);

static struct private_stringStruct elem_2C_ = {
  0,
  ptrof(predefStringFunctions),
  ",",
  1,
  -1
};
const String elem_2C = (String)ptrof(elem_2C_);

static struct private_stringStruct elem_2D_ = {
  0,
  ptrof(predefStringFunctions),
  "-",
  1,
  -1
};
const String elem_2D = (String)ptrof(elem_2D_);

static struct private_stringStruct elem_2D2D_ = {
  0,
  ptrof(predefStringFunctions),
  "--",
  2,
  -1
};
const String elem_2D2D = (String)ptrof(elem_2D2D_);

static struct private_stringStruct elem_2D3D_ = {
  0,
  ptrof(predefStringFunctions),
  "-=",
  2,
  -1
};
const String elem_2D3D = (String)ptrof(elem_2D3D_);

static struct private_stringStruct elem_2D3E_ = {
  0,
  ptrof(predefStringFunctions),
  "->",
  2,
  -1
};
const String elem_2D3E = (String)ptrof(elem_2D3E_);

static struct private_stringStruct elem_2E_ = {
  0,
  ptrof(predefStringFunctions),
  ".",
  1,
  -1
};
const String elem_2E = (String)ptrof(elem_2E_);

static struct private_stringStruct elem_2E2E_ = {
  0,
  ptrof(predefStringFunctions),
  "..",
  2,
  -1
};
const String elem_2E2E = (String)ptrof(elem_2E2E_);

static struct private_stringStruct elem_2E2E2E_ = {
  0,
  ptrof(predefStringFunctions),
  "...",
  3,
  -1
};
const String elem_2E2E2E = (String)ptrof(elem_2E2E2E_);

static struct private_stringStruct elem_2F_ = {
  0,
  ptrof(predefStringFunctions),
  "/",
  1,
  -1
};
const String elem_2F = (String)ptrof(elem_2F_);

static struct private_stringStruct elem_2F3D_ = {
  0,
  ptrof(predefStringFunctions),
  "/=",
  2,
  -1
};
const String elem_2F3D = (String)ptrof(elem_2F3D_);

static struct private_stringStruct elem_30_ = {
  0,
  ptrof(predefStringFunctions),
  "0",
  1,
  -1
};
const String elem_30 = (String)ptrof(elem_30_);

static struct private_stringStruct elem_31_ = {
  0,
  ptrof(predefStringFunctions),
  "1",
  1,
  -1
};
const String elem_31 = (String)ptrof(elem_31_);

static struct private_stringStruct elem_3A_ = {
  0,
  ptrof(predefStringFunctions),
  ":",
  1,
  -1
};
const String elem_3A = (String)ptrof(elem_3A_);

static struct private_stringStruct elem_3B_ = {
  0,
  ptrof(predefStringFunctions),
  ";",
  1,
  -1
};
const String elem_3B = (String)ptrof(elem_3B_);

static struct private_stringStruct elem_3C_ = {
  0,
  ptrof(predefStringFunctions),
  "<",
  1,
  -1
};
const String elem_3C = (String)ptrof(elem_3C_);

static struct private_stringStruct elem_3C3C_ = {
  0,
  ptrof(predefStringFunctions),
  "<<",
  2,
  -1
};
const String elem_3C3C = (String)ptrof(elem_3C3C_);

static struct private_stringStruct elem_3C3C3D_ = {
  0,
  ptrof(predefStringFunctions),
  "<<=",
  3,
  -1
};
const String elem_3C3C3D = (String)ptrof(elem_3C3C3D_);

static struct private_stringStruct elem_3C3D_ = {
  0,
  ptrof(predefStringFunctions),
  "<=",
  2,
  -1
};
const String elem_3C3D = (String)ptrof(elem_3C3D_);

static struct private_stringStruct elem_3D_ = {
  0,
  ptrof(predefStringFunctions),
  "=",
  1,
  -1
};
const String elem_3D = (String)ptrof(elem_3D_);

static struct private_stringStruct elem_3D3D_ = {
  0,
  ptrof(predefStringFunctions),
  "==",
  2,
  -1
};
const String elem_3D3D = (String)ptrof(elem_3D3D_);

static struct private_stringStruct elem_3E_ = {
  0,
  ptrof(predefStringFunctions),
  ">",
  1,
  -1
};
const String elem_3E = (String)ptrof(elem_3E_);

static struct private_stringStruct elem_3E3D_ = {
  0,
  ptrof(predefStringFunctions),
  ">=",
  2,
  -1
};
const String elem_3E3D = (String)ptrof(elem_3E3D_);

static struct private_stringStruct elem_3E3E_ = {
  0,
  ptrof(predefStringFunctions),
  ">>",
  2,
  -1
};
const String elem_3E3E = (String)ptrof(elem_3E3E_);

static struct private_stringStruct elem_3E3E3D_ = {
  0,
  ptrof(predefStringFunctions),
  ">>=",
  3,
  -1
};
const String elem_3E3E3D = (String)ptrof(elem_3E3E3D_);

static struct private_stringStruct elem_3E3E3E_ = {
  0,
  ptrof(predefStringFunctions),
  ">>>",
  3,
  -1
};
const String elem_3E3E3E = (String)ptrof(elem_3E3E3E_);

static struct private_stringStruct elem_3E3E3E3D_ = {
  0,
  ptrof(predefStringFunctions),
  ">>>=",
  4,
  -1
};
const String elem_3E3E3E3D = (String)ptrof(elem_3E3E3E3D_);

static struct private_stringStruct elem_3F_ = {
  0,
  ptrof(predefStringFunctions),
  "?",
  1,
  -1
};
const String elem_3F = (String)ptrof(elem_3F_);

static struct private_stringStruct elem_5B_ = {
  0,
  ptrof(predefStringFunctions),
  "[",
  1,
  -1
};
const String elem_5B = (String)ptrof(elem_5B_);

static struct private_stringStruct elem_5C_ = {
  0,
  ptrof(predefStringFunctions),
  "\\",
  1,
  -1
};
const String elem_5C = (String)ptrof(elem_5C_);

static struct private_stringStruct elem_5D_ = {
  0,
  ptrof(predefStringFunctions),
  "]",
  1,
  -1
};
const String elem_5D = (String)ptrof(elem_5D_);

static struct private_stringStruct elem_5E_ = {
  0,
  ptrof(predefStringFunctions),
  "^",
  1,
  -1
};
const String elem_5E = (String)ptrof(elem_5E_);

static struct private_stringStruct elem_5E3D_ = {
  0,
  ptrof(predefStringFunctions),
  "^=",
  2,
  -1
};
const String elem_5E3D = (String)ptrof(elem_5E3D_);

static struct private_stringStruct elem_7B_ = {
  0,
  ptrof(predefStringFunctions),
  "{",
  1,
  -1
};
const String elem_7B = (String)ptrof(elem_7B_);

static struct private_stringStruct elem_7C_ = {
  0,
  ptrof(predefStringFunctions),
  "|",
  1,
  -1
};
const String elem_7C = (String)ptrof(elem_7C_);

static struct private_stringStruct elem_7C3D_ = {
  0,
  ptrof(predefStringFunctions),
  "|=",
  2,
  -1
};
const String elem_7C3D = (String)ptrof(elem_7C3D_);

static struct private_stringStruct elem_7C7C_ = {
  0,
  ptrof(predefStringFunctions),
  "||",
  2,
  -1
};
const String elem_7C7C = (String)ptrof(elem_7C7C_);

static struct private_stringStruct elem_7D_ = {
  0,
  ptrof(predefStringFunctions),
  "}",
  1,
  -1
};
const String elem_7D = (String)ptrof(elem_7D_);

static struct private_stringStruct elem_7E_ = {
  0,
  ptrof(predefStringFunctions),
  "~",
  1,
  -1
};
const String elem_7E = (String)ptrof(elem_7E_);

static struct private_stringStruct elem_abstract_ = {
  0,
  ptrof(predefStringFunctions),
  "abstract",
  8,
  -1
};
const String elem_abstract = (String)ptrof(elem_abstract_);

static struct private_stringStruct elem_asm_ = {
  0,
  ptrof(predefStringFunctions),
  "asm",
  3,
  -1
};
const String elem_asm = (String)ptrof(elem_asm_);

static struct private_stringStruct elem_assert_ = {
  0,
  ptrof(predefStringFunctions),
  "assert",
  6,
  -1
};
const String elem_assert = (String)ptrof(elem_assert_);

static struct private_stringStruct elem_auto_ = {
  0,
  ptrof(predefStringFunctions),
  "auto",
  4,
  -1
};
const String elem_auto = (String)ptrof(elem_auto_);

static struct private_stringStruct elem_bool_ = {
  0,
  ptrof(predefStringFunctions),
  "bool",
  4,
  -1
};
const String elem_bool = (String)ptrof(elem_bool_);

static struct private_stringStruct elem_boolean_ = {
  0,
  ptrof(predefStringFunctions),
  "boolean",
  7,
  -1
};
const String elem_boolean = (String)ptrof(elem_boolean_);

static struct private_stringStruct elem_break_ = {
  0,
  ptrof(predefStringFunctions),
  "break",
  5,
  -1
};
const String elem_break = (String)ptrof(elem_break_);

static struct private_stringStruct elem_byte_ = {
  0,
  ptrof(predefStringFunctions),
  "byte",
  4,
  -1
};
const String elem_byte = (String)ptrof(elem_byte_);

static struct private_stringStruct elem_case_ = {
  0,
  ptrof(predefStringFunctions),
  "case",
  4,
  -1
};
const String elem_case = (String)ptrof(elem_case_);

static struct private_stringStruct elem_catch_ = {
  0,
  ptrof(predefStringFunctions),
  "catch",
  5,
  -1
};
const String elem_catch = (String)ptrof(elem_catch_);

static struct private_stringStruct elem_char_ = {
  0,
  ptrof(predefStringFunctions),
  "char",
  4,
  -1
};
const String elem_char = (String)ptrof(elem_char_);

static struct private_stringStruct elem_class_ = {
  0,
  ptrof(predefStringFunctions),
  "class",
  5,
  -1
};
const String elem_class = (String)ptrof(elem_class_);

static struct private_stringStruct elem_const_ = {
  0,
  ptrof(predefStringFunctions),
  "const",
  5,
  -1
};
const String elem_const = (String)ptrof(elem_const_);

static struct private_stringStruct elem_continue_ = {
  0,
  ptrof(predefStringFunctions),
  "continue",
  8,
  -1
};
const String elem_continue = (String)ptrof(elem_continue_);

static struct private_stringStruct elem_default_ = {
  0,
  ptrof(predefStringFunctions),
  "default",
  7,
  -1
};
const String elem_default = (String)ptrof(elem_default_);

static struct private_stringStruct elem_define_ = {
  0,
  ptrof(predefStringFunctions),
  "define",
  6,
  -1
};
const String elem_define = (String)ptrof(elem_define_);

static struct private_stringStruct elem_defined_ = {
  0,
  ptrof(predefStringFunctions),
  "defined",
  7,
  -1
};
const String elem_defined = (String)ptrof(elem_defined_);

static struct private_stringStruct elem_delete_ = {
  0,
  ptrof(predefStringFunctions),
  "delete",
  6,
  -1
};
const String elem_delete = (String)ptrof(elem_delete_);

static struct private_stringStruct elem_do_ = {
  0,
  ptrof(predefStringFunctions),
  "do",
  2,
  -1
};
const String elem_do = (String)ptrof(elem_do_);

static struct private_stringStruct elem_double_ = {
  0,
  ptrof(predefStringFunctions),
  "double",
  6,
  -1
};
const String elem_double = (String)ptrof(elem_double_);

static struct private_stringStruct elem_elif_ = {
  0,
  ptrof(predefStringFunctions),
  "elif",
  4,
  -1
};
const String elem_elif = (String)ptrof(elem_elif_);

static struct private_stringStruct elem_else_ = {
  0,
  ptrof(predefStringFunctions),
  "else",
  4,
  -1
};
const String elem_else = (String)ptrof(elem_else_);

static struct private_stringStruct elem_endif_ = {
  0,
  ptrof(predefStringFunctions),
  "endif",
  5,
  -1
};
const String elem_endif = (String)ptrof(elem_endif_);

static struct private_stringStruct elem_enum_ = {
  0,
  ptrof(predefStringFunctions),
  "enum",
  4,
  -1
};
const String elem_enum = (String)ptrof(elem_enum_);

static struct private_stringStruct elem_error_ = {
  0,
  ptrof(predefStringFunctions),
  "error",
  5,
  -1
};
const String elem_error = (String)ptrof(elem_error_);

static struct private_stringStruct elem_extends_ = {
  0,
  ptrof(predefStringFunctions),
  "extends",
  7,
  -1
};
const String elem_extends = (String)ptrof(elem_extends_);

static struct private_stringStruct elem_extern_ = {
  0,
  ptrof(predefStringFunctions),
  "extern",
  6,
  -1
};
const String elem_extern = (String)ptrof(elem_extern_);

static struct private_stringStruct elem_false_ = {
  0,
  ptrof(predefStringFunctions),
  "false",
  5,
  -1
};
const String elem_false = (String)ptrof(elem_false_);

static struct private_stringStruct elem_FALSE_ = {
  0,
  ptrof(predefStringFunctions),
  "FALSE",
  5,
  -1
};
const String elem_FALSE = (String)ptrof(elem_FALSE_);

static struct private_stringStruct elem_FILE_ = {
  0,
  ptrof(predefStringFunctions),
  "FILE",
  4,
  -1
};
const String elem_FILE = (String)ptrof(elem_FILE_);

static struct private_stringStruct elem_final_ = {
  0,
  ptrof(predefStringFunctions),
  "final",
  5,
  -1
};
const String elem_final = (String)ptrof(elem_final_);

static struct private_stringStruct elem_finally_ = {
  0,
  ptrof(predefStringFunctions),
  "finally",
  7,
  -1
};
const String elem_finally = (String)ptrof(elem_finally_);

static struct private_stringStruct elem_float_ = {
  0,
  ptrof(predefStringFunctions),
  "float",
  5,
  -1
};
const String elem_float = (String)ptrof(elem_float_);

static struct private_stringStruct elem_for_ = {
  0,
  ptrof(predefStringFunctions),
  "for",
  3,
  -1
};
const String elem_for = (String)ptrof(elem_for_);

static struct private_stringStruct elem_fortran_ = {
  0,
  ptrof(predefStringFunctions),
  "fortran",
  7,
  -1
};
const String elem_fortran = (String)ptrof(elem_fortran_);

static struct private_stringStruct elem_gboolean_ = {
  0,
  ptrof(predefStringFunctions),
  "gboolean",
  8,
  -1
};
const String elem_gboolean = (String)ptrof(elem_gboolean_);

static struct private_stringStruct elem_gchar_ = {
  0,
  ptrof(predefStringFunctions),
  "gchar",
  5,
  -1
};
const String elem_gchar = (String)ptrof(elem_gchar_);

static struct private_stringStruct elem_gconstpointer_ = {
  0,
  ptrof(predefStringFunctions),
  "gconstpointer",
  13,
  -1
};
const String elem_gconstpointer = (String)ptrof(elem_gconstpointer_);

static struct private_stringStruct elem_gdouble_ = {
  0,
  ptrof(predefStringFunctions),
  "gdouble",
  7,
  -1
};
const String elem_gdouble = (String)ptrof(elem_gdouble_);

static struct private_stringStruct elem_gfloat_ = {
  0,
  ptrof(predefStringFunctions),
  "gfloat",
  6,
  -1
};
const String elem_gfloat = (String)ptrof(elem_gfloat_);

static struct private_stringStruct elem_gint_ = {
  0,
  ptrof(predefStringFunctions),
  "gint",
  4,
  -1
};
const String elem_gint = (String)ptrof(elem_gint_);

static struct private_stringStruct elem_gint16_ = {
  0,
  ptrof(predefStringFunctions),
  "gint16",
  6,
  -1
};
const String elem_gint16 = (String)ptrof(elem_gint16_);

static struct private_stringStruct elem_gint32_ = {
  0,
  ptrof(predefStringFunctions),
  "gint32",
  6,
  -1
};
const String elem_gint32 = (String)ptrof(elem_gint32_);

static struct private_stringStruct elem_gint64_ = {
  0,
  ptrof(predefStringFunctions),
  "gint64",
  6,
  -1
};
const String elem_gint64 = (String)ptrof(elem_gint64_);

static struct private_stringStruct elem_gint8_ = {
  0,
  ptrof(predefStringFunctions),
  "gint8",
  5,
  -1
};
const String elem_gint8 = (String)ptrof(elem_gint8_);

static struct private_stringStruct elem_gintptr_ = {
  0,
  ptrof(predefStringFunctions),
  "gintptr",
  7,
  -1
};
const String elem_gintptr = (String)ptrof(elem_gintptr_);

static struct private_stringStruct elem_glong_ = {
  0,
  ptrof(predefStringFunctions),
  "glong",
  5,
  -1
};
const String elem_glong = (String)ptrof(elem_glong_);

static struct private_stringStruct elem_goffset_ = {
  0,
  ptrof(predefStringFunctions),
  "goffset",
  7,
  -1
};
const String elem_goffset = (String)ptrof(elem_goffset_);

static struct private_stringStruct elem_goto_ = {
  0,
  ptrof(predefStringFunctions),
  "goto",
  4,
  -1
};
const String elem_goto = (String)ptrof(elem_goto_);

static struct private_stringStruct elem_gpointer_ = {
  0,
  ptrof(predefStringFunctions),
  "gpointer",
  8,
  -1
};
const String elem_gpointer = (String)ptrof(elem_gpointer_);

static struct private_stringStruct elem_gshort_ = {
  0,
  ptrof(predefStringFunctions),
  "gshort",
  6,
  -1
};
const String elem_gshort = (String)ptrof(elem_gshort_);

static struct private_stringStruct elem_gsize_ = {
  0,
  ptrof(predefStringFunctions),
  "gsize",
  5,
  -1
};
const String elem_gsize = (String)ptrof(elem_gsize_);

static struct private_stringStruct elem_gssize_ = {
  0,
  ptrof(predefStringFunctions),
  "gssize",
  6,
  -1
};
const String elem_gssize = (String)ptrof(elem_gssize_);

static struct private_stringStruct elem_guchar_ = {
  0,
  ptrof(predefStringFunctions),
  "guchar",
  6,
  -1
};
const String elem_guchar = (String)ptrof(elem_guchar_);

static struct private_stringStruct elem_guint_ = {
  0,
  ptrof(predefStringFunctions),
  "guint",
  5,
  -1
};
const String elem_guint = (String)ptrof(elem_guint_);

static struct private_stringStruct elem_guint16_ = {
  0,
  ptrof(predefStringFunctions),
  "guint16",
  7,
  -1
};
const String elem_guint16 = (String)ptrof(elem_guint16_);

static struct private_stringStruct elem_guint32_ = {
  0,
  ptrof(predefStringFunctions),
  "guint32",
  7,
  -1
};
const String elem_guint32 = (String)ptrof(elem_guint32_);

static struct private_stringStruct elem_guint64_ = {
  0,
  ptrof(predefStringFunctions),
  "guint64",
  7,
  -1
};
const String elem_guint64 = (String)ptrof(elem_guint64_);

static struct private_stringStruct elem_guint8_ = {
  0,
  ptrof(predefStringFunctions),
  "guint8",
  6,
  -1
};
const String elem_guint8 = (String)ptrof(elem_guint8_);

static struct private_stringStruct elem_guintptr_ = {
  0,
  ptrof(predefStringFunctions),
  "guintptr",
  8,
  -1
};
const String elem_guintptr = (String)ptrof(elem_guintptr_);

static struct private_stringStruct elem_gulong_ = {
  0,
  ptrof(predefStringFunctions),
  "gulong",
  6,
  -1
};
const String elem_gulong = (String)ptrof(elem_gulong_);

static struct private_stringStruct elem_gushort_ = {
  0,
  ptrof(predefStringFunctions),
  "gushort",
  7,
  -1
};
const String elem_gushort = (String)ptrof(elem_gushort_);

static struct private_stringStruct elem_if_ = {
  0,
  ptrof(predefStringFunctions),
  "if",
  2,
  -1
};
const String elem_if = (String)ptrof(elem_if_);

static struct private_stringStruct elem_ifdef_ = {
  0,
  ptrof(predefStringFunctions),
  "ifdef",
  5,
  -1
};
const String elem_ifdef = (String)ptrof(elem_ifdef_);

static struct private_stringStruct elem_ifndef_ = {
  0,
  ptrof(predefStringFunctions),
  "ifndef",
  6,
  -1
};
const String elem_ifndef = (String)ptrof(elem_ifndef_);

static struct private_stringStruct elem_implements_ = {
  0,
  ptrof(predefStringFunctions),
  "implements",
  10,
  -1
};
const String elem_implements = (String)ptrof(elem_implements_);

static struct private_stringStruct elem_import_ = {
  0,
  ptrof(predefStringFunctions),
  "import",
  6,
  -1
};
const String elem_import = (String)ptrof(elem_import_);

static struct private_stringStruct elem_include_ = {
  0,
  ptrof(predefStringFunctions),
  "include",
  7,
  -1
};
const String elem_include = (String)ptrof(elem_include_);

static struct private_stringStruct elem_inline_ = {
  0,
  ptrof(predefStringFunctions),
  "inline",
  6,
  -1
};
const String elem_inline = (String)ptrof(elem_inline_);

static struct private_stringStruct elem_instanceof_ = {
  0,
  ptrof(predefStringFunctions),
  "instanceof",
  10,
  -1
};
const String elem_instanceof = (String)ptrof(elem_instanceof_);

static struct private_stringStruct elem_int_ = {
  0,
  ptrof(predefStringFunctions),
  "int",
  3,
  -1
};
const String elem_int = (String)ptrof(elem_int_);

static struct private_stringStruct elem_interface_ = {
  0,
  ptrof(predefStringFunctions),
  "interface",
  9,
  -1
};
const String elem_interface = (String)ptrof(elem_interface_);

static struct private_stringStruct elem_line_ = {
  0,
  ptrof(predefStringFunctions),
  "line",
  4,
  -1
};
const String elem_line = (String)ptrof(elem_line_);

static struct private_stringStruct elem_long_ = {
  0,
  ptrof(predefStringFunctions),
  "long",
  4,
  -1
};
const String elem_long = (String)ptrof(elem_long_);

static struct private_stringStruct elem_native_ = {
  0,
  ptrof(predefStringFunctions),
  "native",
  6,
  -1
};
const String elem_native = (String)ptrof(elem_native_);

static struct private_stringStruct elem_new_ = {
  0,
  ptrof(predefStringFunctions),
  "new",
  3,
  -1
};
const String elem_new = (String)ptrof(elem_new_);

static struct private_stringStruct elem_null_ = {
  0,
  ptrof(predefStringFunctions),
  "null",
  4,
  -1
};
const String elem_null = (String)ptrof(elem_null_);

static struct private_stringStruct elem_NULL_ = {
  0,
  ptrof(predefStringFunctions),
  "NULL",
  4,
  -1
};
const String elem_NULL = (String)ptrof(elem_NULL_);

static struct private_stringStruct elem_package_ = {
  0,
  ptrof(predefStringFunctions),
  "package",
  7,
  -1
};
const String elem_package = (String)ptrof(elem_package_);

static struct private_stringStruct elem_pragma_ = {
  0,
  ptrof(predefStringFunctions),
  "pragma",
  6,
  -1
};
const String elem_pragma = (String)ptrof(elem_pragma_);

static struct private_stringStruct elem_private_ = {
  0,
  ptrof(predefStringFunctions),
  "private",
  7,
  -1
};
const String elem_private = (String)ptrof(elem_private_);

static struct private_stringStruct elem_protected_ = {
  0,
  ptrof(predefStringFunctions),
  "protected",
  9,
  -1
};
const String elem_protected = (String)ptrof(elem_protected_);

static struct private_stringStruct elem_public_ = {
  0,
  ptrof(predefStringFunctions),
  "public",
  6,
  -1
};
const String elem_public = (String)ptrof(elem_public_);

static struct private_stringStruct elem_readonly_ = {
  0,
  ptrof(predefStringFunctions),
  "readonly",
  8,
  -1
};
const String elem_readonly = (String)ptrof(elem_readonly_);

static struct private_stringStruct elem_register_ = {
  0,
  ptrof(predefStringFunctions),
  "register",
  8,
  -1
};
const String elem_register = (String)ptrof(elem_register_);

static struct private_stringStruct elem_restrict_ = {
  0,
  ptrof(predefStringFunctions),
  "restrict",
  8,
  -1
};
const String elem_restrict = (String)ptrof(elem_restrict_);

static struct private_stringStruct elem_return_ = {
  0,
  ptrof(predefStringFunctions),
  "return",
  6,
  -1
};
const String elem_return = (String)ptrof(elem_return_);

static struct private_stringStruct elem_short_ = {
  0,
  ptrof(predefStringFunctions),
  "short",
  5,
  -1
};
const String elem_short = (String)ptrof(elem_short_);

static struct private_stringStruct elem_signed_ = {
  0,
  ptrof(predefStringFunctions),
  "signed",
  6,
  -1
};
const String elem_signed = (String)ptrof(elem_signed_);

static struct private_stringStruct elem_sizeof_ = {
  0,
  ptrof(predefStringFunctions),
  "sizeof",
  6,
  -1
};
const String elem_sizeof = (String)ptrof(elem_sizeof_);

static struct private_stringStruct elem_static_ = {
  0,
  ptrof(predefStringFunctions),
  "static",
  6,
  -1
};
const String elem_static = (String)ptrof(elem_static_);

static struct private_stringStruct elem_strictfp_ = {
  0,
  ptrof(predefStringFunctions),
  "strictfp",
  8,
  -1
};
const String elem_strictfp = (String)ptrof(elem_strictfp_);

static struct private_stringStruct elem_struct_ = {
  0,
  ptrof(predefStringFunctions),
  "struct",
  6,
  -1
};
const String elem_struct = (String)ptrof(elem_struct_);

static struct private_stringStruct elem_super_ = {
  0,
  ptrof(predefStringFunctions),
  "super",
  5,
  -1
};
const String elem_super = (String)ptrof(elem_super_);

static struct private_stringStruct elem_switch_ = {
  0,
  ptrof(predefStringFunctions),
  "switch",
  6,
  -1
};
const String elem_switch = (String)ptrof(elem_switch_);

static struct private_stringStruct elem_synchronized_ = {
  0,
  ptrof(predefStringFunctions),
  "synchronized",
  12,
  -1
};
const String elem_synchronized = (String)ptrof(elem_synchronized_);

static struct private_stringStruct elem_this_ = {
  0,
  ptrof(predefStringFunctions),
  "this",
  4,
  -1
};
const String elem_this = (String)ptrof(elem_this_);

static struct private_stringStruct elem_throw_ = {
  0,
  ptrof(predefStringFunctions),
  "throw",
  5,
  -1
};
const String elem_throw = (String)ptrof(elem_throw_);

static struct private_stringStruct elem_throws_ = {
  0,
  ptrof(predefStringFunctions),
  "throws",
  6,
  -1
};
const String elem_throws = (String)ptrof(elem_throws_);

static struct private_stringStruct elem_transient_ = {
  0,
  ptrof(predefStringFunctions),
  "transient",
  9,
  -1
};
const String elem_transient = (String)ptrof(elem_transient_);

static struct private_stringStruct elem_true_ = {
  0,
  ptrof(predefStringFunctions),
  "true",
  4,
  -1
};
const String elem_true = (String)ptrof(elem_true_);

static struct private_stringStruct elem_TRUE_ = {
  0,
  ptrof(predefStringFunctions),
  "TRUE",
  4,
  -1
};
const String elem_TRUE = (String)ptrof(elem_TRUE_);

static struct private_stringStruct elem_try_ = {
  0,
  ptrof(predefStringFunctions),
  "try",
  3,
  -1
};
const String elem_try = (String)ptrof(elem_try_);

static struct private_stringStruct elem_typedef_ = {
  0,
  ptrof(predefStringFunctions),
  "typedef",
  7,
  -1
};
const String elem_typedef = (String)ptrof(elem_typedef_);

static struct private_stringStruct elem_undef_ = {
  0,
  ptrof(predefStringFunctions),
  "undef",
  5,
  -1
};
const String elem_undef = (String)ptrof(elem_undef_);

static struct private_stringStruct elem_union_ = {
  0,
  ptrof(predefStringFunctions),
  "union",
  5,
  -1
};
const String elem_union = (String)ptrof(elem_union_);

static struct private_stringStruct elem_unsigned_ = {
  0,
  ptrof(predefStringFunctions),
  "unsigned",
  8,
  -1
};
const String elem_unsigned = (String)ptrof(elem_unsigned_);

static struct private_stringStruct elem_va_list_ = {
  0,
  ptrof(predefStringFunctions),
  "va_list",
  7,
  -1
};
const String elem_va_list = (String)ptrof(elem_va_list_);

static struct private_stringStruct elem_virtual_ = {
  0,
  ptrof(predefStringFunctions),
  "virtual",
  7,
  -1
};
const String elem_virtual = (String)ptrof(elem_virtual_);

static struct private_stringStruct elem_void_ = {
  0,
  ptrof(predefStringFunctions),
  "void",
  4,
  -1
};
const String elem_void = (String)ptrof(elem_void_);

static struct private_stringStruct elem_volatile_ = {
  0,
  ptrof(predefStringFunctions),
  "volatile",
  8,
  -1
};
const String elem_volatile = (String)ptrof(elem_volatile_);

static struct private_stringStruct elem_while_ = {
  0,
  ptrof(predefStringFunctions),
  "while",
  5,
  -1
};
const String elem_while = (String)ptrof(elem_while_);

static struct private_stringStruct elem__Bool_ = {
  0,
  ptrof(predefStringFunctions),
  "_Bool",
  5,
  -1
};
const String elem__Bool = (String)ptrof(elem__Bool_);

static struct private_stringStruct elem__G_va_list_ = {
  0,
  ptrof(predefStringFunctions),
  "_G_va_list",
  10,
  -1
};
const String elem__G_va_list = (String)ptrof(elem__G_va_list_);

static struct private_stringStruct elem___attribute___ = {
  0,
  ptrof(predefStringFunctions),
  "__attribute__",
  13,
  -1
};
const String elem___attribute__ = (String)ptrof(elem___attribute___);

static struct private_stringStruct elem___const_ = {
  0,
  ptrof(predefStringFunctions),
  "__const",
  7,
  -1
};
const String elem___const = (String)ptrof(elem___const_);

static struct private_stringStruct elem___FILE___ = {
  0,
  ptrof(predefStringFunctions),
  "__FILE__",
  8,
  -1
};
const String elem___FILE__ = (String)ptrof(elem___FILE___);

static struct private_stringStruct elem___LINE___ = {
  0,
  ptrof(predefStringFunctions),
  "__LINE__",
  8,
  -1
};
const String elem___LINE__ = (String)ptrof(elem___LINE___);

static struct private_stringStruct elem___restrict_ = {
  0,
  ptrof(predefStringFunctions),
  "__restrict",
  10,
  -1
};
const String elem___restrict = (String)ptrof(elem___restrict_);

static struct private_stringStruct elem___signed_ = {
  0,
  ptrof(predefStringFunctions),
  "__signed",
  8,
  -1
};
const String elem___signed = (String)ptrof(elem___signed_);

static struct private_stringStruct elem___volatile_ = {
  0,
  ptrof(predefStringFunctions),
  "__volatile",
  10,
  -1
};
const String elem___volatile = (String)ptrof(elem___volatile_);
