/*
   File:    jagjrelink.h
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

#ifndef JAGJRELINK_H_
#define JAGJRELINK_H_

#include "../../link/jaglink.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_JRE_LINK              (jag_jre_link_get_type())
#define JAG_JRE_LINK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jre_link_get_type(), JagJreLink))
#define JAG_JRE_LINK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JRE_LINK, JagJreLinkClass))
#define JAG_IS_JRE_LINK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JRE_LINK))
#define JAG_IS_JRE_LINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JRE_LINK))
#define JAG_JRE_LINK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JRE_LINK, JagJreLinkClass))


typedef struct _JagJreLink               JagJreLink;
typedef struct _JagJreLinkPrivate        JagJreLinkPrivate;
typedef struct _JagJreLinkClass          JagJreLinkClass;

struct _JgiJreEntry;

struct _JagJreLink {
	JagLink parent;
};

struct _JagJreLinkClass {
	JagLinkClass parent_class;
};

GType jag_jre_link_get_type();

JagJreLink *jag_jre_link_new(CatStringWo *a_jre_name);

void jag_jre_link_set_entry(JagJreLink *link, struct _JgiJreEntry *entry);
struct _JgiJreEntry *jag_jre_link_get_entry_ref(JagJreLink *link);

G_END_DECLS

#endif /* JAGJRELINK_H_ */
