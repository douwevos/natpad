/*
   File:    jagdecimportrepository.h
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


#ifndef JAGDECIMPORTREPOSITORY_H_
#define JAGDECIMPORTREPOSITORY_H_

#include "jagdecimportnameentry.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_IMPORT_REPOSITORY              (jag_dec_import_repository_get_type())
#define JAG_DEC_IMPORT_REPOSITORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_import_repository_get_type(), JagDecImportRepository))
#define JAG_DEC_IMPORT_REPOSITORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_IMPORT_REPOSITORY, JagDecImportRepositoryClass))
#define JAG_DEC_IS_IMPORT_REPOSITORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_IMPORT_REPOSITORY))
#define JAG_DEC_IS_IMPORT_REPOSITORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_IMPORT_REPOSITORY))
#define JAG_DEC_IMPORT_REPOSITORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_IMPORT_REPOSITORY, JagDecImportRepositoryClass))
#define JAG_DEC_IMPORT_REPOSITORY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_IMPORT_REPOSITORY, JagDecImportRepositoryPrivate))

typedef struct _JagDecImportRepository               JagDecImportRepository;
typedef struct _JagDecImportRepositoryPrivate        JagDecImportRepositoryPrivate;
typedef struct _JagDecImportRepositoryClass          JagDecImportRepositoryClass;


struct _JagDecImportRepository {
	GObject parent;
	JagDecImportRepositoryPrivate *priv;
};

struct _JagDecImportRepositoryClass {
	GObjectClass parent_class;
};


GType jag_dec_import_repository_get_type();

JagDecImportRepository *jag_dec_import_repository_new(JagBytName *main_type_name);

JagBytName *jag_dec_import_repository_get_main_type_name(JagDecImportRepository *import_repository);

JagBytName *jag_dec_import_repository_create_resolved_typename(JagDecImportRepository *import_repository, JagBytName *referenceTypeName);

JagAstDeclarationType *jag_dec_import_repository_create_final_declaration_type(JagDecImportRepository *import_repository, JagAstDeclarationType *full_type);

JagBytName *jag_dec_import_repository_add_name(JagDecImportRepository *import_repository, JagBytName *fullName);

CatArrayWo *jag_dec_import_repository_create_statements(JagDecImportRepository *import_repository);


void jag_dec_import_repository_attach_method(JagDecImportRepository *import_repository, JagBytMethod *method);

void jag_dec_import_repository_attach_method_header(JagDecImportRepository *import_repository, JagBytMethodHeader *method_header);

void jag_dec_import_repository_attach_exceptions(JagDecImportRepository *import_repository, JagBytExceptions *excpetions);

JagAstDeclarationType *jag_dec_import_repository_attach_declaration_type(JagDecImportRepository *import_repository, JagAstDeclarationType *declaration_type, gboolean quit);

G_END_DECLS

#endif /* JAGDECIMPORTREPOSITORY_H_ */
