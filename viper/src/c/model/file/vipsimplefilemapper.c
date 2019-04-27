/*
   File:    vipsimplefilemapper.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
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

#include "vipsimplefilemapper.h"
#include "vipfsscanwork.h"
#include "../scanner/vipimapper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipSimpleFileMapper"
#include <logging/catlog.h>

struct _VipSimpleFileMapperPrivate {
	VipISequence *sequence;
};

static void l_mapper_iface_init(VipIMapperInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipSimpleFileMapper, vip_simple_file_mapper, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(VipSimpleFileMapper)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IMAPPER, l_mapper_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_simple_file_mapper_class_init(VipSimpleFileMapperClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_simple_file_mapper_init(VipSimpleFileMapper *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	VipSimpleFileMapper *instance = VIP_SIMPLE_FILE_MAPPER(object);
//	VipSimpleFileMapperPrivate *priv = instance->priv;
//	cat_unref_ptr(priv->sequence);
	G_OBJECT_CLASS(vip_simple_file_mapper_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(vip_simple_file_mapper_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipSimpleFileMapper *vip_simple_file_mapper_new(VipISequence *sequence) {
	VipSimpleFileMapper *result = g_object_new(VIP_TYPE_SIMPLE_FILE_MAPPER, NULL);
	cat_ref_anounce(result);
	VipSimpleFileMapperPrivate *priv = vip_simple_file_mapper_get_instance_private(result);
	priv->sequence = sequence;
	return result;
}


/********************* start VipIMapper implementation *********************/

static VipIScanWork *l_mapper_create_work_for_node(VipIMapper *self, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent) {
	VipSimpleFileMapper *instance = VIP_SIMPLE_FILE_MAPPER(self);
	VipSimpleFileMapperPrivate *priv = vip_simple_file_mapper_get_instance_private(instance);
	return (VipIScanWork *) vip_fs_scan_work_new(priv->sequence, node, recursive_from_parent, validated_by_parent);
}


static void l_mapper_iface_init(VipIMapperInterface *iface) {
	iface->createWorkForNode = l_mapper_create_work_for_node;
}

/********************* end VipIMapper implementation *********************/
