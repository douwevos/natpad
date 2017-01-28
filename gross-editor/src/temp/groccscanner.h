///*
//   File:    groccscanner.h
//   Project: gross-editor
//   Author:  Douwe Vos
//   Date:    Oct 12, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#ifndef TEMP_GROCCSCANNER_H_
//#define TEMP_GROCCSCANNER_H_
//
//#include <caterpillar.h>
//#include <grossruntime.h>
//
//G_BEGIN_DECLS
//
//#define SYM_END_OF_INPUT         0
//#define SYM_NUMBER               1
//#define SYM_RULE_SEPERATOR       2
//#define SYM_INDEX_MARK           3
//#define SYM_COMMA                4
//#define SYM_COLON                5
//#define SYM_SEMI                 6
//#define SYM_BAR                  7
//#define SYM_EXCL                 8
//#define SYM_STAR                 9
//#define SYM_PLUS                 10
//#define SYM_LPAREN               11
//#define SYM_RPAREN               12
//#define SYM_LSQBRACK             13
//#define SYM_RSQBRACK             14
//#define SYM_LCUBRACK             15
//#define SYM_RCUBRACK             16
//#define SYM_KW_TERMINAL          17
//#define SYM_KW_CODE              18
//#define SYM_KW_TYPE              19
//#define SYM_KW_IMPORTS           20
//#define SYM_IDENTIFIER           21
//#define SYM_MULTI_LINE_TEXT      22
//
//
//
//#define GROCC_TYPE_SCANNER              (grocc_scanner_get_type())
//#define GROCC_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grocc_scanner_get_type(), GroCCScanner))
//#define GROCC_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROCC_TYPE_SCANNER, GroCCScannerClass))
//#define GROCC_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROCC_TYPE_SCANNER))
//#define GROCC_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROCC_TYPE_SCANNER))
//#define GROCC_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROCC_TYPE_SCANNER, GroCCScannerClass))
//
//
//typedef struct _GroCCScanner               GroCCScanner;
//typedef struct _GroCCScannerPrivate        GroCCScannerPrivate;
//typedef struct _GroCCScannerClass          GroCCScannerClass;
//
//
//struct _GroCCScanner {
//	GObject parent;
//};
//
//struct _GroCCScannerClass {
//	GObjectClass parent_class;
//};
//
//
//GType grocc_scanner_get_type();
//
//GroCCScanner *grocc_scanner_new();
//
//gboolean grocc_scanner_is_eof(GroCCScanner *scanner, GroRunIToken *token);
//
//int grocc_scanner_get_symbol_index(GroCCScanner *scanner, GroRunSymbol *sym);
//
//G_END_DECLS
//
//#endif /* TEMP_GROCCSCANNER_H_ */
