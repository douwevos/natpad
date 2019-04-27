/*
   File:    jgijarentry.h
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

#ifndef JGIJARENTRY_H_
#define JGIJARENTRY_H_

#include "../../link/jagilinklistener.h"
#include <caterpillar.h>
#include <viper.h>
#include <moose.h>

G_BEGIN_DECLS

#define JGI_TYPE_JAR_ENTRY              (jgi_jar_entry_get_type())
#define JGI_JAR_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jgi_jar_entry_get_type(), JgiJarEntry))
#define JGI_JAR_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JGI_TYPE_JAR_ENTRY, JgiJarEntryClass))
#define JGI_IS_JAR_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JGI_TYPE_JAR_ENTRY))
#define JGI_IS_JAR_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JGI_TYPE_JAR_ENTRY))
#define JGI_JAR_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JGI_TYPE_JAR_ENTRY, JgiJarEntryClass))

typedef struct _JgiJarEntry               JgiJarEntry;
typedef struct _JgiJarEntryPrivate        JgiJarEntryPrivate;
typedef struct _JgiJarEntryClass          JgiJarEntryClass;


struct _JgiJarEntry {
	GObject parent;
};

struct _JgiJarEntryClass {
	GObjectClass parent_class;
};

GType jgi_jar_entry_get_type();

JgiJarEntry *jgi_jar_entry_new(VipISequence *moo_sequence, VipNodePath *vip_node_path);

void jgi_jar_entry_usage_up(JgiJarEntry *entry);
gboolean jgi_jar_entry_usage_down(JgiJarEntry *entry);

VipNodePath *jgi_jar_entry_get_vip_node_path(JgiJarEntry *entry);

VipSnapshot *jgi_jar_entry_get_last_vip_snapshot(JgiJarEntry *entry);

MooNodeWo *jgi_jar_entry_get_moo_node_ref(JgiJarEntry *entry);

void jgi_jar_entry_add_link_listener(JgiJarEntry *entry, JagILinkListener *link_listener);
void jgi_jar_entry_remove_link_listener(JgiJarEntry *entry, JagILinkListener *link_listener);

G_END_DECLS

#endif /* JGIJARENTRY_H_ */
