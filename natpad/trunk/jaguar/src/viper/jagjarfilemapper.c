/*
   File:    jagjarfilemapper.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 27, 2013
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

#include "jagjarfilemapper.h"
#include "jagscanjarmapwork.h"
#include "jagscanjarfilework.h"
#include "jagvipjarroot.h"
#include "jagvipjarfile.h"
#include "jagvipjarmap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJarFileMapper"
#include <logging/catlog.h>

struct _JagJarFileMapperPrivate {
	VipISequence *sequence;
};

static void l_mapper_iface_init(VipIMapperInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagJarFileMapper, jag_jar_file_mapper, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(VIP_TYPE_IMAPPER, l_mapper_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_jar_file_mapper_class_init(JagJarFileMapperClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagJarFileMapperPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_jar_file_mapper_init(JagJarFileMapper *instance) {
	JagJarFileMapperPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_JAR_FILE_MAPPER, JagJarFileMapperPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagJarFileMapper *instance = JAG_JAR_FILE_MAPPER(object);
	JagJarFileMapperPrivate *priv = instance->priv;
	cat_unref_ptr(priv->sequence);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagJarFileMapper *jag_jar_file_mapper_new(VipISequence *sequence) {
	JagJarFileMapper *result = g_object_new(JAG_TYPE_JAR_FILE_MAPPER, NULL);
	cat_ref_anounce(result);
	JagJarFileMapperPrivate *priv = result->priv;
	priv->sequence = cat_ref_ptr(sequence);
	return result;
}




/********************* start VipIMapper implementation *********************/


static CatS l_s_txt_jar = CAT_S_DEF(".jar");
static CatS l_s_txt_ear = CAT_S_DEF(".ear");
static CatS l_s_txt_war = CAT_S_DEF(".war");

static VipIScanWork *l_mapper_create_work_for_node2(VipIMapper *self, CatWritableTreeNode *tree_node, gboolean recursive_from_parent, gboolean validated_by_parent) {
	JagJarFileMapperPrivate *priv = JAG_JAR_FILE_MAPPER_GET_PRIVATE(self);


	VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) tree_node);
	VipIResource *content = vip_node_get_content(vip_node);

	if (JAG_IS_VIP_JAR_ROOT(content)) {
		return (VipIScanWork *) jag_scan_jar_map_work_new(priv->sequence, tree_node, TRUE, validated_by_parent);
	} else if (JAG_IS_VIP_JAR_MAP(content)) {
		return (VipIScanWork *) jag_scan_jar_map_work_new(priv->sequence, tree_node, recursive_from_parent, validated_by_parent);
	} else if (VIP_IS_IFILE(content)) {
		VipIFile *vipFile = (VipIFile *) content;
		CatStringWo *a_name = vip_iresource_get_name((VipIResource *) vipFile);
		cat_log_debug("testing name:%o", a_name);
		if (cat_string_wo_endswith(a_name, CAT_S(l_s_txt_jar))
				|| cat_string_wo_endswith(a_name, CAT_S(l_s_txt_ear))
				|| cat_string_wo_endswith(a_name, CAT_S(l_s_txt_war))) {

			JagVipJarRoot *vip_jar_root = jag_vip_jar_root_new(vipFile);
			VipNode *replacement_vip_node = vip_node_set_resource(vip_node, (VipIResource *) vip_jar_root);
			cat_writable_tree_node_set_content(tree_node, (GObject *) replacement_vip_node);
			cat_unref_ptr(replacement_vip_node);
			cat_unref_ptr(vip_jar_root);
			return (VipIScanWork *) jag_scan_jar_map_work_new(priv->sequence, tree_node, recursive_from_parent, validated_by_parent);
		}
		if (JAG_IS_VIP_JAR_FILE(content)) {
			return (VipIScanWork *) jag_scan_jar_file_work_new(vip_node, validated_by_parent);
		}
	}
	return NULL;
}

static VipIScanWork *l_mapper_create_work_for_node(VipIMapper *self, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent) {
	VipIScanWork *result = l_mapper_create_work_for_node2(self, node, recursive_from_parent, validated_by_parent);
	cat_log_debug("node=%o, result=%o", node, result);
	return result;
}



static void l_mapper_iface_init(VipIMapperInterface *iface) {
	iface->createWorkForNode = l_mapper_create_work_for_node;
}

/********************* end VipIMapper implementation *********************/
