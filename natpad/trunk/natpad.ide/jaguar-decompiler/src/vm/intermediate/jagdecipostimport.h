/*
   File:    jagdecipostimport.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 7, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef JAGDECIPOSTIMPORT_H_
#define JAGDECIPOSTIMPORT_H_

#include <glib-object.h>

#define JAG_DEC_TYPE_IPOST_IMPORT                 (jag_dec_ipost_import_get_type())
#define JAG_DEC_IPOST_IMPORT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_DEC_TYPE_IPOST_IMPORT, JagDecIPostImport))
#define JAG_DEC_IS_IPOST_IMPORT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_DEC_TYPE_IPOST_IMPORT))
#define JAG_DEC_IPOST_IMPORT_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_DEC_TYPE_IPOST_IMPORT, JagDecIPostImportInterface))


typedef struct _JagDecIPostImport               JagDecIPostImport; /* dummy object */
typedef struct _JagDecIPostImportInterface      JagDecIPostImportInterface;


struct _JagDecImportRepository;

struct _JagDecIPostImportInterface {
	GTypeInterface parent_iface;

	void (*addImport)(JagDecIPostImport *self, struct _JagDecImportRepository *import_repository);
};

GType jag_dec_ipost_import_get_type(void);

void jag_dec_ipost_import_add_import(JagDecIPostImport *self, struct _JagDecImportRepository *import_repository);

#endif /* JAGDECIPOSTIMPORT_H_ */
