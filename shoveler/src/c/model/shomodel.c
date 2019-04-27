/*
   File:    shomodel.c
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

#include "shomodel.h"
#include "shoreferencelistprivate.h"
#include "../serialize/shoserializer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoModel"
#include <logging/catlog.h>

struct _ShoModelPrivate {
	CatHashMapWo *templates;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ShoModel, sho_model, SHO_TYPE_REFERENCE_LIST,
		G_ADD_PRIVATE(ShoModel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_model_class_init(ShoModelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_model_init(ShoModel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoModel *instance = SHO_MODEL(object);
	ShoModelPrivate *priv = sho_model_get_instance_private(instance);
	cat_unref_ptr(priv->templates);
	G_OBJECT_CLASS(sho_model_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_model_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ShoModel *sho_model_new() {
	ShoModel *result = g_object_new(SHO_TYPE_MODEL, NULL);
	cat_ref_anounce(result);
	ShoModelPrivate *priv = sho_model_get_instance_private(result);
	priv->templates = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	sho_reference_list_construct((ShoReferenceList *) result);
	return result;
}

ShoModel *sho_model_deep_copy(ShoModel *source) {
	ShoModel *result = g_object_new(SHO_TYPE_MODEL, NULL);
	cat_ref_anounce(result);
	CatArrayWo *reference_list = sho_reference_list_get_list((ShoReferenceList *) source);
	sho_reference_list_construct_with((ShoReferenceList *) result, reference_list);
	ShoModelPrivate *priv = sho_model_get_instance_private(result);
	ShoModelPrivate *spriv = sho_model_get_instance_private(source);
	priv->templates = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	/* deep copy all templates */
	CatArrayWo *key_list = cat_hash_map_wo_enlist_keys(spriv->templates, NULL);
	if (key_list) {
		CatIIterator *iter = cat_array_wo_iterator(key_list);
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *id = (CatStringWo *) cat_iiterator_next(iter);
			ShoTemplate *template = (ShoTemplate *) cat_hash_map_wo_get(spriv->templates, id);
			ShoTemplate *new_template = sho_template_deep_copy(template);
			cat_hash_map_wo_put(priv->templates, (GObject *) id, (GObject *) new_template);
			cat_unref_ptr(new_template);
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(key_list);
	}

	return result;
}

void sho_model_add_template(ShoModel *model, ShoTemplate *stemplate) {
	ShoModelPrivate *priv = sho_model_get_instance_private(model);
	cat_hash_map_wo_put(priv->templates, (GObject *) sho_template_get_name(stemplate), (GObject *) stemplate);
}


void sho_model_dump(ShoModel *model) {
	cat_log_print("DUMP", "model=%p", model);
	if (model) {
		CatArrayWo *list = sho_reference_list_get_list((ShoReferenceList *) model);
		CatIIterator *iter = cat_array_wo_iterator(list);
		while(cat_iiterator_has_next(iter)) {
			ShoReference *ref = (ShoReference *) cat_iiterator_next(iter);
			cat_log_print("DUMP", "ref=%p", ref);
		}
		cat_unref_ptr(iter);
	}
	fflush(stdout);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ShoModel *instance = SHO_MODEL(self);
//	ShoModelPrivate *priv = sho_model_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	ShoSerializer *serializer = sho_serializer_new(instance);
	CatStringWo *serialized = sho_serialize_to_char_array(serializer);

	cat_string_wo_format(append_to, "%s[%p: %o]", iname, self, serialized);
	cat_unref_ptr(serialized);
	cat_unref_ptr(serializer);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
