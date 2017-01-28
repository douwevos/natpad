/*
   File:    jagscanjarfilework.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 5, 2013
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

#include "jagscanjarfilework.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagScanJarFileWork"
#include <logging/catlog.h>

struct _JagScanJarFileWorkPrivate {
	VipNode *main_node;
};


static void l_scan_work_iface_init(VipIScanWorkInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagScanJarFileWork, jag_scan_jar_file_work, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ISCAN_WORK, l_scan_work_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_scan_jar_file_work_class_init(JagScanJarFileWorkClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagScanJarFileWorkPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_scan_jar_file_work_init(JagScanJarFileWork *instance) {
	JagScanJarFileWorkPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_SCAN_JAR_FILE_WORK, JagScanJarFileWorkPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagScanJarFileWork *instance = JAG_SCAN_JAR_FILE_WORK(object);
	JagScanJarFileWorkPrivate *priv = instance->priv;
	cat_unref_ptr(priv->main_node);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagScanJarFileWork *jag_scan_jar_file_work_new(VipNode *main_node, gboolean validated_by_parent) {
	JagScanJarFileWork *result = g_object_new(JAG_TYPE_SCAN_JAR_FILE_WORK, NULL);
	cat_ref_anounce(result);
	JagScanJarFileWorkPrivate *priv = result->priv;
	priv->main_node = cat_ref_ptr(main_node);
	return result;
}


/********************* begin VipIScanWork implementation *********************/

static CatArrayWo *l_scan_work_init_child_work(VipIScanWork *self, struct _VipMapperRegistry *mapper_registry) {
	return NULL;
}

static void l_scan_work_run_scan(VipIScanWork *self) {
//	JagScanJarFileWorkPrivate *priv = JAG_SCAN_JAR_FILE_WORK_GET_PRIVATE(self);
}

static void l_scan_work_iface_init(VipIScanWorkInterface *iface) {
	iface->initChildWork = l_scan_work_init_child_work;
	iface->runScan = l_scan_work_run_scan;
}

/********************* end VipIScanWork implementation *********************/
