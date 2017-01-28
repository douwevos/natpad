/*
   File:    jagjarentrynotify.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 16, 2012
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


#ifndef JAGJARENTRYNOTIFY_H_
#define JAGJARENTRYNOTIFY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_JAR_ENTRY_NOTIFY              (jag_jar_entry_notify_get_type())
#define JAG_JAR_ENTRY_NOTIFY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jar_entry_notify_get_type(), JagJarEntryNotify))
#define JAG_JAR_ENTRY_NOTIFY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JAR_ENTRY_NOTIFY, JagJarEntryNotifyClass))
#define JAG_IS_JAR_ENTRY_NOTIFY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JAR_ENTRY_NOTIFY))
#define JAG_IS_JAR_ENTRY_NOTIFY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JAR_ENTRY_NOTIFY))
#define JAG_JAR_ENTRY_NOTIFY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JAR_ENTRY_NOTIFY, JagJarEntryNotifyClass))
#define JAG_JAR_ENTRY_NOTIFY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_JAR_ENTRY_NOTIFY, JagJarEntryNotifyPrivate))

typedef struct _JagJarEntryNotify               JagJarEntryNotify;
typedef struct _JagJarEntryNotifyPrivate        JagJarEntryNotifyPrivate;
typedef struct _JagJarEntryNotifyClass          JagJarEntryNotifyClass;


struct _JagJarEntryNotify {
	GObject parent;
	JagJarEntryNotifyPrivate *priv;
};

struct _JagJarEntryNotifyClass {
	GObjectClass parent_class;
};


GType jag_jar_entry_notify_get_type();

JagJarEntryNotify *jag_jar_entry_notify_new();

G_END_DECLS


#endif /* JAGJARENTRYNOTIFY_H_ */
