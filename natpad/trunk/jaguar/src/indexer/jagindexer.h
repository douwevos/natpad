/*
   File:    jagindexer.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 15, 2012
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


#ifndef JAGINDEXER_H_
#define JAGINDEXER_H_


#include <caterpillar.h>
#include <worm.h>
#include <moose.h>
#include "../preferences/jagpreferenceswo.h"

G_BEGIN_DECLS

#define JAG_TYPE_INDEXER              (jag_indexer_get_type())
#define JAG_INDEXER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_indexer_get_type(), JagIndexer))
#define JAG_INDEXER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEXER, JagIndexerClass))
#define JAG_IS_INDEXER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEXER))
#define JAG_IS_INDEXER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEXER))
#define JAG_INDEXER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEXER, JagIndexerClass))
#define JAG_INDEXER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_INDEXER, JagIndexerPrivate))

typedef struct _JagIndexer               JagIndexer;
typedef struct _JagIndexerPrivate        JagIndexerPrivate;
typedef struct _JagIndexerClass          JagIndexerClass;


struct _JagIndexer {
	GObject parent;
	JagIndexerPrivate *priv;
};

struct _JagIndexerClass {
	GObjectClass parent_class;
};


GType jag_indexer_get_type();

JagIndexer *jag_indexer_new(WorService *wor_service, MooService *moo_service);
void jag_indexer_refresh(JagIndexer *indexer);

void jag_indexer_set_preferences(JagIndexer *indexer, JagPreferencesWo *a_jag_prefs);

MooService *jag_indexer_get_moo_service(JagIndexer *indexer);

G_END_DECLS

#endif /* JAGINDEXER_H_ */
