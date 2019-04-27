/*
   File:    jagbytlabelrepository.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "jagbytlabelrepository.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytLabelRepository"
#include <logging/catlog.h>

struct _JagBytLabelRepositoryPrivate {
	CatHashMapWo *e_label_set;
	int hard_count;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytLabelRepository, jag_byt_label_repository, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_label_repository_class_init(JagBytLabelRepositoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_label_repository_init(JagBytLabelRepository *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytLabelRepository *instance = JAG_BYT_LABEL_REPOSITORY(object);
	JagBytLabelRepositoryPrivate *priv = jag_byt_label_repository_get_instance_private(instance);
	cat_unref_ptr(priv->e_label_set);
	G_OBJECT_CLASS(jag_byt_label_repository_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_label_repository_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytLabelRepository *jag_byt_label_repository_new() {
	JagBytLabelRepository *result = g_object_new(JAG_BYT_TYPE_LABEL_REPOSITORY, NULL);
	cat_ref_anounce(result);
	JagBytLabelRepositoryPrivate *priv = jag_byt_label_repository_get_instance_private(result);
	priv->e_label_set = cat_hash_map_wo_new((GHashFunc) cat_integer_hash, (GEqualFunc) cat_integer_equal);
	priv->hard_count = 0;
	return result;
}

JagBytLabel *jag_byt_label_repository_get(JagBytLabelRepository *label_repository, int mnemonicIndex) {
	JagBytLabelRepositoryPrivate *priv = jag_byt_label_repository_get_instance_private(label_repository);
	CatInteger *midx = cat_integer_new(mnemonicIndex);
	JagBytLabel *result = (JagBytLabel *) cat_hash_map_wo_get(priv->e_label_set, midx);
	cat_unref_ptr(midx);
	return result;
}


JagBytLabel *jag_byt_label_repository_create_soft(JagBytLabelRepository *label_repository, int mnemonicIndex, int offset) {
	JagBytLabelRepositoryPrivate *priv = jag_byt_label_repository_get_instance_private(label_repository);
	CatInteger *midx = cat_integer_new(mnemonicIndex);
	JagBytLabel *result = (JagBytLabel *) cat_hash_map_wo_get(priv->e_label_set, midx);
	if (result == NULL) {
		result = jag_byt_label_new(JAG_BYT_LABEL_TYPE_SOFT, mnemonicIndex, offset, NULL);
		cat_hash_map_wo_put(priv->e_label_set, (GObject *) midx, (GObject *) result);
		cat_unref(result);
	}
	cat_unref_ptr(midx);
	return result;
}

JagBytLabel *jag_byt_label_repository_create_hard(JagBytLabelRepository *label_repository, int mnemonicIndex, int bytecodeOffset) {
	JagBytLabelRepositoryPrivate *priv = jag_byt_label_repository_get_instance_private(label_repository);
	CatInteger *midx = cat_integer_new(mnemonicIndex);
	JagBytLabel *result = (JagBytLabel *) cat_hash_map_wo_get(priv->e_label_set, midx);
	if (result == NULL) {
		char buf[50];
		sprintf(buf, "lab%d", priv->hard_count++);
		result = jag_byt_label_new(JAG_BYT_LABEL_TYPE_HARD, mnemonicIndex, bytecodeOffset, cat_string_wo_new_with(buf));
		cat_hash_map_wo_put(priv->e_label_set, (GObject *) midx, (GObject *) result);
		cat_unref(result);
	} else if (jag_byt_label_get_label_type(result)==JAG_BYT_LABEL_TYPE_SOFT) {
		char buf[50];
		sprintf(buf, "lab%d", priv->hard_count++);
		jag_byt_label_set_label_type(result, JAG_BYT_LABEL_TYPE_HARD);
		jag_byt_label_set_name(result, cat_string_wo_new_with(buf));
	}
	cat_unref_ptr(midx);
	return result;
}
