/*
   File:    shoblockvalue.c
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
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



#include "shoblockvalue.h"
#include "shoivalue.h"
#include "shoreferencelistprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoBlockValue"
#include <logging/catlog.h>

struct _ShoBlockValuePrivate {
	ShoTemplateCall *template_call;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_value_iface_init(ShoIValueInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ShoBlockValue, sho_block_value, SHO_TYPE_REFERENCE_LIST,
		G_ADD_PRIVATE(ShoBlockValue)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(SHO_TYPE_IVALUE, l_value_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_block_value_class_init(ShoBlockValueClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_block_value_init(ShoBlockValue *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoBlockValue *instance = SHO_BLOCK_VALUE(object);
	ShoBlockValuePrivate *priv = sho_block_value_get_instance_private(instance);
	cat_unref_ptr(priv->template_call);
	G_OBJECT_CLASS(sho_block_value_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_block_value_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ShoBlockValue *sho_block_value_new() {
	ShoBlockValue *result = g_object_new(SHO_TYPE_BLOCK_VALUE, NULL);
	cat_ref_anounce(result);
	ShoBlockValuePrivate *priv = sho_block_value_get_instance_private(result);
	priv->template_call = NULL;
	sho_reference_list_construct((ShoReferenceList *) result);
	return result;
}

void sho_block_value_set_template_call(ShoBlockValue *block, ShoTemplateCall *template_call) {
	ShoBlockValuePrivate *priv = sho_block_value_get_instance_private(block);
	cat_ref_swap(priv->template_call, template_call);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ShoBlockValue *instance = SHO_BLOCK_VALUE(self);
	ShoBlockValuePrivate *priv = sho_block_value_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %o]", iname, self, priv->template_call);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



/********************* start ShoIValue implementation *********************/

static ShoIValue *l_value_deep_copy(ShoIValue *self) {
	ShoBlockValue *result = g_object_new(SHO_TYPE_BLOCK_VALUE, NULL);
	cat_ref_anounce(result);
	CatArrayWo *source_ref_list = sho_reference_list_get_list((ShoReferenceList *) self);
	sho_reference_list_construct_with((ShoReferenceList *) result, source_ref_list);
	ShoBlockValuePrivate *priv = sho_block_value_get_instance_private(result);
	ShoBlockValuePrivate *spriv = sho_block_value_get_instance_private((ShoBlockValue *) self);
	priv->template_call = NULL;

	/* deep copy remplate call */
	if (spriv->template_call) {
		priv->template_call = sho_template_call_deep_copy(spriv->template_call);
	}

	return (ShoIValue *) result;
}

static void l_value_iface_init(ShoIValueInterface *iface) {
	iface->deepCopy = l_value_deep_copy;

}

/********************* end ShoIValue implementation *********************/
