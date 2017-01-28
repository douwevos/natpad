/*
   File:    mooicontent.c
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


#include "mooicontent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooIContent"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(MooIContent, moo_icontent, G_TYPE_OBJECT);

static void moo_icontent_default_init (MooIContentInterface *iface) {
	iface->equal = NULL;
}


CatStringWo *moo_icontent_get_key(MooIContent *self) {
	g_return_val_if_fail(MOO_IS_ICONTENT(self), NULL);
	return MOO_ICONTENT_GET_INTERFACE(self)->getKey(self);
}

MooIContent *moo_icontent_anchor(MooIContent *self, int version) {
	g_return_val_if_fail(MOO_IS_ICONTENT(self), NULL);
	return MOO_ICONTENT_GET_INTERFACE(self)->anchor(self, version);
}


gboolean moo_icontent_equal(MooIContent *content_a, MooIContent *content_b) {
	if (content_a==content_b) {
		return TRUE;
	}
	if (content_a==NULL || content_b==NULL) {
		return TRUE;
	}
	if (((GTypeInstance *) content_a)->g_class->g_type == ((GTypeInstance *)  content_b)->g_class->g_type) {
		MooIContentInterface *iface = MOO_ICONTENT_GET_INTERFACE(content_a);
		if (iface->equal) {
			return iface->equal(content_a, content_b);
		} else {
			cat_log_fatal("NOT IMPLEMENTED: mooicontent->equal %s", g_type_name_from_instance((GTypeInstance *) content_a));
		}
	}
	return FALSE;
}
