/*
   File:    chaeditorprinter.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 4, 2015
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

#ifndef PRINTING_CHAEDITORPRINTER_H_
#define PRINTING_CHAEDITORPRINTER_H_

#include "../chaeditor.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_EDITOR_PRINTER              (cha_editor_printer_get_type())
#define CHA_EDITOR_PRINTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_editor_printer_get_type(), ChaEditorPrinter))
#define CHA_EDITOR_PRINTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_EDITOR_PRINTER, ChaEditorPrinterClass))
#define CHA_IS_EDITOR_PRINTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_EDITOR_PRINTER))
#define CHA_IS_EDITOR_PRINTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_EDITOR_PRINTER))
#define CHA_EDITOR_PRINTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_EDITOR_PRINTER, ChaEditorPrinterClass))


typedef struct _ChaEditorPrinter               ChaEditorPrinter;
typedef struct _ChaEditorPrinterPrivate        ChaEditorPrinterPrivate;
typedef struct _ChaEditorPrinterClass          ChaEditorPrinterClass;


struct _ChaEditorPrinter {
	GObject parent;
};

struct _ChaEditorPrinterClass {
	GObjectClass parent_class;
};


GType cha_editor_printer_get_type();

ChaEditorPrinter *cha_editor_printer_new(ChaEditor *editor, CatStringWo *a_markup_key);

void cha_editor_printer_do_print(ChaEditorPrinter *editor_printer, gboolean do_preview);

G_END_DECLS

#endif /* PRINTING_CHAEDITORPRINTER_H_ */
