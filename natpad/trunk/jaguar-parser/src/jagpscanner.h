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
#define JAGP_SYM_ABSTRACT                                                                           2
#define JAGP_SYM_DO                                                                                 3
#define JAGP_SYM_IMPLEMENTS                                                                         4
#define JAGP_SYM_PACKAGE                                                                            5
#define JAGP_SYM_THROW                                                                              6
#define JAGP_SYM_BOOLEAN                                                                            7
#define JAGP_SYM_DOUBLE                                                                             8
#define JAGP_SYM_IMPORT                                                                             9
#define JAGP_SYM_PRIVATE                                                                            10
#define JAGP_SYM_THROWS                                                                             11
#define JAGP_SYM_BREAK                                                                              12
#define JAGP_SYM_ELSE                                                                               13
#define JAGP_SYM_PROTECTED                                                                          14
#define JAGP_SYM_TRANSIENT                                                                          15
#define JAGP_SYM_BYTE                                                                               16
#define JAGP_SYM_SHORT                                                                              17
#define JAGP_SYM_INT                                                                                18
#define JAGP_SYM_LONG                                                                               19
#define JAGP_SYM_CHAR                                                                               20
#define JAGP_SYM_EXTENDS                                                                            21
#define JAGP_SYM_INSTANCEOF                                                                         22
#define JAGP_SYM_PUBLIC                                                                             23
#define JAGP_SYM_TRY                                                                                24
#define JAGP_SYM_CASE                                                                               25
#define JAGP_SYM_FINAL                                                                              26
#define JAGP_SYM_FINALLY                                                                            27
#define JAGP_SYM_INTERFACE                                                                          28
#define JAGP_SYM_RETURN                                                                             29
#define JAGP_SYM_VOID                                                                               30
#define JAGP_SYM_CATCH                                                                              31
#define JAGP_SYM_FLOAT                                                                              32
#define JAGP_SYM_VOLATILE                                                                           33
#define JAGP_SYM_FOR                                                                                34
#define JAGP_SYM_NATIVE                                                                             35
#define JAGP_SYM_STATIC                                                                             36
#define JAGP_SYM_WHILE                                                                              37
#define JAGP_SYM_CLASS                                                                              38
#define JAGP_SYM_NEW                                                                                39
#define JAGP_SYM_SUPER                                                                              40
#define JAGP_SYM_CONST                                                                              41
#define JAGP_SYM_NULL                                                                               42
#define JAGP_SYM_SWITCH                                                                             43
#define JAGP_SYM_CONTINUE                                                                           44
#define JAGP_SYM_GOTO                                                                               45
#define JAGP_SYM_SYNCHRONIZED                                                                       46
#define JAGP_SYM_DEFAULT                                                                            47
#define JAGP_SYM_IF                                                                                 48
#define JAGP_SYM_THIS                                                                               49
#define JAGP_SYM_LPAREN                                                                             50
#define JAGP_SYM_RPAREN                                                                             51
#define JAGP_SYM_LBRACE                                                                             52
#define JAGP_SYM_RBRACE                                                                             53
#define JAGP_SYM_LBRACK                                                                             54
#define JAGP_SYM_RBRACK                                                                             55
#define JAGP_SYM_IDENTIFIER                                                                         56
#define JAGP_SYM_DOT                                                                                57
#define JAGP_SYM_COMMA                                                                              58
#define JAGP_SYM_COLON                                                                              59
#define JAGP_SYM_SEMICOLON                                                                          60
#define JAGP_SYM_STRING_LITERAL                                                                     61
#define JAGP_SYM_BOOLEAN_LITERAL                                                                    62
#define JAGP_SYM_QUESTION                                                                           63
#define JAGP_SYM_OP_MUL                                                                             64
#define JAGP_SYM_OP_DIV                                                                             65
#define JAGP_SYM_OP_SUB                                                                             66
#define JAGP_SYM_OP_ADD                                                                             67
#define JAGP_SYM_OP_AND                                                                             68
#define JAGP_SYM_OP_OR                                                                              69
#define JAGP_SYM_OP_XOR                                                                             70
#define JAGP_SYM_OP_NOT                                                                             71
#define JAGP_SYM_OP_NEG                                                                             72
#define JAGP_SYM_OP_LAND                                                                            73
#define JAGP_SYM_OP_INC                                                                             74
#define JAGP_SYM_OP_DEC                                                                             75
#define JAGP_SYM_OP_MOD                                                                             76
#define JAGP_SYM_OP_LOR                                                                             77
#define JAGP_SYM_OP_SHL                                                                             78
#define JAGP_SYM_OP_SHR                                                                             79
#define JAGP_SYM_OP_SHRR                                                                            80
#define JAGP_SYM_OP_EQ                                                                              81
#define JAGP_SYM_OP_NE                                                                              82
#define JAGP_SYM_OP_GE                                                                              83
#define JAGP_SYM_OP_GT                                                                              84
#define JAGP_SYM_OP_LE                                                                              85
#define JAGP_SYM_OP_LT                                                                              86
#define JAGP_SYM_ASS_MUL                                                                            87
#define JAGP_SYM_ASS_DIV                                                                            88
#define JAGP_SYM_ASS_MOD                                                                            89
#define JAGP_SYM_ASS_ADD                                                                            90
#define JAGP_SYM_ASS_SUB                                                                            91
#define JAGP_SYM_ASS_SHL                                                                            92
#define JAGP_SYM_ASS_SHR                                                                            93
#define JAGP_SYM_ASS_SHRR                                                                           94
#define JAGP_SYM_ASS_AND                                                                            95
#define JAGP_SYM_ASS_XOR                                                                            96
#define JAGP_SYM_ASS_OR                                                                             97
#define JAGP_SYM_ASS_IS                                                                             98
#define JAGP_SYM_AT                                                                                 99
#define JAGP_SYM_INTEGER_LITERAL                                                                    100
#define JAGP_SYM_FLOATING_POINT_LITERAL                                                             101
#define JAGP_SYM_CHARACTER_LITERAL                                                                  102
#define JAGP_SYM_STRICTFP                                                                           103
#define JAGP_SYM_ASSERT                                                                             104
#define JAGP_SYM_ELLIPSIS                                                                           105
#define JAGP_SYM_ENUM                                                                               106

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
