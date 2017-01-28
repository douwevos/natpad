/*
   File:    mulrustscanner.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#ifndef RUST_MULRUSTSCANNER_H_
#define RUST_MULRUSTSCANNER_H_

#include <caterpillar.h>
#include <grossruntime.h>

#define MUL_RUST_SYM_END_OF_INPUT                                                                    0
#define MUL_RUST_SYM_ERROR                                                                           1
#define MUL_RUST_SYM_SHL                                                                             2
#define MUL_RUST_SYM_SHR                                                                             3
#define MUL_RUST_SYM_LE                                                                              4
#define MUL_RUST_SYM_EQEQ                                                                            5
#define MUL_RUST_SYM_NE                                                                              6
#define MUL_RUST_SYM_GE                                                                              7
#define MUL_RUST_SYM_ANDAND                                                                          8
#define MUL_RUST_SYM_OROR                                                                            9
#define MUL_RUST_SYM_SHLEQ                                                                           10
#define MUL_RUST_SYM_SHREQ                                                                           11
#define MUL_RUST_SYM_MINUSEQ                                                                         12
#define MUL_RUST_SYM_ANDEQ                                                                           13
#define MUL_RUST_SYM_OREQ                                                                            14
#define MUL_RUST_SYM_PLUSEQ                                                                          15
#define MUL_RUST_SYM_STAREQ                                                                          16
#define MUL_RUST_SYM_SLASHEQ                                                                         17
#define MUL_RUST_SYM_CARETEQ                                                                         18
#define MUL_RUST_SYM_PERCENTEQ                                                                       19
#define MUL_RUST_SYM_DOTDOT                                                                          20
#define MUL_RUST_SYM_DOTDOTDOT                                                                       21
#define MUL_RUST_SYM_MOD_SEP                                                                         22
#define MUL_RUST_SYM_RARROW                                                                          23
#define MUL_RUST_SYM_LARROW                                                                          24
#define MUL_RUST_SYM_FAT_ARROW                                                                       25
#define MUL_RUST_SYM_LIT_BYTE                                                                        26
#define MUL_RUST_SYM_LIT_CHAR                                                                        27
#define MUL_RUST_SYM_LIT_INTEGER                                                                     28
#define MUL_RUST_SYM_LIT_FLOAT                                                                       29
#define MUL_RUST_SYM_LIT_STR                                                                         30
#define MUL_RUST_SYM_LIT_STR_RAW                                                                     31
#define MUL_RUST_SYM_LIT_BYTE_STR                                                                    32
#define MUL_RUST_SYM_LIT_BYTE_STR_RAW                                                                33
#define MUL_RUST_SYM_IDENT                                                                           34
#define MUL_RUST_SYM_UNDERSCORE                                                                      35
#define MUL_RUST_SYM_LIFETIME                                                                        36
#define MUL_RUST_SYM_SELF                                                                            37
#define MUL_RUST_SYM_STATIC                                                                          38
#define MUL_RUST_SYM_AS                                                                              39
#define MUL_RUST_SYM_BREAK                                                                           40
#define MUL_RUST_SYM_CRATE                                                                           41
#define MUL_RUST_SYM_ELSE                                                                            42
#define MUL_RUST_SYM_ENUM                                                                            43
#define MUL_RUST_SYM_EXTERN                                                                          44
#define MUL_RUST_SYM_FALSE                                                                           45
#define MUL_RUST_SYM_FN                                                                              46
#define MUL_RUST_SYM_FOR                                                                             47
#define MUL_RUST_SYM_IF                                                                              48
#define MUL_RUST_SYM_IMPL                                                                            49
#define MUL_RUST_SYM_IN                                                                              50
#define MUL_RUST_SYM_LET                                                                             51
#define MUL_RUST_SYM_LOOP                                                                            52
#define MUL_RUST_SYM_MATCH                                                                           53
#define MUL_RUST_SYM_MOD                                                                             54
#define MUL_RUST_SYM_MOVE                                                                            55
#define MUL_RUST_SYM_MUT                                                                             56
#define MUL_RUST_SYM_PRIV                                                                            57
#define MUL_RUST_SYM_PUB                                                                             58
#define MUL_RUST_SYM_REF                                                                             59
#define MUL_RUST_SYM_RETURN                                                                          60
#define MUL_RUST_SYM_STRUCT                                                                          61
#define MUL_RUST_SYM_TRUE                                                                            62
#define MUL_RUST_SYM_TRAIT                                                                           63
#define MUL_RUST_SYM_TYPE                                                                            64
#define MUL_RUST_SYM_UNSAFE                                                                          65
#define MUL_RUST_SYM_USE                                                                             66
#define MUL_RUST_SYM_WHILE                                                                           67
#define MUL_RUST_SYM_CONTINUE                                                                        68
#define MUL_RUST_SYM_PROC                                                                            69
#define MUL_RUST_SYM_BOX                                                                             70
#define MUL_RUST_SYM_CONST                                                                           71
#define MUL_RUST_SYM_WHERE                                                                           72
#define MUL_RUST_SYM_TYPEOF                                                                          73
#define MUL_RUST_SYM_INNER_DOC_COMMENT                                                               74
#define MUL_RUST_SYM_OUTER_DOC_COMMENT                                                               75
#define MUL_RUST_SYM_SHEBANG                                                                         76
#define MUL_RUST_SYM_SHEBANG_LINE                                                                    77
#define MUL_RUST_SYM_STATIC_LIFETIME                                                                 78
#define MUL_RUST_SYM_L_LSQBRACE                                                                      79
#define MUL_RUST_SYM_L_RSQBRACE                                                                      80
#define MUL_RUST_SYM_L_LPAREN                                                                        81
#define MUL_RUST_SYM_L_RPAREN                                                                        82
#define MUL_RUST_SYM_L_LCUBRACE                                                                      83
#define MUL_RUST_SYM_L_RCUBRACE                                                                      84
#define MUL_RUST_SYM_L_LT                                                                            85
#define MUL_RUST_SYM_L_GT                                                                            86
#define MUL_RUST_SYM_L_COMMA                                                                         87
#define MUL_RUST_SYM_L_DOT                                                                           88
#define MUL_RUST_SYM_L_SEMI                                                                          89
#define MUL_RUST_SYM_L_AMPERSAND                                                                     90
#define MUL_RUST_SYM_L_COLON                                                                         91
#define MUL_RUST_SYM_L_HASH                                                                          92
#define MUL_RUST_SYM_L_AT                                                                            93
#define MUL_RUST_SYM_L_NEG                                                                           94
#define MUL_RUST_SYM_L_QUESTIONMARK                                                                  95
#define MUL_RUST_SYM_L_EXCLAMATION                                                                   96
#define MUL_RUST_SYM_L_VERTICAL_LINE                                                                 97
#define MUL_RUST_SYM_L_CIRCUMFLEX                                                                    98
#define MUL_RUST_SYM_L_PLUS                                                                          99
#define MUL_RUST_SYM_L_MINUS                                                                         100
#define MUL_RUST_SYM_L_MUL                                                                           101
#define MUL_RUST_SYM_L_DIV                                                                           102
#define MUL_RUST_SYM_L_IS                                                                            103
#define MUL_RUST_SYM_L_MOD                                                                           104
#define MUL_RUST_SYM_BLOCK_COMMENT                                                                   105

G_BEGIN_DECLS

#define MUL_TYPE_RUST_SCANNER              (mul_rust_scanner_get_type())
#define MUL_RUST_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_rust_scanner_get_type(), MulRustScanner))
#define MUL_RUST_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_RUST_SCANNER, MulRustScannerClass))
#define MUL_IS_RUST_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_RUST_SCANNER))
#define MUL_IS_RUST_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_RUST_SCANNER))
#define MUL_RUST_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_RUST_SCANNER, MulRustScannerClass))


typedef struct _MulRustScanner               MulRustScanner;
typedef struct _MulRustScannerPrivate        MulRustScannerPrivate;
typedef struct _MulRustScannerClass          MulRustScannerClass;


struct _MulRustScanner {
	GroRunScannerBase parent;
};

struct _MulRustScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType mul_rust_scanner_get_type();

MulRustScanner *mul_rust_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);

gboolean mul_rust_scanner_is_eof(MulRustScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* RUST_MULRUSTSCANNER_H_ */
