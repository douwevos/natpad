/*
   File:    jagplainlink.h
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

#ifndef JAGPLAINLINK_H_
#define JAGPLAINLINK_H_

#include "jaglink.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_PLAIN_LINK              (jag_plain_link_get_type())
#define JAG_PLAIN_LINK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_plain_link_get_type(), JagPlainLink))
#define JAG_PLAIN_LINK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PLAIN_LINK, JagPlainLinkClass))
#define JAG_IS_PLAIN_LINK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PLAIN_LINK))
#define JAG_IS_PLAIN_LINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PLAIN_LINK))
#define JAG_PLAIN_LINK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PLAIN_LINK, JagPlainLinkClass))


typedef struct _JagPlainLink               JagPlainLink;
typedef struct _JagPlainLinkPrivate        JagPlainLinkPrivate;
typedef struct _JagPlainLinkClass          JagPlainLinkClass;


struct _JagPlainLink {
	JagLink parent;
};

struct _JagPlainLinkClass {
	JagLinkClass parent_class;
};


GType jag_plain_link_get_type();

JagPlainLink *jag_plain_link_new();

G_END_DECLS

#endif /* JAGPLAINLINK_H_ */
