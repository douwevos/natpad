/*
   File:    mulphpscanner.h
   Project: mule
   Author:  Douwe Vos
   Date:    Dec 8, 2016
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

#ifndef PHP_PARSER_MULPHPSCANNER_H_
#define PHP_PARSER_MULPHPSCANNER_H_

#include <caterpillar.h>
#include <grossruntime.h>

#define MUL_PHP_SYM_END_OF_INPUT                      0
#define MUL_PHP_SYM_ERROR                             1
#define MUL_PHP_SYM_T_INCLUDE                                                                         2
#define MUL_PHP_SYM_T_INCLUDE_ONCE                                                                    3
#define MUL_PHP_SYM_T_EVAL                                                                            4
#define MUL_PHP_SYM_T_REQUIRE                                                                         5
#define MUL_PHP_SYM_T_REQUIRE_ONCE                                                                    6
#define MUL_PHP_SYM_T_LOGICAL_OR                                                                      7
#define MUL_PHP_SYM_T_LOGICAL_XOR                                                                     8
#define MUL_PHP_SYM_T_LOGICAL_AND                                                                     9
#define MUL_PHP_SYM_T_INSTANCEOF                                                                      10
#define MUL_PHP_SYM_T_NEW                                                                             11
#define MUL_PHP_SYM_T_CLONE                                                                           12
#define MUL_PHP_SYM_T_EXIT                                                                            13
#define MUL_PHP_SYM_T_IF                                                                              14
#define MUL_PHP_SYM_T_ELSEIF                                                                          15
#define MUL_PHP_SYM_T_ELSE                                                                            16
#define MUL_PHP_SYM_T_ENDIF                                                                           17
#define MUL_PHP_SYM_T_ECHO                                                                            18
#define MUL_PHP_SYM_T_DO                                                                              19
#define MUL_PHP_SYM_T_WHILE                                                                           20
#define MUL_PHP_SYM_T_ENDWHILE                                                                        21
#define MUL_PHP_SYM_T_FOR                                                                             22
#define MUL_PHP_SYM_T_ENDFOR                                                                          23
#define MUL_PHP_SYM_T_FOREACH                                                                         24
#define MUL_PHP_SYM_T_ENDFOREACH                                                                      25
#define MUL_PHP_SYM_T_DECLARE                                                                         26
#define MUL_PHP_SYM_T_ENDDECLARE                                                                      27
#define MUL_PHP_SYM_T_AS                                                                              28
#define MUL_PHP_SYM_T_TRY                                                                             29
#define MUL_PHP_SYM_T_CATCH                                                                           30
#define MUL_PHP_SYM_T_FINALLY                                                                         31
#define MUL_PHP_SYM_T_THROW                                                                           32
#define MUL_PHP_SYM_T_USE                                                                             33
#define MUL_PHP_SYM_T_INSTEADOF                                                                       34
#define MUL_PHP_SYM_T_GLOBAL                                                                          35
#define MUL_PHP_SYM_T_VAR                                                                             36
#define MUL_PHP_SYM_T_UNSET                                                                           37
#define MUL_PHP_SYM_T_ISSET                                                                           38
#define MUL_PHP_SYM_T_EMPTY                                                                           39
#define MUL_PHP_SYM_T_CONTINUE                                                                        40
#define MUL_PHP_SYM_T_GOTO                                                                            41
#define MUL_PHP_SYM_T_FUNCTION                                                                        42
#define MUL_PHP_SYM_T_CONST                                                                           43
#define MUL_PHP_SYM_T_RETURN                                                                          44
#define MUL_PHP_SYM_T_PRINT                                                                           45
#define MUL_PHP_SYM_T_YIELD                                                                           46
#define MUL_PHP_SYM_T_LIST                                                                            47
#define MUL_PHP_SYM_T_SWITCH                                                                          48
#define MUL_PHP_SYM_T_ENDSWITCH                                                                       49
#define MUL_PHP_SYM_T_CASE                                                                            50
#define MUL_PHP_SYM_T_DEFAULT                                                                         51
#define MUL_PHP_SYM_T_BREAK                                                                           52
#define MUL_PHP_SYM_T_ARRAY                                                                           53
#define MUL_PHP_SYM_T_CALLABLE                                                                        54
#define MUL_PHP_SYM_T_EXTENDS                                                                         55
#define MUL_PHP_SYM_T_IMPLEMENTS                                                                      56
#define MUL_PHP_SYM_T_NAMESPACE                                                                       57
#define MUL_PHP_SYM_T_TRAIT                                                                           58
#define MUL_PHP_SYM_T_INTERFACE                                                                       59
#define MUL_PHP_SYM_T_CLASS                                                                           60
#define MUL_PHP_SYM_T_CLASS_C                                                                         61
#define MUL_PHP_SYM_T_TRAIT_C                                                                         62
#define MUL_PHP_SYM_T_FUNC_C                                                                          63
#define MUL_PHP_SYM_T_METHOD_C                                                                        64
#define MUL_PHP_SYM_T_LINE                                                                            65
#define MUL_PHP_SYM_T_FILE                                                                            66
#define MUL_PHP_SYM_T_DIR                                                                             67
#define MUL_PHP_SYM_T_NS_C                                                                            68
#define MUL_PHP_SYM_T_HALT_COMPILER                                                                   69
#define MUL_PHP_SYM_T_STATIC                                                                          70
#define MUL_PHP_SYM_T_ABSTRACT                                                                        71
#define MUL_PHP_SYM_T_FINAL                                                                           72
#define MUL_PHP_SYM_T_PRIVATE                                                                         73
#define MUL_PHP_SYM_T_PROTECTED                                                                       74
#define MUL_PHP_SYM_T_PUBLIC                                                                          75
#define MUL_PHP_SYM_T_STRING                                                                          76
#define MUL_PHP_SYM_T_NS_SEPARATOR                                                                    77
#define MUL_PHP_SYM_SEMI                                                                              78
#define MUL_PHP_SYM_LCUBRACK                                                                          79
#define MUL_PHP_SYM_RCUBRACK                                                                          80
#define MUL_PHP_SYM_COMMA                                                                             81
#define MUL_PHP_SYM_LPAREN                                                                            82
#define MUL_PHP_SYM_RPAREN                                                                            83
#define MUL_PHP_SYM_COLON                                                                             84
#define MUL_PHP_SYM_AMPERSAND                                                                         85
#define MUL_PHP_SYM_OP_PLUS                                                                           86
#define MUL_PHP_SYM_OP_MINUS                                                                          87
#define MUL_PHP_SYM_T_INLINE_HTML                                                                     88
#define MUL_PHP_SYM_T_DOUBLE_ARROW                                                                    89
#define MUL_PHP_SYM_T_VARIABLE                                                                        90
#define MUL_PHP_SYM_T_ELLIPSIS                                                                        91
#define MUL_PHP_SYM_T_PAAMAYIM_NEKUDOTAYIM                                                            92
#define MUL_PHP_SYM_T_PLUS_EQUAL                                                                      93
#define MUL_PHP_SYM_T_MINUS_EQUAL                                                                     94
#define MUL_PHP_SYM_T_MUL_EQUAL                                                                       95
#define MUL_PHP_SYM_T_DIV_EQUAL                                                                       96
#define MUL_PHP_SYM_T_CONCAT_EQUAL                                                                    97
#define MUL_PHP_SYM_T_MOD_EQUAL                                                                       98
#define MUL_PHP_SYM_T_AND_EQUAL                                                                       99
#define MUL_PHP_SYM_T_OR_EQUAL                                                                        100
#define MUL_PHP_SYM_T_XOR_EQUAL                                                                       101
#define MUL_PHP_SYM_T_SL_EQUAL                                                                        102
#define MUL_PHP_SYM_T_SR_EQUAL                                                                        103
#define MUL_PHP_SYM_T_POW_EQUAL                                                                       104
#define MUL_PHP_SYM_T_INC                                                                             105
#define MUL_PHP_SYM_T_DEC                                                                             106
#define MUL_PHP_SYM_T_BOOLEAN_OR                                                                      107
#define MUL_PHP_SYM_T_BOOLEAN_AND                                                                     108
#define MUL_PHP_SYM_T_SL                                                                              109
#define MUL_PHP_SYM_T_ENCAPSED_AND_WHITESPACE                                                         110
#define MUL_PHP_SYM_T_SR                                                                              111
#define MUL_PHP_SYM_T_POW                                                                             112
#define MUL_PHP_SYM_T_IS_IDENTICAL                                                                    113
#define MUL_PHP_SYM_QUESTION_MARK                                                                     114
#define MUL_PHP_SYM_LSQBRACK                                                                          115
#define MUL_PHP_SYM_RSQBRACK                                                                          116
#define MUL_PHP_SYM_T_DOLLAR_OPEN_CURLY_BRACES                                                        117
#define MUL_PHP_SYM_T_OBJECT_OPERATOR                                                                 118
#define MUL_PHP_SYM_T_CURLY_OPEN                                                                      119
#define MUL_PHP_SYM_T_NUM_STRING                                                                      120
#define MUL_PHP_SYM_T_STRING_VARNAME                                                                  121
#define MUL_PHP_SYM_T_START_HEREDOC                                                                   122
#define MUL_PHP_SYM_T_END_HEREDOC                                                                     123
#define MUL_PHP_SYM_T_CONSTANT_ENCAPSED_STRING                                                        124
#define MUL_PHP_SYM_T_LNUMBER                                                                         125
#define MUL_PHP_SYM_T_DNUMBER                                                                         126
#define MUL_PHP_SYM_T_IS_NOT_IDENTICAL                                                                127
#define MUL_PHP_SYM_T_IS_EQUAL                                                                        128
#define MUL_PHP_SYM_T_IS_NOT_EQUAL                                                                    129
#define MUL_PHP_SYM_T_SPACESHIP                                                                       130
#define MUL_PHP_SYM_T_IS_SMALLER_OR_EQUAL                                                             131
#define MUL_PHP_SYM_T_IS_GREATER_OR_EQUAL                                                             132
#define MUL_PHP_SYM_T_COALESCE                                                                        133
#define MUL_PHP_SYM_T_INT_CAST                                                                        134
#define MUL_PHP_SYM_T_DOUBLE_CAST                                                                     135
#define MUL_PHP_SYM_T_STRING_CAST                                                                     136
#define MUL_PHP_SYM_T_ARRAY_CAST                                                                      137
#define MUL_PHP_SYM_T_OBJECT_CAST                                                                     138
#define MUL_PHP_SYM_T_BOOL_CAST                                                                       139
#define MUL_PHP_SYM_T_UNSET_CAST                                                                      140
#define MUL_PHP_SYM_T_YIELD_FROM                                                                      141
#define MUL_PHP_SYM_DOLLAR                                                                            142
#define MUL_PHP_SYM_OP_ASSIGN                                                                         143
#define MUL_PHP_SYM_OP_OR                                                                             144
#define MUL_PHP_SYM_OP_POW                                                                            145
#define MUL_PHP_SYM_DOT                                                                               146
#define MUL_PHP_SYM_OP_MUL                                                                            147
#define MUL_PHP_SYM_OP_DIV                                                                            148
#define MUL_PHP_SYM_OP_MOD                                                                            149
#define MUL_PHP_SYM_OP_NOT                                                                            150
#define MUL_PHP_SYM_OP_NEG                                                                            151
#define MUL_PHP_SYM_OP_LT                                                                             152
#define MUL_PHP_SYM_OP_GT                                                                             153
#define MUL_PHP_SYM_AT                                                                                154
#define MUL_PHP_SYM_APASTOPH_BACKWARD                                                                 155
#define MUL_PHP_SYM_DOUBLE_QUOTE                                                                      156

#define MUL_PHP_SYM_COMMENT                                       157

G_BEGIN_DECLS

#define MUL_PHP_TYPE_SCANNER              (mul_php_scanner_get_type())
#define MUL_PHP_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_php_scanner_get_type(), MulPhpScanner))
#define MUL_PHP_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_PHP_TYPE_SCANNER, MulPhpScannerClass))
#define MUL_PHP_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_PHP_TYPE_SCANNER))
#define MUL_PHP_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_PHP_TYPE_SCANNER))
#define MUL_PHP_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_PHP_TYPE_SCANNER, MulPhpScannerClass))


typedef struct _MulPhpScanner               MulPhpScanner;
typedef struct _MulPhpScannerPrivate        MulPhpScannerPrivate;
typedef struct _MulPhpScannerClass          MulPhpScannerClass;


struct _MulPhpScanner {
	GroRunScannerBase parent;
};

struct _MulPhpScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType mul_php_scanner_get_type();

MulPhpScanner *mul_php_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);

gboolean mul_php_scanner_is_eof(MulPhpScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* PHP_PARSER_MULPHPSCANNER_H_ */
