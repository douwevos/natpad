/*
   File:    tergrammarreader.h
   Project: tern
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

#include <glib-object.h>
#include <caterpillar.h>
#include "tersyntax.h"

#ifndef TERGRAMMARREADER_H_
#define TERGRAMMARREADER_H_

G_BEGIN_DECLS

extern CatS ter_s_sect_keywords_1;
extern CatS ter_s_sect_keywords_2;
extern CatS ter_s_sect_keywords_3;
extern CatS ter_s_sect_keywords_4;
extern CatS ter_s_sect_keywords_5;
extern CatS ter_s_sect_syntax;

extern CatS ter_s_ignore_case;
extern CatS ter_s_syntax_start;
extern CatS ter_s_bracket_chars;
extern CatS ter_s_operator_chars;
extern CatS ter_s_namespace1;
extern CatS ter_s_key_word_length;
extern CatS ter_s_syntax_end;
extern CatS ter_s_hex_prefix;
extern CatS ter_s_preproc_start;
extern CatS ter_s_char_esc;
extern CatS ter_s_char_start;
extern CatS ter_s_char_end;
extern CatS ter_s_comment_start_alt;
extern CatS ter_s_comment_end_alt;
extern CatS ter_s_comment_start;
extern CatS ter_s_comment_end;
extern CatS ter_s_single_comment_col;
extern CatS ter_s_single_comment_esc;
extern CatS ter_s_single_comment_alt;
extern CatS ter_s_single_comment_col_alt;
extern CatS ter_s_single_comment;
extern CatS ter_s_string_end;
extern CatS ter_s_string_start;
extern CatS ter_s_strings_span_lines;
extern CatS ter_s_string_esc;
extern CatS ter_s_string_alt;

#define TER_TYPE_GRAMMAR_READER            (ter_grammar_reader_get_type())
#define TER_GRAMMAR_READER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_grammar_reader_get_type(), TerGrammarReader))
#define TER_GRAMMAR_READER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_GRAMMAR_READER, TerGrammarReaderClass))
#define TER_IS_GRAMMAR_READER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_GRAMMAR_READER))
#define TER_IS_GRAMMAR_READER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_GRAMMAR_READER))
#define TER_GRAMMAR_READER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_GRAMMAR_READER, TerGrammarReaderClass))

typedef struct _TerGrammarReader	      TerGrammarReader;
typedef struct _TerGrammarReaderClass	  TerGrammarReaderClass;

struct _TerGrammarReader {
	GObject parent;
};

struct _TerGrammarReaderClass {
	GObjectClass parent_class;
};

TerGrammarReader *ter_grammar_reader_new();


TerSyntax *ter_grammar_reader_read(CatIInputStream *in_stream);

G_END_DECLS

#endif /* TERGRAMMARREADER_H_ */
