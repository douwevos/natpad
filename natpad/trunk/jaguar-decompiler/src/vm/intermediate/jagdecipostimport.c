/*
   File:    jagdecipostimport.c
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

#include "jagdecipostimport.h"

G_DEFINE_INTERFACE(JagDecIPostImport, jag_dec_ipost_import, G_TYPE_OBJECT);

static void jag_dec_ipost_import_default_init (JagDecIPostImportInterface *klass) {
}

void jag_dec_ipost_import_add_import(JagDecIPostImport *self, struct _JagDecImportRepository *import_repository) {
	g_return_if_fail(JAG_DEC_IS_IPOST_IMPORT(self));
	JAG_DEC_IPOST_IMPORT_GET_INTERFACE(self)->addImport(self, import_repository);
}

