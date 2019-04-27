/*
   File:    jgijreentry.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 17, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef JGIJREENTRY_H_
#define JGIJREENTRY_H_

#include "../jar/jgijarentry.h"
#include "jagindexerjremap.h"
#include "../../preferences/jagprefsjrewo.h"
#include "../../link/jaglink.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JGI_TYPE_JRE_ENTRY              (jgi_jre_entry_get_type())
#define JGI_JRE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jgi_jre_entry_get_type(), JgiJreEntry))
#define JGI_JRE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JGI_TYPE_JRE_ENTRY, JgiJreEntryClass))
#define JGI_IS_JRE_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JGI_TYPE_JRE_ENTRY))
#define JGI_IS_JRE_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JGI_TYPE_JRE_ENTRY))
#define JGI_JRE_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JGI_TYPE_JRE_ENTRY, JgiJreEntryClass))

typedef struct _JgiJreEntry               JgiJreEntry;
typedef struct _JgiJreEntryPrivate        JgiJreEntryPrivate;
typedef struct _JgiJreEntryClass          JgiJreEntryClass;


struct _JgiJreEntry {
	GObject parent;
};

struct _JgiJreEntryClass {
	GObjectClass parent_class;
};


GType jgi_jre_entry_get_type();

JgiJreEntry *jgi_jre_entry_new(WorService *wor_service, VipISequence *moo_sequence, JagPrefsJreWo *a_jre, JagLink *jre_link);

CatStringWo *jgi_jre_entry_get_name(JgiJreEntry *entry);

MooNodeWo *jgi_jre_entry_get_moo_node_ref(JgiJreEntry *entry);

gboolean jgi_jre_entry_set_jar_entry_list(JgiJreEntry *jre_entry, JagIndexerJarMap *jar_map, CatArrayWo *new_jar_entry_list, int jre_list_version);

G_END_DECLS

#endif /* JGIJREENTRY_H_ */
