/*
   File:    jaglink.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#ifndef JAGLINK_H_
#define JAGLINK_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_LINK              (jag_link_get_type())
#define JAG_LINK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_link_get_type(), JagLink))
#define JAG_LINK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_LINK, JagLinkClass))
#define JAG_IS_LINK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_LINK))
#define JAG_IS_LINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_LINK))
#define JAG_LINK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_LINK, JagLinkClass))
#define JAG_LINK_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_LINK, JagLinkPrivate))


typedef struct _JagLink               JagLink;
typedef struct _JagLinkPrivate        JagLinkPrivate;
typedef struct _JagLinkClass          JagLinkClass;


struct _JagLink {
	GObject parent;
};

struct _JagLinkClass {
	GObjectClass parent_class;
	void (*setValue)(JagLink *link, GObject *value);
	GObject *(*getValueRef)(JagLink *link);
};

GType jag_link_get_type();

void jag_link_construct(JagLink *link);


void jag_link_add_referred_by(JagLink *link, GObject *content);
void jag_link_remove_referred_by(JagLink *link, GObject *content);
void jag_link_notify(JagLink *link, GObject *notify_data);


void jag_link_set_value(JagLink *link, GObject *value);
GObject *jag_link_get_value_ref(JagLink *link);

G_END_DECLS

#endif /* JAGLINK_H_ */
