/*
   File:    catiutf8printer.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 17, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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

#ifndef CATIUTF8PRINTER_H_
#define CATIUTF8PRINTER_H_

#include <glib-object.h>
#include "../catstream.h"
#include "../../woo/catstringwo.h"

#define CAT_TYPE_IUTF8_PRINTER                 (cat_iutf8_printer_get_type())
#define CAT_IUTF8_PRINTER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_IUTF8_PRINTER, CatIUtf8Printer))
#define CAT_IS_IUTF8_PRINTER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_IUTF8_PRINTER))
#define CAT_IUTF8_PRINTER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CAT_TYPE_IUTF8_PRINTER, CatIUtf8PrinterInterface))


typedef struct _CatIUtf8Printer               CatIUtf8Printer; /* dummy object */
typedef struct _CatIUtf8PrinterInterface      CatIUtf8PrinterInterface;

struct _CatIUtf8PrinterInterface {
	GTypeInterface parent_iface;
	void (*close)(CatIUtf8Printer *self);
	int (*print_unichar4)(CatIUtf8Printer *self, gunichar ch, CatStreamStatus *status);
	int (*print_utf8_string)(CatIUtf8Printer *self, CatStringWo *a_txt, CatStreamStatus *status);
};

GType cat_iutf8_printer_get_type(void);


void cat_iutf8_printer_close(CatIUtf8Printer *self);
int cat_iutf8_printer_print_unichar4(CatIUtf8Printer *self, gunichar ch, CatStreamStatus *status);
int cat_iutf8_printer_print_utf8_string(CatIUtf8Printer *self, CatStringWo *toprint, CatStreamStatus *status);


#endif /* CATIUTF8PRINTER_H_ */

