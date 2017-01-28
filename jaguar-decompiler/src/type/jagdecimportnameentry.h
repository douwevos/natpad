/*
   File:    jagdecimportnameentry.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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


#ifndef JAGDECIMPORTNAMEENTRY_H_
#define JAGDECIMPORTNAMEENTRY_H_


#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_IMPORT_NAME_ENTRY              (jag_dec_import_name_entry_get_type())
#define JAG_DEC_IMPORT_NAME_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_import_name_entry_get_type(), JagDecImportNameEntry))
#define JAG_DEC_IMPORT_NAME_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_IMPORT_NAME_ENTRY, JagDecImportNameEntryClass))
#define JAG_DEC_IS_IMPORT_NAME_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_IMPORT_NAME_ENTRY))
#define JAG_DEC_IS_IMPORT_NAME_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_IMPORT_NAME_ENTRY))
#define JAG_DEC_IMPORT_NAME_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_IMPORT_NAME_ENTRY, JagDecImportNameEntryClass))
#define JAG_DEC_IMPORT_NAME_ENTRY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_IMPORT_NAME_ENTRY, JagDecImportNameEntryPrivate))

typedef struct _JagDecImportNameEntry               JagDecImportNameEntry;
typedef struct _JagDecImportNameEntryPrivate        JagDecImportNameEntryPrivate;
typedef struct _JagDecImportNameEntryClass          JagDecImportNameEntryClass;


struct _JagDecImportNameEntry {
	GObject parent;
	JagDecImportNameEntryPrivate *priv;
};

struct _JagDecImportNameEntryClass {
	GObjectClass parent_class;
};


GType jag_dec_import_name_entry_get_type();


JagDecImportNameEntry *jag_dec_import_name_entry_new(CatStringWo *a_short_type_name);

JagBytName *jag_dec_import_name_entry_get_or_add(JagDecImportNameEntry *name_entry, JagBytName *name);

gboolean jag_dec_import_name_entry_allow_short_version(JagDecImportNameEntry *name_entry);

JagBytName *jag_dec_import_name_entry_get_first_name(JagDecImportNameEntry *name_entry);



G_END_DECLS


#endif /* JAGDECIMPORTNAMEENTRY_H_ */
