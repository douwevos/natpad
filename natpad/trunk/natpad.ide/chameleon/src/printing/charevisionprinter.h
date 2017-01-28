/*
   File:    charevisionprinter.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 5, 2015
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

#ifndef PRINTING_CHAREVISIONPRINTER_H_
#define PRINTING_CHAREVISIONPRINTER_H_

#include "../chaeditor.h"
#include "chaprintsettings.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CHA_TYPE_REVISION_PRINTER              (cha_revision_printer_get_type())
#define CHA_REVISION_PRINTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_revision_printer_get_type(), ChaRevisionPrinter))
#define CHA_REVISION_PRINTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_REVISION_PRINTER, ChaRevisionPrinterClass))
#define CHA_IS_REVISION_PRINTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_REVISION_PRINTER))
#define CHA_IS_REVISION_PRINTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_REVISION_PRINTER))
#define CHA_REVISION_PRINTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_REVISION_PRINTER, ChaRevisionPrinterClass))


typedef struct _ChaRevisionPrinter               ChaRevisionPrinter;
typedef struct _ChaRevisionPrinterPrivate        ChaRevisionPrinterPrivate;
typedef struct _ChaRevisionPrinterClass          ChaRevisionPrinterClass;


struct _ChaRevisionPrinter {
	GObject parent;
};

struct _ChaRevisionPrinterClass {
	GObjectClass parent_class;
};


GType cha_revision_printer_get_type();

ChaRevisionPrinter *cha_revision_printer_new(ChaPrintSettings *print_settings, ChaEditor *editor, CatStringWo *a_markup_hook_key, cairo_t *cairo, PangoContext *pango_context, PangoLayout *pango_layout, double page_width, double page_height, double margin_top, double margin_bottom, double margin_left, double margin_right);

int cha_revision_printer_get_page_count(ChaRevisionPrinter *editor_printer);

void cha_revision_printer_draw_page(ChaRevisionPrinter *revision_print, int page_index);

G_END_DECLS

#endif /* PRINTING_CHAREVISIONPRINTER_H_ */
