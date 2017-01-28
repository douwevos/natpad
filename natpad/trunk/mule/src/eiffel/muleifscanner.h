/*
   File:    muleifscanner.h
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 13, 2016
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

#ifndef EIFFEL_MULEIFSCANNER_H_
#define EIFFEL_MULEIFSCANNER_H_

#include <caterpillar.h>
#include <grossruntime.h>

#define MUL_EIF_SYM_END_OF_INPUT                      0
#define MUL_EIF_SYM_ERROR                             1

#define MUL_EIF_SYM_KWDEFERRED                        2
#define MUL_EIF_SYM_KWEXPANDED                        3
#define MUL_EIF_SYM_KWSEPARATE                        4
#define MUL_EIF_SYM_KWCLASS                           5
#define MUL_EIF_SYM_IDENTIFIER                        6
#define MUL_EIF_SYM_KWEND                             7
#define MUL_EIF_SYM_KWINDEXING                        8
#define MUL_EIF_SYM_SEMICOLON                         9
#define MUL_EIF_SYM_COLON                             10
#define MUL_EIF_SYM_COMMA                             11
#define MUL_EIF_SYM_ARROW                             12
#define MUL_EIF_SYM_KWOBSOLETE                        13
#define MUL_EIF_SYM_KWINHERIT                         14
#define MUL_EIF_SYM_KWRENAME                          15
#define MUL_EIF_SYM_KWAS                              16
#define MUL_EIF_SYM_KWLIKE                            17
#define MUL_EIF_SYM_KWCURRENT                         18
#define MUL_EIF_SYM_KWBIT                             19
#define MUL_EIF_SYM_INTEGER_CONSTANT                  20
#define MUL_EIF_SYM_KWUNDEFINE                        21
#define MUL_EIF_SYM_KWREDEFINE                        22
#define MUL_EIF_SYM_KWEXPORT                          23
#define MUL_EIF_SYM_KWSELECT                          24
#define MUL_EIF_SYM_KWALL                             25
#define MUL_EIF_SYM_KWCREATE                          26
#define MUL_EIF_SYM_KWFEATURE                         27
#define MUL_EIF_SYM_KWFROZEN                          28
#define MUL_EIF_SYM_KWPREFIX                          29
#define MUL_EIF_SYM_KWINFIX                           30
#define MUL_EIF_SYM_QUOTED_UNARY                      31
#define MUL_EIF_SYM_QUOTED_BINARY                     32
#define MUL_EIF_SYM_QUOTED_FREE_OPERATOR              33
#define MUL_EIF_SYM_QUESTION_MARK                     34
#define MUL_EIF_SYM_LPAREN                            35
#define MUL_EIF_SYM_RPAREN                            36
#define MUL_EIF_SYM_KWIS                              37
#define MUL_EIF_SYM_KWUNIQUE                          38
#define MUL_EIF_SYM_KWDO                              39
#define MUL_EIF_SYM_KWONCE                            40
#define MUL_EIF_SYM_KWEXTERNAL                        41
#define MUL_EIF_SYM_MANIFEST_STRING                   42
#define MUL_EIF_SYM_KWALIAS                           43
#define MUL_EIF_SYM_KWLOCAL                           44
#define MUL_EIF_SYM_KWREQUIRE                         45
#define MUL_EIF_SYM_KWELSE                            46
#define MUL_EIF_SYM_KWENSURE                          47
#define MUL_EIF_SYM_KWTHEN                            48
#define MUL_EIF_SYM_KWINVARIANT                       49
#define MUL_EIF_SYM_KWVARIANT                         50
#define MUL_EIF_SYM_COMMENT                           51
#define MUL_EIF_SYM_KWRESCUE                          52
#define MUL_EIF_SYM_EXCLAMATION                       53
#define MUL_EIF_SYM_DOT                               54
#define MUL_EIF_SYM_COLON_IS                          55
#define MUL_EIF_SYM_QUESTION_MARK_IS                  56
#define MUL_EIF_SYM_KWIF                              57
#define MUL_EIF_SYM_KWELSEIF                          58
#define MUL_EIF_SYM_KWINSPECT                         59
#define MUL_EIF_SYM_KWWHEN                            60
#define MUL_EIF_SYM_DOUBLE_DOT                        61
#define MUL_EIF_SYM_CHARACTER_CONSTANT                62
#define MUL_EIF_SYM_KWFROM                            63
#define MUL_EIF_SYM_KWLOOP                            64
#define MUL_EIF_SYM_KWUNTIL                           65
#define MUL_EIF_SYM_KWDEBUG                           66
#define MUL_EIF_SYM_KWCHECK                           67
#define MUL_EIF_SYM_KWRETRY                           68
#define MUL_EIF_SYM_KWRESULT                          69
#define MUL_EIF_SYM_KWPRECURSOR                       70
#define MUL_EIF_SYM_DOLLAR                            71
#define MUL_EIF_SYM_BIT_CONSTANT                      72
#define MUL_EIF_SYM_INTEGER                           73
#define MUL_EIF_SYM_REAL                              74
#define MUL_EIF_SYM_HEXADECIMAL_CONSTANT              75
#define MUL_EIF_SYM_PREFIX_OPERATOR                   76
#define MUL_EIF_SYM_INFIX_OPERATOR                    77
#define MUL_EIF_SYM_OP_IS                             78
#define MUL_EIF_SYM_OP_FWSLASH_IS                     79
#define MUL_EIF_SYM_KWTRUE                            80
#define MUL_EIF_SYM_KWFALSE                           81
#define MUL_EIF_SYM_OP_PLUS                           82
#define MUL_EIF_SYM_OP_MINUS                          83
#define MUL_EIF_SYM_LTLT                              84
#define MUL_EIF_SYM_GTGT                              85
#define MUL_EIF_SYM_KWOLD                             86
#define MUL_EIF_SYM_KWSTRIP                           87
#define MUL_EIF_SYM_KWNOTE                            88

G_BEGIN_DECLS

#define MUL_EIF_TYPE_SCANNER              (mul_eif_scanner_get_type())
#define MUL_EIF_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_eif_scanner_get_type(), MulEifScanner))
#define MUL_EIF_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_EIF_TYPE_SCANNER, MulEifScannerClass))
#define MUL_EIF_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_EIF_TYPE_SCANNER))
#define MUL_EIF_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_EIF_TYPE_SCANNER))
#define MUL_EIF_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_EIF_TYPE_SCANNER, MulEifScannerClass))


typedef struct _MulEifScanner               MulEifScanner;
typedef struct _MulEifScannerClass          MulEifScannerClass;


struct _MulEifScanner {
	GroRunScannerBase parent;
};

struct _MulEifScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType mul_eif_scanner_get_type();

MulEifScanner *mul_eif_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);

gboolean mul_eif_scanner_is_eof(MulEifScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* EIFFEL_MULEIFSCANNER_H_ */
