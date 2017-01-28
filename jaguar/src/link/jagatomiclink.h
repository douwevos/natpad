/*
   File:    jagatomiclink.h
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

#ifndef JAGATOMICLINK_H_
#define JAGATOMICLINK_H_

#include "jaglink.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_ATOMIC_LINK              (jag_atomic_link_get_type())
#define JAG_ATOMIC_LINK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_atomic_link_get_type(), JagAtomicLink))
#define JAG_ATOMIC_LINK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_ATOMIC_LINK, JagAtomicLinkClass))
#define JAG_IS_ATOMIC_LINK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_ATOMIC_LINK))
#define JAG_IS_ATOMIC_LINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_ATOMIC_LINK))
#define JAG_ATOMIC_LINK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_ATOMIC_LINK, JagAtomicLinkClass))


typedef struct _JagAtomicLink               JagAtomicLink;
typedef struct _JagAtomicLinkPrivate        JagAtomicLinkPrivate;
typedef struct _JagAtomicLinkClass          JagAtomicLinkClass;


struct _JagAtomicLink {
	JagLink parent;
};

struct _JagAtomicLinkClass {
	JagLinkClass parent_class;
};


GType jag_atomic_link_get_type();

JagAtomicLink *jag_atomic_link_new();

G_END_DECLS

#endif /* JAGATOMICLINK_H_ */
