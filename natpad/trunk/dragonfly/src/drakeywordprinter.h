/*
   File:    drakeywordprinter.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 4, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef DRAKEYWORDPRINTER_H_
#define DRAKEYWORDPRINTER_H_

#include "dralinetagwo.h"
#include "attr/draiattributebuilder.h"
#include <caterpillar.h>
#include <chameleon.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DRA_TYPE_KEYWORD_PRINTER              (dra_keyword_printer_get_type())
#define DRA_KEYWORD_PRINTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_keyword_printer_get_type(), DraKeywordPrinter))
#define DRA_KEYWORD_PRINTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_KEYWORD_PRINTER, DraKeywordPrinterClass))
#define DRA_IS_KEYWORD_PRINTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_KEYWORD_PRINTER))
#define DRA_IS_KEYWORD_PRINTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_KEYWORD_PRINTER))
#define DRA_KEYWORD_PRINTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_KEYWORD_PRINTER, DraKeywordPrinterClass))


typedef struct _DraKeywordPrinter               DraKeywordPrinter;
typedef struct _DraKeywordPrinterPrivate        DraKeywordPrinterPrivate;
typedef struct _DraKeywordPrinterClass          DraKeywordPrinterClass;


struct _DraKeywordPrinter {
	GObject parent;
};

struct _DraKeywordPrinterClass {
	GObjectClass parent_class;
};


GType dra_keyword_printer_get_type();

DraKeywordPrinter *dra_keyword_printer_new(ChaRevisionWo *a_revision, CatStringWo *a_markup_key, int markup_idx);

void dra_keyword_printer_reset(DraKeywordPrinter *printer);

void dra_keyword_printer_print(DraKeywordPrinter *printer, int begin_row, int begin_column, int end_row, int end_column, DraIAttributeBuilder *ab);

void dra_keyword_printer_flush(DraKeywordPrinter *printer);

void dra_keyword_printer_print_fg_color(DraKeywordPrinter *printer, int begin_row, int begin_column, int end_row, int end_column, int color_index);

void dra_keyword_printer_flush_line_tags(DraKeywordPrinter *printer);

void dra_keyword_printer_print_line_tag(DraKeywordPrinter *printer, DraLineTagWo *c_line_tag_wo);

G_END_DECLS

#endif /* DRAKEYWORDPRINTER_H_ */
