/*
   File:    catiutf8scanner.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 2, 2009
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

#ifndef CATIUTF8SCANNER_H_
#define CATIUTF8SCANNER_H_

#include <glib-object.h>
#include "../catstream.h"

#define CAT_TYPE_IUTF8_SCANNER                 (cat_iutf8_scanner_get_type())
#define CAT_IUTF8_SCANNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_IUTF8_SCANNER, CatIUtf8Scanner))
#define CAT_IS_IUTF8_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_IUTF8_SCANNER))
#define CAT_IUTF8_SCANNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CAT_TYPE_IUTF8_SCANNER, CatIUtf8ScannerInterface))


typedef struct _CatIUtf8Scanner               CatIUtf8Scanner; /* dummy object */
typedef struct _CatIUtf8ScannerInterface      CatIUtf8ScannerInterface;

struct _CatIUtf8ScannerInterface {
	GTypeInterface parent_iface;

	gunichar (*scan_utf8_char)(CatIUtf8Scanner *self, CatStreamStatus *status);
	int (*scan_utf8_char_length)(CatIUtf8Scanner *self, char *data, const int length, CatStreamStatus *status);

};

GType cat_iutf8_scanner_get_type(void);


gunichar cat_iutf8_scanner_next_char(CatIUtf8Scanner *self, CatStreamStatus *status);


#endif /* CATIUTF8SCANNER_H_ */

