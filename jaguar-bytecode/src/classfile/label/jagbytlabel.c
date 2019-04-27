/*
   File:    jagbytlabel.c
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

#include "jagbytlabel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytLabel"
#include <logging/catlog.h>

struct _JagBytLabelPrivate {
	int mnemonicIndex;
	int bytecodeOffset;
	CatStringWo *a_name;
	JagBytLabelType type;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytLabel, jag_byt_label, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_label_class_init(JagBytLabelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_label_init(JagBytLabel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytLabel *instance = JAG_BYT_LABEL(object);
	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(instance);
	cat_unref_ptr(priv->a_name);
	G_OBJECT_CLASS(jag_byt_label_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_label_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytLabel *jag_byt_label_new(JagBytLabelType type, int mnemonicIndex, int bytecodeOffset, CatStringWo *a_name) {
	JagBytLabel *result = g_object_new(JAG_BYT_TYPE_LABEL, NULL);
	cat_ref_anounce(result);
	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(result);
	priv->mnemonicIndex = mnemonicIndex;
	priv->bytecodeOffset = bytecodeOffset;
	if (a_name==NULL) {
		char temp[100];
		sprintf(temp, "Label(%d:%04x)", mnemonicIndex, bytecodeOffset);
		priv->a_name = cat_string_wo_new_with(temp);
	} else {
		priv->a_name = cat_ref_ptr(a_name);
	}
	priv->type = type;
	return result;
}

void jag_byt_label_set_name(JagBytLabel *label, CatStringWo *a_newName) {
	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(label);
	cat_ref_swap(priv->a_name, a_newName);
}

CatStringWo *jag_byt_label_get_name(JagBytLabel *label) {
	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(label);
	return priv->a_name;
}

void jag_byt_label_set_label_type(JagBytLabel *label, JagBytLabelType type) {
	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(label);
	priv->type = type;
}

JagBytLabelType jag_byt_label_get_label_type(JagBytLabel *label) {
	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(label);
	return priv->type;
}

gboolean jag_byt_label_equal(JagBytLabel *label, JagBytLabel *other) {
	if (label==other) {
		return TRUE;
	}
	if (label==NULL || other==NULL) {
		return FALSE;
	}

	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(label);
	JagBytLabelPrivate *opriv = jag_byt_label_get_instance_private(other);
	return priv->mnemonicIndex==opriv->mnemonicIndex && priv->bytecodeOffset==opriv->bytecodeOffset;
}

int jag_byt_label_hash(JagBytLabel *label) {
	JagBytLabelPrivate *priv = jag_byt_label_get_instance_private(label);
	return priv->mnemonicIndex*15+priv->bytecodeOffset;
}
