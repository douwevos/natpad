/*
   File:    armpscanner.h
   Project: armadilla-parser
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

#ifndef ARMPSCANNER_H_
#define ARMPSCANNER_H_

#include <grossruntime.h>

G_BEGIN_DECLS

#define ARMP_SYM_END_OF_INPUT                                                                       0
#define ARMP_SYM_ERROR                                                                              1
#define ARMP_SYM_IDENTIFIER                                                                         2
#define ARMP_SYM_I_CONSTANT                                                                         3
#define ARMP_SYM_F_CONSTANT                                                                         4
#define ARMP_SYM_STRING_LITERAL                                                                     5
#define ARMP_SYM_FUNC_NAME                                                                          6
#define ARMP_SYM_SIZEOF                                                                             7
#define ARMP_SYM_PTR_OP                                                                             8
#define ARMP_SYM_INC_OP                                                                             9
#define ARMP_SYM_DEC_OP                                                                             10
#define ARMP_SYM_LEFT_OP                                                                            11
#define ARMP_SYM_RIGHT_OP                                                                           12
#define ARMP_SYM_LE_OP                                                                              13
#define ARMP_SYM_GE_OP                                                                              14
#define ARMP_SYM_EQ_OP                                                                              15
#define ARMP_SYM_NE_OP                                                                              16
#define ARMP_SYM_AND_OP                                                                             17
#define ARMP_SYM_OR_OP                                                                              18
#define ARMP_SYM_MUL_ASSIGN                                                                         19
#define ARMP_SYM_DIV_ASSIGN                                                                         20
#define ARMP_SYM_MOD_ASSIGN                                                                         21
#define ARMP_SYM_ADD_ASSIGN                                                                         22
#define ARMP_SYM_SUB_ASSIGN                                                                         23
#define ARMP_SYM_LEFT_ASSIGN                                                                        24
#define ARMP_SYM_RIGHT_ASSIGN                                                                       25
#define ARMP_SYM_AND_ASSIGN                                                                         26
#define ARMP_SYM_XOR_ASSIGN                                                                         27
#define ARMP_SYM_OR_ASSIGN                                                                          28
#define ARMP_SYM_TYPEDEF_NAME                                                                       29
#define ARMP_SYM_ENUMERATION_CONSTANT                                                               30
#define ARMP_SYM_TYPEDEF                                                                            31
#define ARMP_SYM_EXTERN                                                                             32
#define ARMP_SYM_STATIC                                                                             33
#define ARMP_SYM_AUTO                                                                               34
#define ARMP_SYM_REGISTER                                                                           35
#define ARMP_SYM_INLINE                                                                             36
#define ARMP_SYM_CONST                                                                              37
#define ARMP_SYM_RESTRICT                                                                           38
#define ARMP_SYM_VOLATILE                                                                           39
#define ARMP_SYM_BOOL                                                                               40
#define ARMP_SYM_CHAR                                                                               41
#define ARMP_SYM_SHORT                                                                              42
#define ARMP_SYM_INT                                                                                43
#define ARMP_SYM_LONG                                                                               44
#define ARMP_SYM_SIGNED                                                                             45
#define ARMP_SYM_UNSIGNED                                                                           46
#define ARMP_SYM_FLOAT                                                                              47
#define ARMP_SYM_DOUBLE                                                                             48
#define ARMP_SYM_VOID                                                                               49
#define ARMP_SYM_COMPLEX                                                                            50
#define ARMP_SYM_IMAGINARY                                                                          51
#define ARMP_SYM_STRUCT                                                                             52
#define ARMP_SYM_UNION                                                                              53
#define ARMP_SYM_ENUM                                                                               54
#define ARMP_SYM_ELLIPSIS                                                                           55
#define ARMP_SYM_CASE                                                                               56
#define ARMP_SYM_DEFAULT                                                                            57
#define ARMP_SYM_IF                                                                                 58
#define ARMP_SYM_ELSE                                                                               59
#define ARMP_SYM_SWITCH                                                                             60
#define ARMP_SYM_WHILE                                                                              61
#define ARMP_SYM_DO                                                                                 62
#define ARMP_SYM_FOR                                                                                63
#define ARMP_SYM_GOTO                                                                               64
#define ARMP_SYM_CONTINUE                                                                           65
#define ARMP_SYM_BREAK                                                                              66
#define ARMP_SYM_RETURN                                                                             67
#define ARMP_SYM_ALIGNAS                                                                            68
#define ARMP_SYM_ALIGNOF                                                                            69
#define ARMP_SYM_ATOMIC                                                                             70
#define ARMP_SYM_GENERIC                                                                            71
#define ARMP_SYM_NORETURN                                                                           72
#define ARMP_SYM_STATIC_ASSERT                                                                      73
#define ARMP_SYM_THREAD_LOCAL                                                                       74
#define ARMP_SYM_L_BAR                                                                              75
#define ARMP_SYM_L_SEMI                                                                             76
#define ARMP_SYM_L_COMMA                                                                            77
#define ARMP_SYM_L_DOT                                                                              78
#define ARMP_SYM_L_COLON                                                                            79
#define ARMP_SYM_L_QUESTIONMARK                                                                     80
#define ARMP_SYM_L_IS                                                                               81
#define ARMP_SYM_L_AND                                                                              82
#define ARMP_SYM_L_MUL                                                                              83
#define ARMP_SYM_L_PLUS                                                                             84
#define ARMP_SYM_L_MINUS                                                                            85
#define ARMP_SYM_L_LT                                                                               86
#define ARMP_SYM_L_GT                                                                               87
#define ARMP_SYM_L_NEG                                                                              88
#define ARMP_SYM_L_NOT                                                                              89
#define ARMP_SYM_L_DIV                                                                              90
#define ARMP_SYM_L_MOD                                                                              91
#define ARMP_SYM_L_XOR                                                                              92
#define ARMP_SYM_L_LCUBRACE                                                                         93
#define ARMP_SYM_L_RCUBRACE                                                                         94
#define ARMP_SYM_L_LSQBRACE                                                                         95
#define ARMP_SYM_L_RSQBRACE                                                                         96
#define ARMP_SYM_L_LPAREN                                                                           97
#define ARMP_SYM_L_RPAREN                                                                           98
#define ARMP_SYM_EOL_COMMENT                                                                        99
#define ARMP_SYM_FULL_COMMENT                                                                       100

#define ARMP_TYPE_SCANNER              (armp_scanner_get_type())
#define ARMP_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), armp_scanner_get_type(), ArmPScanner))
#define ARMP_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ARMP_TYPE_SCANNER, ArmPScannerClass))
#define ARMP_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARMP_TYPE_SCANNER))
#define ARMP_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ARMP_TYPE_SCANNER))
#define ARMP_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ARMP_TYPE_SCANNER, ArmPScannerClass))


typedef struct _ArmPScanner               ArmPScanner;
typedef struct _ArmPScannerPrivate        ArmPScannerPrivate;
typedef struct _ArmPScannerClass          ArmPScannerClass;


struct _ArmPScanner {
	GroRunScannerBase parent;
};

struct _ArmPScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType armp_scanner_get_type();

ArmPScanner *armp_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);

gboolean armp_scanner_is_eof(ArmPScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* ARMPSCANNER_H_ */
