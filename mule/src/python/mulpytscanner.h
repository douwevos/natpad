/*
   File:    mulpytscanner.h
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

#ifndef PYTHON_MULPYTSCANNER_H_
#define PYTHON_MULPYTSCANNER_H_

#include <caterpillar.h>
#include <grossruntime.h>

#define MUL_PYT_SYM_END_OF_INPUT                      0
#define MUL_PYT_SYM_ERROR                             1
#define MUL_PYT_SYM_NEWLINE                           2
#define MUL_PYT_SYM_ENDMARKER                         3
#define MUL_PYT_SYM_DOUBLEASTERISK                    4
#define MUL_PYT_SYM_NAME                              5
#define MUL_PYT_SYM_COMMA                             6
#define MUL_PYT_SYM_CHARAT                            7
#define MUL_PYT_SYM_LPARREN                           8
#define MUL_PYT_SYM_RPARREN                           9
#define MUL_PYT_SYM_KWDEF                             10
#define MUL_PYT_SYM_COLON                             11
#define MUL_PYT_SYM_EQUALS                            12
#define MUL_PYT_SYM_SEMI                              13
#define MUL_PYT_SYM_ADD_ASSIGN                        14
#define MUL_PYT_SYM_SUB_ASSIGN                        15
#define MUL_PYT_SYM_MUL_ASSIGN                        16
#define MUL_PYT_SYM_DIV_ASSIGN                        17
#define MUL_PYT_SYM_MOD_ASSIGN                        18
#define MUL_PYT_SYM_AND_ASSIGN                        19
#define MUL_PYT_SYM_OR_ASSIGN                         20
#define MUL_PYT_SYM_XOR_ASSIGN                        21
#define MUL_PYT_SYM_SHL_ASSIGN                        22
#define MUL_PYT_SYM_SHR_ASSIGN                        23
#define MUL_PYT_SYM_MULMUL_ASSIGN                     24
#define MUL_PYT_SYM_DIVDIV_ASSIGN                     25
#define MUL_PYT_SYM_KWPRINT                           26
#define MUL_PYT_SYM_SHIFTRIGHT                        27
#define MUL_PYT_SYM_SHIFTLEFT                         28
#define MUL_PYT_SYM_KWDEL                             29
#define MUL_PYT_SYM_KWPASS                            30
#define MUL_PYT_SYM_KWBREAK                           31
#define MUL_PYT_SYM_KWCONTINUE                        32
#define MUL_PYT_SYM_KWRETURN                          33
#define MUL_PYT_SYM_KWRAISE                           34
#define MUL_PYT_SYM_KWIMPORT                          35
#define MUL_PYT_SYM_KWFROM                            36
#define MUL_PYT_SYM_DOT                               37
#define MUL_PYT_SYM_KWAS                              38
#define MUL_PYT_SYM_KWGLOBAL                          39
#define MUL_PYT_SYM_KWEXEC                            40
#define MUL_PYT_SYM_KWASSERT                          41
#define MUL_PYT_SYM_KWIN                              42
#define MUL_PYT_SYM_KWIF                              43
#define MUL_PYT_SYM_KWELSE                            44
#define MUL_PYT_SYM_KWELIF                            45
#define MUL_PYT_SYM_KWWHILE                           46
#define MUL_PYT_SYM_KWFOR                             47
#define MUL_PYT_SYM_KWTRY                             48
#define MUL_PYT_SYM_KWFINALLY                         49
#define MUL_PYT_SYM_KWWITH                            50
#define MUL_PYT_SYM_KWEXCEPT                          51
#define MUL_PYT_SYM_INDENT                            52
#define MUL_PYT_SYM_DEDENT                            53
#define MUL_PYT_SYM_KWLAMBDA                          54
#define MUL_PYT_SYM_KWOR                              55
#define MUL_PYT_SYM_KWAND                             56
#define MUL_PYT_SYM_KWNOT                             57
#define MUL_PYT_SYM_KWIS                              58
#define MUL_PYT_SYM_KWCLASS                           59
#define MUL_PYT_SYM_KWYIELD                           60
#define MUL_PYT_SYM_OPLT                              61
#define MUL_PYT_SYM_OPGT                              62
#define MUL_PYT_SYM_OPLE                              63
#define MUL_PYT_SYM_OPGE                              64
#define MUL_PYT_SYM_OPEQ                              65
#define MUL_PYT_SYM_OPNE                              66
#define MUL_PYT_SYM_OPDIF                             67
#define MUL_PYT_SYM_OPOR                              68
#define MUL_PYT_SYM_OPAND                             69
#define MUL_PYT_SYM_OPXOR                             70
#define MUL_PYT_SYM_OPADD                             71
#define MUL_PYT_SYM_OPSUB                             72
#define MUL_PYT_SYM_OPDIV                             73
#define MUL_PYT_SYM_OPMUL                             74
#define MUL_PYT_SYM_OPMOD                             75
#define MUL_PYT_SYM_OPDIVDIV                          76
#define MUL_PYT_SYM_OPNEG                             77
#define MUL_PYT_SYM_LSQBR                             78
#define MUL_PYT_SYM_RSQBR                             79
#define MUL_PYT_SYM_LACCOL                            80
#define MUL_PYT_SYM_RACCOL                            81
#define MUL_PYT_SYM_SINGLEQUOTE                       82
#define MUL_PYT_SYM_NUMBER                            83
#define MUL_PYT_SYM_STRING                            84
#define MUL_PYT_SYM_KWASSIGN                          85
#define MUL_PYT_SYM_EOL_COMMENT                       86

G_BEGIN_DECLS

#define MUL_PYT_TYPE_SCANNER              (mul_pyt_scanner_get_type())
#define MUL_PYT_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_pyt_scanner_get_type(), MulPytScanner))
#define MUL_PYT_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_PYT_TYPE_SCANNER, MulPytScannerClass))
#define MUL_PYT_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_PYT_TYPE_SCANNER))
#define MUL_PYT_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_PYT_TYPE_SCANNER))
#define MUL_PYT_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_PYT_TYPE_SCANNER, MulPytScannerClass))


typedef struct _MulPytScanner               MulPytScanner;
typedef struct _MulPytScannerPrivate        MulPytScannerPrivate;
typedef struct _MulPytScannerClass          MulPytScannerClass;


struct _MulPytScanner {
	GroRunScannerBase parent;
};

struct _MulPytScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType mul_pyt_scanner_get_type();

MulPytScanner *mul_pyt_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);

gboolean mul_pyt_scanner_is_eof(MulPytScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* PYTHON_MULPYTSCANNER_H_ */
