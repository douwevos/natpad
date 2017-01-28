/*
   File:    chadocumentmanager.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 25, 2015
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

#ifndef CHADOCUMENTMANAGER_H_
#define CHADOCUMENTMANAGER_H_

#include "chaioasync.h"
#include "../document/chadocument.h"
#include "../charset/chaiconverter.h"
#include <caterpillar.h>
#include <worm.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define CHA_TYPE_DOCUMENT_MANAGER              (cha_document_manager_get_type())
#define CHA_DOCUMENT_MANAGER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_document_manager_get_type(), ChaDocumentManager))
#define CHA_DOCUMENT_MANAGER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_DOCUMENT_MANAGER, ChaDocumentManagerClass))
#define CHA_IS_DOCUMENT_MANAGER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_DOCUMENT_MANAGER))
#define CHA_IS_DOCUMENT_MANAGER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_DOCUMENT_MANAGER))
#define CHA_DOCUMENT_MANAGER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_DOCUMENT_MANAGER, ChaDocumentManagerClass))


typedef struct _ChaDocumentManager               ChaDocumentManager;
typedef struct _ChaDocumentManagerPrivate        ChaDocumentManagerPrivate;
typedef struct _ChaDocumentManagerClass          ChaDocumentManagerClass;


struct _ChaDocumentManager {
	GObject parent;
};

struct _ChaDocumentManagerClass {
	GObjectClass parent_class;
};


GType cha_document_manager_get_type();

ChaDocumentManager *cha_document_manager_new();

ChaIConverter *cha_document_manager_get_converter(ChaDocumentManager *document_manager, const char *charset_name);

void cha_document_manager_load(ChaDocumentManager *document_manager, ChaDocument *document, GFile *file);

void cha_document_manager_write(ChaDocumentManager *document_manager, ChaDocument *document, GFile *file, ChaIOAsync *async);

void cha_document_manager_post(ChaDocumentManager *document_manager, WorRequest *request);

G_END_DECLS

#endif /* CHADOCUMENTMANAGER_H_ */
