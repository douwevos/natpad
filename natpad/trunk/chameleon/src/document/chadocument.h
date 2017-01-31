/*
   File:    chadocument.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 23, 2015
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

#ifndef CHADOCUMENT_H_
#define CHADOCUMENT_H_

#include "charevisionwo.h"
#include "chaidocumentlistener.h"
#include "../charset/chaiconverter.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_DOCUMENT              (cha_document_get_type())
#define CHA_DOCUMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_document_get_type(), ChaDocument))
#define CHA_DOCUMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_DOCUMENT, ChaDocumentClass))
#define CHA_IS_DOCUMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_DOCUMENT))
#define CHA_IS_DOCUMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_DOCUMENT))
#define CHA_DOCUMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_DOCUMENT, ChaDocumentClass))


typedef struct _ChaDocument               ChaDocument;
typedef struct _ChaDocumentPrivate        ChaDocumentPrivate;
typedef struct _ChaDocumentClass          ChaDocumentClass;

enum _ChaDocumentAnchorMode {
	CHA_DOCUMENT_ANCHOR_MODE_NO_HISTORY_UPDATE,
	CHA_DOCUMENT_ANCHOR_MODE_ADD_HISTORY,
	CHA_DOCUMENT_ANCHOR_MODE_REPLACE_LAST_HISTORY,
};

typedef enum _ChaDocumentAnchorMode ChaDocumentAnchorMode;

struct _ChaDocumentManager;

struct _ChaDocument {
	GObject parent;
};

struct _ChaDocumentClass {
	GObjectClass parent_class;
	CatAtomicInteger *document_sequence;
};

GType cha_document_get_type();

void cha_document_construct(ChaDocument *document, struct _ChaDocumentManager *document_manager);

ChaDocument *cha_document_new(struct _ChaDocumentManager *document_manager);

ChaIConverter *cha_document_get_input_converter(ChaDocument *document);

void cha_document_set_input_converter(ChaDocument *document, ChaIConverter *converter);

int cha_document_get_id(ChaDocument *document);
struct _ChaDocumentManager *cha_document_get_document_manager(ChaDocument *document);


ChaLineEnd cha_document_get_line_end_user(ChaDocument *document);
void cha_document_set_line_end_user(ChaDocument *document, ChaLineEnd line_end);

void cha_document_set_big_file_mode(ChaDocument *document, gboolean big_file_mode);
gboolean cha_document_is_big_file_mode(ChaDocument *document);

void cha_document_set_saved_revision(ChaDocument *document, ChaRevisionWo *a_revision);
gboolean cha_document_is_saved_revision(const ChaDocument *document);

void cha_document_set_read_only(ChaDocument *document, gboolean read_only);
gboolean cha_document_is_read_only(const ChaDocument *document);

ChaRevisionWo *cha_document_get_current_revision_ref(ChaDocument *document);

ChaEnrichmentDataMapWo *cha_document_get_enrichment_data_map(ChaDocument *document);

void cha_document_undo(ChaDocument *document);
void cha_document_redo(ChaDocument *document);

gboolean cha_document_is_editable(ChaDocument *document);

ChaRevisionWo *cha_document_get_editable_revision(ChaDocument *document);

void cha_document_anchor_document(ChaDocument *document);
void cha_document_anchor_document_full(ChaDocument *document, ChaDocumentAnchorMode anchor_mode);


void cha_document_post_enrichment_slot_notify(ChaDocument *document, ChaRevisionWo *a_revision, GObject *slot_key, gpointer user_data);

int cha_document_register_enrichment_slot(ChaDocument *document, GObject *slot_key);
void cha_document_unregister_enrichment_slot(ChaDocument *document, GObject *slot_key);

int cha_document_hash_code(ChaDocument *document);
gboolean cha_document_equal(ChaDocument *document_a, ChaDocument *document_b);

void cha_document_add_listener(ChaDocument *document, ChaIDocumentListener *listener);
void cha_document_remove_listener(ChaDocument *document, ChaIDocumentListener *listener);

void cha_document_get_history_info(const ChaDocument *document, int *history_index, int *history_length);

CatArrayWo *cha_document_create_line_list(const ChaDocument *document, const CatStringWo *text);

G_END_DECLS

#endif /* CHADOCUMENT_H_ */
