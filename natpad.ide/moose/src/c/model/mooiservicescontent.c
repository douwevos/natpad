/*
   File:    mooiservicescontent.c
   Project: moose
   Author:  Douwe Vos
   Date:    Mar 14, 2014
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

#include "mooiservicescontent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooIServicesContent"
#include <logging/catlog.h>


static CatS moo_s_iservices_content_key = CAT_S_DEF("MooIServicesContent");

CatStringWo *moo_iservices_content_key() {
	return CAT_S(moo_s_iservices_content_key);
}

G_DEFINE_INTERFACE(MooIServicesContent, moo_iservices_content, G_TYPE_OBJECT);

static void moo_iservices_content_default_init (MooIServicesContentInterface *iface) {
	iface->canDelete = NULL;
	iface->delete = NULL;
}

gboolean moo_iservices_content_can_refresh(MooIServicesContent *self) {
	g_return_val_if_fail(MOO_IS_ISERVICES_CONTENT(self), FALSE);
	return MOO_ISERVICES_CONTENT_GET_INTERFACE(self)->canRefresh(self);
}

void moo_iservices_content_refresh(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node) {
	g_return_if_fail(MOO_IS_ISERVICES_CONTENT(self));
	MOO_ISERVICES_CONTENT_GET_INTERFACE(self)->refresh(self, moo_service, node);
}

gboolean moo_iservices_content_can_rename(MooIServicesContent *self) {
	g_return_val_if_fail(MOO_IS_ISERVICES_CONTENT(self), FALSE);
	return MOO_ISERVICES_CONTENT_GET_INTERFACE(self)->canRename(self);
}

gboolean moo_iservices_content_rename(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame) {
	g_return_val_if_fail(MOO_IS_ISERVICES_CONTENT(self), FALSE);
	return MOO_ISERVICES_CONTENT_GET_INTERFACE(self)->rename(self, moo_service, node, frame);
}

gboolean moo_iservices_content_can_delete(MooIServicesContent *self) {
	g_return_val_if_fail(MOO_IS_ISERVICES_CONTENT(self), FALSE);
	MooIServicesContentInterface *iface = MOO_ISERVICES_CONTENT_GET_INTERFACE(self);
	if (iface->canDelete) {
		return iface->canDelete(self);
	}
	cat_log_error("class %o does not implement MooIServiceContent->canDelete", g_type_name_from_instance((GTypeInstance *) self) );
	return FALSE;
}

gboolean moo_iservices_content_delete(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame) {
	g_return_val_if_fail(MOO_IS_ISERVICES_CONTENT(self), FALSE);
	MooIServicesContentInterface *iface = MOO_ISERVICES_CONTENT_GET_INTERFACE(self);
	if (iface->delete) {
		return iface->delete(self, moo_service, node, frame);
	}
	cat_log_error("class %o does not implement MooIServiceContent->delete", g_type_name_from_instance((GTypeInstance *) self) );
	return FALSE;
}
