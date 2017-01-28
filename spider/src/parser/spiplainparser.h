/*
   File:    spiplainparser.h
   Project: spider
   Author:  Douwe Vos
   Date:    Mai 7, 2016
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#ifndef SPIPLAINPARSER_H_
#define SPIPLAINPARSER_H_

#include "../editor/spiscannernew.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define SPI_TYPE_PLAIN_PARSER              (spi_plain_parser_get_type())
#define SPI_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_plain_parser_get_type(), SpiPlainParser))
#define SPI_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_PLAIN_PARSER, SpiPlainParserClass))
#define SPI_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_PLAIN_PARSER))
#define SPI_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_PLAIN_PARSER))
#define SPI_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_PLAIN_PARSER, SpiPlainParserClass))


typedef struct _SpiPlainParser               SpiPlainParser;
typedef struct _SpiPlainParserPrivate        SpiPlainParserPrivate;
typedef struct _SpiPlainParserClass          SpiPlainParserClass;


struct _SpiPlainParser {
		GObject parent;
};

struct _SpiPlainParserClass {
		GObjectClass parent_class;
};


GType spi_plain_parser_get_type();

SpiPlainParser *spi_plain_parser_new(SpiScannerNew *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void spi_plain_parser_run(SpiPlainParser *parser);

G_END_DECLS

#endif /* SPIPLAINPARSER_H_ */
