/*
   File:    elkdocumentio.h
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 10, 2012
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

#ifndef ELKDOCUMENTIO_H_
#define ELKDOCUMENTIO_H_

#include "elkdocumentbin.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define ELK_TYPE_DOCUMENT_IO              (elk_document_io_get_type())
#define ELK_DOCUMENT_IO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_document_io_get_type(), ElkDocumentIO))
#define ELK_DOCUMENT_IO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_DOCUMENT_IO, ElkDocumentIOClass))
#define ELK_IS_DOCUMENT_IO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_DOCUMENT_IO))
#define ELK_IS_DOCUMENT_IO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_DOCUMENT_IO))
#define ELK_DOCUMENT_IO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_DOCUMENT_IO, ElkDocumentIOClass))

typedef struct _ElkDocumentIO               ElkDocumentIO;
typedef struct _ElkDocumentIOPrivate        ElkDocumentIOPrivate;
typedef struct _ElkDocumentIOClass          ElkDocumentIOClass;


struct _ElkDocumentIO {
	GObject parent;
};

struct _ElkDocumentIOClass {
	GObjectClass parent_class;
};


GType elk_document_io_get_type();

ElkDocumentIO *elk_document_io_new(VipService *vip_service, ChaDocumentManager *document_manager);

ElkDocumentBin *elk_document_io_open_document_for_file(ElkDocumentIO *document_io, VipIFile *vip_file);

ElkDocumentBin *elk_document_io_open_new_document(ElkDocumentIO *document_io);

G_END_DECLS

#endif /* ELKDOCUMENTIO_H_ */
