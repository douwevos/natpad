/*
   File:    vipcdprovider.c
   Project: viper
   Author:  Douwe Vos
   Date:    Aug 9, 2014
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

#include "vipcdprovider.h"
#include "vipicdprovider.h"
#include "vippathprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipCdProvider"
#include <logging/catlog.h>

struct _VipCdProviderPrivate {
	VipPath *current_directory;
	VipPath *root;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_cd_provider_iface_init(VipICdProviderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipCdProvider, vip_cd_provider, G_TYPE_OBJECT,
		G_ADD_PRIVATE(VipCdProvider)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ICD_PROVIDER, l_cd_provider_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_cd_provider_class_init(VipCdProviderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_cd_provider_init(VipCdProvider *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipCdProvider *instance = VIP_CD_PROVIDER(object);
	VipCdProviderPrivate *priv = vip_cd_provider_get_instance_private(instance);
	cat_unref_ptr(priv->root);
	cat_unref_ptr(priv->current_directory);
	G_OBJECT_CLASS(vip_cd_provider_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_cd_provider_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipCdProvider *vip_cd_provider_new() {
	VipCdProvider *result = g_object_new(VIP_TYPE_CD_PROVIDER, NULL);
	cat_ref_anounce(result);
	VipCdProviderPrivate *priv = vip_cd_provider_get_instance_private(result);

	char *cwd = getcwd(NULL, 0);
	priv->current_directory = vip_path_new(cat_string_wo_new_with(cwd));
	cat_free_ptr(cwd);


#ifdef G_OS_WIN32
	priv->root = vip_path_create_drive_path(priv->current_directory);
#else
	priv->root = vip_path_new(cat_string_wo_new_with("/"));
#endif
	cat_log_debug("root=%o, current_directory=%o", priv->root, priv->current_directory);

	return result;
}



static VipPath *l_cd_provider_get_cd(VipICdProvider *self, CatStringWo *a_drive) {
	VipCdProviderPrivate *priv = vip_cd_provider_get_instance_private(VIP_CD_PROVIDER(self));
	return priv->current_directory;
}

static VipPath *l_cd_provider_get_root(VipICdProvider *self, CatStringWo *a_drive) {
	VipCdProviderPrivate *priv = vip_cd_provider_get_instance_private(VIP_CD_PROVIDER(self));
	return priv->root;
}

static void l_cd_provider_iface_init(VipICdProviderInterface *iface) {
	iface->getCd = l_cd_provider_get_cd;
	iface->getRoot = l_cd_provider_get_root;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

