/*
   File:    jagpscanner.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Mar 9, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef JAGPSCANNER_H_
#define JAGPSCANNER_H_

#include <grossruntime.h>

G_BEGIN_DECLS

#define JAGP_SYM_END_OF_INPUT                                                                       0
#define JAGP_SYM_ERROR                                                                              1
#define JAGP_SYM_L_LSQBRACE                                                                         2
#define JAGP_SYM_L_RSQBRACE                                                                         3
#define JAGP_SYM_L_ARROW                                                                            4
#define JAGP_SYM_L_AND                                                                              5
#define JAGP_SYM_L_LT                                                                               6
#define JAGP_SYM_L_GT                                                                               7
#define JAGP_SYM_L_LE                                                                               8
#define JAGP_SYM_L_GE                                                                               9
#define JAGP_SYM_L_EQ                                                                               10
#define JAGP_SYM_L_NE                                                                               11
#define JAGP_SYM_L_COMMA                                                                            12
#define JAGP_SYM_L_QUESTION_MARK                                                                    13
#define JAGP_SYM_L_DOT                                                                              14
#define JAGP_SYM_L_XOR                                                                              15
#define JAGP_SYM_L_MUL                                                                              16
#define JAGP_SYM_L_DIV                                                                              17
#define JAGP_SYM_L_MOD                                                                              18
#define JAGP_SYM_L_SUB                                                                              19
#define JAGP_SYM_L_ADD                                                                              20
#define JAGP_SYM_L_IS                                                                               21
#define JAGP_SYM_L_NOT                                                                              22
#define JAGP_SYM_L_NEG                                                                              23
#define JAGP_SYM_L_LPAREN                                                                           24
#define JAGP_SYM_L_RPAREN                                                                           25
#define JAGP_SYM_L_LCUBRACE                                                                         26
#define JAGP_SYM_L_RCUBRACE                                                                         27
#define JAGP_SYM_L_VARARGS                                                                          28
#define JAGP_SYM_L_AT                                                                               29
#define JAGP_SYM_L_INC                                                                              30
#define JAGP_SYM_L_DEC                                                                              31
#define JAGP_SYM_L_COLON                                                                            32
#define JAGP_SYM_L_SEMI                                                                             33
#define JAGP_SYM_L_COLON_COLON                                                                      34
#define JAGP_SYM_L_BAR                                                                              35
#define JAGP_SYM_L_LOR                                                                              36
#define JAGP_SYM_L_LAND                                                                             37
#define JAGP_SYM_L_MUL_IS                                                                           38
#define JAGP_SYM_L_DIV_IS                                                                           39
#define JAGP_SYM_L_MOD_IS                                                                           40
#define JAGP_SYM_L_ADD_IS                                                                           41
#define JAGP_SYM_L_SUB_IS                                                                           42
#define JAGP_SYM_L_SHL_IS                                                                           43
#define JAGP_SYM_L_SHR_IS                                                                           44
#define JAGP_SYM_L_SHRR_IS                                                                          45
#define JAGP_SYM_L_AND_IS                                                                           46
#define JAGP_SYM_L_XOR_IS                                                                           47
#define JAGP_SYM_L_OR_IS                                                                            48
#define JAGP_SYM_K_BOOLEAN                                                                          49
#define JAGP_SYM_K_BYTE                                                                             50
#define JAGP_SYM_K_SHORT                                                                            51
#define JAGP_SYM_K_INT                                                                              52
#define JAGP_SYM_K_LONG                                                                             53
#define JAGP_SYM_K_CHAR                                                                             54
#define JAGP_SYM_K_DOUBLE                                                                           55
#define JAGP_SYM_K_FLOAT                                                                            56
#define JAGP_SYM_IDENTIFIER                                                                         57
#define JAGP_SYM_LITERAL                                                                            58
#define JAGP_SYM_K_EXTENDS                                                                          59
#define JAGP_SYM_K_SUPER                                                                            60
#define JAGP_SYM_K_THIS                                                                             61
#define JAGP_SYM_K_PACKAGE                                                                          62
#define JAGP_SYM_K_PUBLIC                                                                           63
#define JAGP_SYM_K_PRIVATE                                                                          64
#define JAGP_SYM_K_PROTECTED                                                                        65
#define JAGP_SYM_K_IMPORT                                                                           66
#define JAGP_SYM_K_CLASS                                                                            67
#define JAGP_SYM_K_INTERFACE                                                                        68
#define JAGP_SYM_K_ABSTRACT                                                                         69
#define JAGP_SYM_K_STATIC                                                                           70
#define JAGP_SYM_K_FINAL                                                                            71
#define JAGP_SYM_K_STRICTFP                                                                         72
#define JAGP_SYM_K_IMPLEMENTS                                                                       73
#define JAGP_SYM_K_TRANSIENT                                                                        74
#define JAGP_SYM_K_VOLATILE                                                                         75
#define JAGP_SYM_K_SYNCHRONIZED                                                                     76
#define JAGP_SYM_K_NATIVE                                                                           77
#define JAGP_SYM_K_VOID                                                                             78
#define JAGP_SYM_K_THROWS                                                                           79
#define JAGP_SYM_K_ENUM                                                                             80
#define JAGP_SYM_K_DEFAULT                                                                          81
#define JAGP_SYM_K_IF                                                                               82
#define JAGP_SYM_K_ASSERT                                                                           83
#define JAGP_SYM_K_SWITCH                                                                           84
#define JAGP_SYM_K_CASE                                                                             85
#define JAGP_SYM_K_BREAK                                                                            86
#define JAGP_SYM_K_ELSE                                                                             87
#define JAGP_SYM_K_DO                                                                               88
#define JAGP_SYM_K_WHILE                                                                            89
#define JAGP_SYM_K_FOR                                                                              90
#define JAGP_SYM_K_NEW                                                                              91
#define JAGP_SYM_K_INSTANCEOF                                                                       92
#define JAGP_SYM_K_TRY                                                                              93
#define JAGP_SYM_K_CATCH                                                                            94
#define JAGP_SYM_K_FINALLY                                                                          95
#define JAGP_SYM_K_CONTINUE                                                                         96
#define JAGP_SYM_K_THROW                                                                            97
#define JAGP_SYM_L_LPARREN2                                                                         98
#define JAGP_SYM_K_RETURN                                                                           99
#define JAGP_SYM_EOL_COMMENT                                                                        100
#define JAGP_SYM_FULL_COMMENT                                                                       101

#define JAGP_TYPE_SCANNER              (jagp_scanner_get_type())
#define JAGP_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_scanner_get_type(), JagPScanner))
#define JAGP_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_SCANNER, JagPScannerClass))
#define JAGP_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_SCANNER))
#define JAGP_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_SCANNER))
#define JAGP_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_SCANNER, JagPScannerClass))


typedef struct _JagPScanner               JagPScanner;
typedef struct _JagPScannerPrivate        JagPScannerPrivate;
typedef struct _JagPScannerClass          JagPScannerClass;


struct _JagPScanner {
	GroRunScannerBase parent;
};

struct _JagPScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType jagp_scanner_get_type();

JagPScanner *jagp_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);

gboolean jagp_scanner_is_eof(JagPScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* JAGPSCANNER_H_ */
