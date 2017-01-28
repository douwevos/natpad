/*
   File:    spiscannernew.h
   Project: spider
   Author:  Douwe Vos
   Date:    Oct 18, 2016
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

#ifndef EDITOR_SPISCANNERNEW_H_
#define EDITOR_SPISCANNERNEW_H_

#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define SPI_SYM_END_OF_INPUT                 0
#define SPI_SYM_KW_FUNCTION                  2
#define SPI_SYM_KW_VAR                       3
#define SPI_SYM_KW_RETURN                    4
#define SPI_SYM_KW_IF                        5
#define SPI_SYM_KW_ELSE                      6
#define SPI_SYM_KW_THIS                      7
#define SPI_SYM_KW_FOR                       8
#define SPI_SYM_ID                           9
#define SPI_SYM_KW_NULL                      10
#define SPI_SYM_KW_TRUE                      11
#define SPI_SYM_KW_FALSE                     12
#define SPI_SYM_NUMBER                       13
#define SPI_SYM_STRING                       14
#define SPI_SYM_LPAREN                       15
#define SPI_SYM_RPAREN                       16
#define SPI_SYM_LBRACE                       17
#define SPI_SYM_RBRACE                       18
#define SPI_SYM_SEMI                         19
#define SPI_SYM_COMMA                        20
#define SPI_SYM_ASS_IS                       21
#define SPI_SYM_QUESTION                     22
#define SPI_SYM_COLON                        23
#define SPI_SYM_OP_LOR                       24
#define SPI_SYM_OP_LAND                      25
#define SPI_SYM_OP_BOR                       26
#define SPI_SYM_OP_BXOR                      27
#define SPI_SYM_OP_BAND                      28
#define SPI_SYM_KW_DELETE                    29
#define SPI_SYM_KW_VOID                      30
#define SPI_SYM_KW_TYPEOF                    31
#define SPI_SYM_OP_INC                       32
#define SPI_SYM_OP_DEC                       33
#define SPI_SYM_OP_ADD                       34
#define SPI_SYM_OP_SUB                       35
#define SPI_SYM_OP_NEG                       36
#define SPI_SYM_OP_NOT                       37
#define SPI_SYM_OP_MUL                       38
#define SPI_SYM_OP_DIV                       39
#define SPI_SYM_OP_MOD                       40
#define SPI_SYM_OP_SHL                       41
#define SPI_SYM_OP_SHR                       42
#define SPI_SYM_OP_SHRR                      43
#define SPI_SYM_OP_LT                        44
#define SPI_SYM_OP_GT                        45
#define SPI_SYM_OP_LE                        46
#define SPI_SYM_OP_GE                        47
#define SPI_SYM_KW_INSTANCE_OF               48
#define SPI_SYM_KW_IN                        49
#define SPI_SYM_OP_EQ                        50
#define SPI_SYM_OP_NE                        51
#define SPI_SYM_OP_TEQ                       52
#define SPI_SYM_OP_TNE                       53
#define SPI_SYM_OP_ASS_MUL                   54
#define SPI_SYM_OP_ASS_DIV                   55
#define SPI_SYM_OP_ASS_MOD                   56
#define SPI_SYM_OP_ASS_ADD                   57
#define SPI_SYM_OP_ASS_SUB                   58
#define SPI_SYM_OP_ASS_SHL                   59
#define SPI_SYM_OP_ASS_SHR                   60
#define SPI_SYM_OP_ASS_SHRR                  61
#define SPI_SYM_OP_ASS_AND                   62
#define SPI_SYM_OP_ASS_XOR                   63
#define SPI_SYM_OP_ASS_OR                    64
#define SPI_SYM_KW_NEW                       65
#define SPI_SYM_LBRACK                       66
#define SPI_SYM_RBRACK                       67
#define SPI_SYM_DOT                          68
#define SPI_SYM_REGULAR_EXPRESSION           69
#define SPI_SYM_KW_SWITCH                    70
#define SPI_SYM_KW_CASE                      71
#define SPI_SYM_KW_DEFAULT                   72
#define SPI_SYM_KW_DO                        73
#define SPI_SYM_KW_WHILE                     74
#define SPI_SYM_KW_WITH                      75
#define SPI_SYM_KW_CONTINUE                  76
#define SPI_SYM_KW_BREAK                     77
#define SPI_SYM_KW_THROW                     78
#define SPI_SYM_KW_TRY                       79
#define SPI_SYM_KW_CATCH                     80
#define SPI_SYM_KW_FINALLY                   81
#define SPI_SYM_LINE_COMMENT                 82
#define SPI_SYM_FULL_COMMENT                 83


#define SPI_TYPE_SCANNER_NEW              (spi_scanner_new_get_type())
#define SPI_SCANNER_NEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_scanner_new_get_type(), SpiScannerNew))
#define SPI_SCANNER_NEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_SCANNER_NEW, SpiScannerNewClass))
#define SPI_IS_SCANNER_NEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_SCANNER_NEW))
#define SPI_IS_SCANNER_NEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_SCANNER_NEW))
#define SPI_SCANNER_NEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_SCANNER_NEW, SpiScannerNewClass))


typedef struct _SpiScannerNew               SpiScannerNew;
typedef struct _SpiScannerNewPrivate        SpiScannerNewPrivate;
typedef struct _SpiScannerNewClass          SpiScannerNewClass;


struct _SpiScannerNew {
	GObject parent;
};

struct _SpiScannerNewClass {
	GObjectClass parent_class;
};


GType spi_scanner_new_get_type();

SpiScannerNew *spi_scanner_new_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, GroRunSymbol **connected_symbols);

gboolean spi_scanner_new_is_eof(SpiScannerNew *scanner, GroRunIToken *token);

int spi_scanner_new_get_symbol_index(SpiScannerNew *scanner, GroRunSymbol *sym);

G_END_DECLS

#endif /* EDITOR_SPISCANNERNEW_H_ */
