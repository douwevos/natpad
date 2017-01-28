/*
   File:    mooicontent.h
   Project: moose
   Author:  Douwe Vos
   Date:    Oct 31, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef MOOICONTENT_H_
#define MOOICONTENT_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_ICONTENT                 (moo_icontent_get_type())
#define MOO_ICONTENT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_ICONTENT, MooIContent))
#define MOO_IS_ICONTENT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_ICONTENT))
#define MOO_ICONTENT_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_ICONTENT, MooIContentInterface))


typedef struct _MooIContent               MooIContent; /* dummy object */
typedef struct _MooIContentInterface      MooIContentInterface;


struct _MooIContentMatcher;

struct _MooIContentInterface {
	GTypeInterface parent_iface;

	CatStringWo *(*getKey)(MooIContent *self);

	MooIContent *(*anchor)(MooIContent *self, int version);

	gboolean (*equal)(MooIContent *content_a, MooIContent *content_b);
};

GType moo_icontent_get_type(void);

CatStringWo *moo_icontent_get_key(MooIContent *self);

MooIContent *moo_icontent_anchor(MooIContent *self, int version);

gboolean moo_icontent_equal(MooIContent *content_a, MooIContent *content_b);

G_END_DECLS

#endif /* MOOICONTENT_H_ */
