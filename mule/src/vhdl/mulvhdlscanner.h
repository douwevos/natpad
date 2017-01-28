/*
   File:    mulvhdlscanner.h
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 11, 2016
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

#ifndef VHDL_MULVHDLSCANNER_H_
#define VHDL_MULVHDLSCANNER_H_

#include <caterpillar.h>
#include <grossruntime.h>


#define MUL_VHDL_SYM_END_OF_INPUT                      0
#define MUL_VHDL_SYM_ERROR                             1
#define MUL_VHDL_SYM_KWACCESS                          2
#define MUL_VHDL_SYM_KWAFTER                           3
#define MUL_VHDL_SYM_KWALIAS                           4
#define MUL_VHDL_SYM_KWALL                             5
#define MUL_VHDL_SYM_KWAND                             6
#define MUL_VHDL_SYM_KWARCHITECTURE                    7
#define MUL_VHDL_SYM_KWARRAY                           8
#define MUL_VHDL_SYM_KWASSERT                          9
#define MUL_VHDL_SYM_KWATTRIBUTE                       10
#define MUL_VHDL_SYM_KWBEGIN                           11
#define MUL_VHDL_SYM_KWBLOCK                           12
#define MUL_VHDL_SYM_KWBODY                            13
#define MUL_VHDL_SYM_KWBUFFER                          14
#define MUL_VHDL_SYM_KWBUS                             15
#define MUL_VHDL_SYM_KWCASE                            16
#define MUL_VHDL_SYM_KWCOMPONENT                       17
#define MUL_VHDL_SYM_KWCONFIGURATION                   18
#define MUL_VHDL_SYM_KWCONSTANT                        19
#define MUL_VHDL_SYM_KWDISCONNECT                      20
#define MUL_VHDL_SYM_KWDOWNTO                          21
#define MUL_VHDL_SYM_KWELSE                            22
#define MUL_VHDL_SYM_KWELSIF                           23
#define MUL_VHDL_SYM_KWEND                             24
#define MUL_VHDL_SYM_KWENTITY                          25
#define MUL_VHDL_SYM_KWEXIT                            26
#define MUL_VHDL_SYM_KWFILE                            27
#define MUL_VHDL_SYM_KWFOR                             28
#define MUL_VHDL_SYM_KWFUNCTION                        29
#define MUL_VHDL_SYM_KWGENERATE                        30
#define MUL_VHDL_SYM_KWGENERIC                         31
#define MUL_VHDL_SYM_KWGUARDED                         32
#define MUL_VHDL_SYM_KWIF                              33
#define MUL_VHDL_SYM_KWIMPURE                          34
#define MUL_VHDL_SYM_KWIN                              35
#define MUL_VHDL_SYM_KWINERTIAL                        36
#define MUL_VHDL_SYM_KWINOUT                           37
#define MUL_VHDL_SYM_KWIS                              38
#define MUL_VHDL_SYM_KWLABEL                           39
#define MUL_VHDL_SYM_KWLIBRARY                         40
#define MUL_VHDL_SYM_KWLINKAGE                         41
#define MUL_VHDL_SYM_KWLOOP                            42
#define MUL_VHDL_SYM_KWMAP                             43
#define MUL_VHDL_SYM_KWNAND                            44
#define MUL_VHDL_SYM_KWNEW                             45
#define MUL_VHDL_SYM_KWNEXT                            46
#define MUL_VHDL_SYM_KWNOR                             47
#define MUL_VHDL_SYM_KWNULL                            48
#define MUL_VHDL_SYM_KWOF                              49
#define MUL_VHDL_SYM_KWON                              50
#define MUL_VHDL_SYM_KWOPEN                            51
#define MUL_VHDL_SYM_KWOR                              52
#define MUL_VHDL_SYM_KWOTHERS                          53
#define MUL_VHDL_SYM_KWOUT                             54
#define MUL_VHDL_SYM_KWPACKAGE                         55
#define MUL_VHDL_SYM_KWPORT                            56
#define MUL_VHDL_SYM_KWPOSTPONED                       57
#define MUL_VHDL_SYM_KWPROCEDURE                       58
#define MUL_VHDL_SYM_KWPROCESS                         59
#define MUL_VHDL_SYM_KWPURE                            60
#define MUL_VHDL_SYM_KWRANGE                           61
#define MUL_VHDL_SYM_KWREVERSE_RANGE                   62
#define MUL_VHDL_SYM_KWRECORD                          63
#define MUL_VHDL_SYM_KWREGISTER                        64
#define MUL_VHDL_SYM_KWREJECT                          65
#define MUL_VHDL_SYM_KWREPORT                          66
#define MUL_VHDL_SYM_KWRETURN                          67
#define MUL_VHDL_SYM_KWROL                             68
#define MUL_VHDL_SYM_KWROR                             69
#define MUL_VHDL_SYM_KWSELECT                          70
#define MUL_VHDL_SYM_KWSEVERITY                        71
#define MUL_VHDL_SYM_KWSIGNAL                          72
#define MUL_VHDL_SYM_KWSLA                             73
#define MUL_VHDL_SYM_KWSLL                             74
#define MUL_VHDL_SYM_KWSRA                             75
#define MUL_VHDL_SYM_KWSRL                             76
#define MUL_VHDL_SYM_KWSUBTYPE                         77
#define MUL_VHDL_SYM_KWTHEN                            78
#define MUL_VHDL_SYM_KWTO                              79
#define MUL_VHDL_SYM_KWTRANSPORT                       80
#define MUL_VHDL_SYM_KWTYPE                            81
#define MUL_VHDL_SYM_KWUNITS                           82
#define MUL_VHDL_SYM_KWUNTIL                           83
#define MUL_VHDL_SYM_KWUSE                             84
#define MUL_VHDL_SYM_KWVARIABLE                        85
#define MUL_VHDL_SYM_KWWAIT                            86
#define MUL_VHDL_SYM_KWWHEN                            87
#define MUL_VHDL_SYM_KWWHILE                           88
#define MUL_VHDL_SYM_KWWITH                            89
#define MUL_VHDL_SYM_KWXNOR                            90
#define MUL_VHDL_SYM_KWXOR                             91
#define MUL_VHDL_SYM_OPPEQ                             92
#define MUL_VHDL_SYM_OPPNE                             93
#define MUL_VHDL_SYM_OPPLT                             94
#define MUL_VHDL_SYM_OPPLE                             95
#define MUL_VHDL_SYM_OPPGT                             96
#define MUL_VHDL_SYM_OPPGE                             97
#define MUL_VHDL_SYM_PLUS                              98
#define MUL_VHDL_SYM_MINUS                             99
#define MUL_VHDL_SYM_AMPERSAND                         100
#define MUL_VHDL_SYM_STAR                              101
#define MUL_VHDL_SYM_SLASH                             102
#define MUL_VHDL_SYM_KWMOD                             103
#define MUL_VHDL_SYM_KWREM                             104
#define MUL_VHDL_SYM_DOUBLESTAR                        105
#define MUL_VHDL_SYM_KWABS                             106
#define MUL_VHDL_SYM_KWNOT                             107
#define MUL_VHDL_SYM_APOSTROPHE                        108
#define MUL_VHDL_SYM_LEFTPAREN                         109
#define MUL_VHDL_SYM_RIGHTPAREN                        110
#define MUL_VHDL_SYM_COMMA                             111
#define MUL_VHDL_SYM_VARASSIGN                         112
#define MUL_VHDL_SYM_COLON                             113
#define MUL_VHDL_SYM_SEMICOLON                         114
#define MUL_VHDL_SYM_ARROW                             115
#define MUL_VHDL_SYM_BOX                               116
#define MUL_VHDL_SYM_BAR                               117
#define MUL_VHDL_SYM_DOT                               118
#define MUL_VHDL_SYM_NUMBERLITERAL                     119
#define MUL_VHDL_SYM_CHARACTERLITERAL                  120
#define MUL_VHDL_SYM_IDENTIFIER                        121
#define MUL_VHDL_SYM_STRINGLITERAL                     122
#define MUL_VHDL_SYM_EOL_COMMENT                       123


G_BEGIN_DECLS

#define MUL_VHDL_TYPE_SCANNER              (mul_vhdl_scanner_get_type())
#define MUL_VHDL_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_vhdl_scanner_get_type(), MulVhdlScanner))
#define MUL_VHDL_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_VHDL_TYPE_SCANNER, MulVhdlScannerClass))
#define MUL_VHDL_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_VHDL_TYPE_SCANNER))
#define MUL_VHDL_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_VHDL_TYPE_SCANNER))
#define MUL_VHDL_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_VHDL_TYPE_SCANNER, MulVhdlScannerClass))


typedef struct _MulVhdlScanner               MulVhdlScanner;
typedef struct _MulVhdlScannerClass          MulVhdlScannerClass;


struct _MulVhdlScanner {
	GroRunScannerBase parent;
};

struct _MulVhdlScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType mul_vhdl_scanner_get_type();

MulVhdlScanner *mul_vhdl_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols);


gboolean mul_vhdl_scanner_is_eof(MulVhdlScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* VHDL_MULVHDLSCANNER_H_ */
