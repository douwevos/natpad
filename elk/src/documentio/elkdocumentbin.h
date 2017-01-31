/*
   File:    elkdocumentbin.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 3, 2015
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

#ifndef ELKDOCUMENTBIN_H_
#define ELKDOCUMENTBIN_H_

#include <caterpillar.h>
#include <viper.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define ELK_TYPE_DOCUMENT_BIN              (elk_document_bin_get_type())
#define ELK_DOCUMENT_BIN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_document_bin_get_type(), ElkDocumentBin))
#define ELK_DOCUMENT_BIN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_DOCUMENT_BIN, ElkDocumentBinClass))
#define ELK_IS_DOCUMENT_BIN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_DOCUMENT_BIN))
#define ELK_IS_DOCUMENT_BIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_DOCUMENT_BIN))
#define ELK_DOCUMENT_BIN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_DOCUMENT_BIN, ElkDocumentBinClass))


typedef struct _ElkDocumentBin               ElkDocumentBin;
typedef struct _ElkDocumentBinPrivate        ElkDocumentBinPrivate;
typedef struct _ElkDocumentBinClass          ElkDocumentBinClass;


struct _ElkDocumentBin {
	GObject parent;
};

struct _ElkDocumentBinClass {
	GObjectClass parent_class;
};


GType elk_document_bin_get_type();

ElkDocumentBin *elk_document_bin_new(ChaDocumentManager *document_manager, VipService *vip_service, VipIFile *vip_file);
ElkDocumentBin *elk_document_bin_new_empty(ChaDocumentManager *document_manager, VipService *vip_service, int number);


gboolean elk_document_bin_update_read_only(ElkDocumentBin *document_bin, gboolean do_udpate);


void elk_document_bin_set_document(ElkDocumentBin *document_bin, ChaDocument *document);
ChaDocument *elk_document_bin_get_document(ElkDocumentBin *document_bin);
ChaDocumentManager *elk_document_bin_get_document_manager(ElkDocumentBin *document_bin);

ChaDocument *elk_document_bin_get_or_create_document(ElkDocumentBin *document_bin);
void elk_document_bin_revert(ElkDocumentBin *document_bin);
gboolean elk_document_bin_has_file(const ElkDocumentBin *document_bin);
CatStringWo *elk_document_bin_get_name(ElkDocumentBin *document_bin);
void elk_document_bin_store(ElkDocumentBin *document_bin, ChaIOAsync *async);

void elk_document_bin_store_as(ElkDocumentBin *document_bin, CatStringWo *path, ChaIConverter *converter, ChaIOAsync *async);
void elk_document_bin_set_path(ElkDocumentBin *document_bin, CatStringWo *path);

void elk_document_bin_increase_usage(ElkDocumentBin *document_bin);
gboolean elk_document_bin_decrease_usage(ElkDocumentBin *document_bin);

G_END_DECLS

#endif /* ELKDOCUMENTBIN_H_ */
