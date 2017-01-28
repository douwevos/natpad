/*
   File:    mooinodeexpander.c
   Project: moose
   Author:  Douwe Vos
   Date:    Jun 28, 2014
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

#include "mooinodeexpander.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooINodeExpander"
#include <logging/catlog.h>


static CatS moo_s_iservices_content_key = CAT_S_DEF("MooINodeExpander");

CatStringWo *a_moo_inode_expander_key() {
	return CAT_S(moo_s_iservices_content_key);
}

G_DEFINE_INTERFACE(MooINodeExpander, moo_inode_expander, G_TYPE_OBJECT);

static void moo_inode_expander_default_init (MooINodeExpanderInterface *iface) {
	iface->expand = NULL;
	iface->collapse = NULL;
}

gboolean moo_inode_expander_expand(MooINodeExpander *self, struct _MooNodeWo *node) {
	g_return_val_if_fail(MOO_IS_INODE_EXPANDER(self), FALSE);
	MooINodeExpanderInterface *iface = MOO_INODE_EXPANDER_GET_INTERFACE(self);
	if (iface->expand) {
		return iface->expand(self, node);
	}
	cat_log_error("class %o does not implement MooINodeExpander->expand", g_type_name_from_instance((GTypeInstance *) self) );
	return FALSE;
}

void moo_inode_expander_collapse(MooINodeExpander *self, struct _MooNodeWo *node) {
	g_return_if_fail(MOO_IS_INODE_EXPANDER(self));
	MooINodeExpanderInterface *iface = MOO_INODE_EXPANDER_GET_INTERFACE(self);
	if (iface->collapse) {
		iface->collapse(self, node);
		return;
	}
	cat_log_error("class %o does not implement MooINodeExpander->collapse", g_type_name_from_instance((GTypeInstance *) self) );
}
