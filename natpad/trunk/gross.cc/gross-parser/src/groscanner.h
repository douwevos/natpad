/*
   File:    groscanner.h
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Nov 18, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef PARSER_GROSCANNER_H_
#define PARSER_GROSCANNER_H_


#define GRO_SYM_END_OF_INPUT         0
#define GRO_SYM_ERROR                1
#define GRO_SYM_NUMBER               2
#define GRO_SYM_RULE_SEPERATOR       3
#define GRO_SYM_INDEX_MARK           4
#define GRO_SYM_COMMA                5
#define GRO_SYM_COLON                6
#define GRO_SYM_SEMI                 7
#define GRO_SYM_BAR                  8
#define GRO_SYM_EXCL                 9
#define GRO_SYM_STAR                 10
#define GRO_SYM_PLUS                 11
#define GRO_SYM_LPAREN               12
#define GRO_SYM_RPAREN               13
#define GRO_SYM_LSQBRACK             14
#define GRO_SYM_RSQBRACK             15
#define GRO_SYM_LCUBRACK             16
#define GRO_SYM_RCUBRACK             17
#define GRO_SYM_KW_TERMINAL          18
#define GRO_SYM_KW_CODE              19
#define GRO_SYM_KW_TYPE              20
#define GRO_SYM_KW_IMPORTS           21
#define GRO_SYM_IDENTIFIER           22
#define GRO_SYM_CODE_TEXT            23
#define GRO_SYM_COLON_DOT            24
#define GRO_SYM_EOL_COMMENT          25

#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define GRO_TYPE_SCANNER              (gro_scanner_get_type())
#define GRO_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_scanner_get_type(), GroScanner))
#define GRO_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_SCANNER, GroScannerClass))
#define GRO_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_SCANNER))
#define GRO_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_SCANNER))
#define GRO_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_SCANNER, GroScannerClass))


typedef struct _GroScanner               GroScanner;
typedef struct _GroScannerPrivate        GroScannerPrivate;
typedef struct _GroScannerClass          GroScannerClass;


struct _GroScanner {
	GroRunScannerBase parent;
};

struct _GroScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType gro_scanner_get_type();

GroScanner *gro_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);

gboolean gro_scanner_is_eof(GroScanner *scanner, GroRunIToken *token);


G_END_DECLS

#endif /* PARSER_GROSCANNER_H_ */
