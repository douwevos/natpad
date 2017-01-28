/*
   File:    shoreferencelist.c
   Project: shoveler
   Author:  Douwe Vos
   Date:    Oct 5, 2014
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

#include "shoreferencelistprivate.h"
#include "shoivalue.h"
#include "../shokeywords.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoReferenceList"
#include <logging/catlog.h>

struct _ShoReferenceListPrivate {
	CatArrayWo *references;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ShoReferenceList, sho_reference_list, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ShoReferenceList)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_reference_list_class_init(ShoReferenceListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_reference_list_init(ShoReferenceList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoReferenceList *instance = SHO_REFERENCE_LIST(object);
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(instance);
	cat_unref_ptr(priv->references);
	G_OBJECT_CLASS(sho_reference_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_reference_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void sho_reference_list_construct(ShoReferenceList *reference_list) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	priv->references = cat_array_wo_new();
}

void sho_reference_list_construct_with(ShoReferenceList *reference_list, CatArrayWo *list_to_copy) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	if (list_to_copy) {
		int cnt = cat_array_wo_size(list_to_copy);
		priv->references = cat_array_wo_new_size(cnt);
		if (cnt>0) {
			CatIIterator *iter = cat_array_wo_iterator(list_to_copy);
			while(cat_iiterator_has_next(iter)) {
				ShoReference *reference = (ShoReference *) cat_iiterator_next(iter);
				ShoReference *new_reference = sho_reference_deep_copy(reference);
				cat_array_wo_append(priv->references, (GObject *) new_reference);
				cat_unref_ptr(new_reference);
			}
			cat_unref_ptr(iter);
		}
	} else {
		priv->references = cat_array_wo_new();
	}
}



void sho_reference_list_add_reference(ShoReferenceList *reference_list, ShoReference *reference) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	cat_array_wo_append(priv->references, (GObject *) reference);
}

void sho_reference_list_set_reference(ShoReferenceList *reference_list, ShoReference *reference) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	CatStringWo *reference_id = sho_reference_get_id(reference);
	int idx;
	for(idx=0; idx<cat_array_wo_size(priv->references); idx++) {
		ShoReference *test = (ShoReference *) cat_array_wo_get(priv->references, idx);
		if (cat_string_wo_equal(sho_reference_get_id(test), reference_id)) {
			cat_array_wo_set(priv->references, (GObject *) reference, idx, NULL);
			return;
		}
	}
	cat_array_wo_append(priv->references, (GObject *) reference);
}

ShoReference *sho_reference_list_get_first_reference_by_id(ShoReferenceList *reference_list, CatStringWo *ref_id) {
	g_return_val_if_fail(ref_id!=NULL, NULL);
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	CatIIterator *iter = cat_array_wo_iterator(priv->references);
	ShoReference *result = NULL;
	while(cat_iiterator_has_next(iter)) {
		ShoReference *ref = (ShoReference *) cat_iiterator_next(iter);
		if (cat_string_wo_equal(ref_id, sho_reference_get_id(ref))) {
			result = ref;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}


ShoSimpleValue *sho_reference_list_get_simple_value(ShoReferenceList *reference_list, CatStringWo *ref_id) {
	ShoSimpleValue *result = NULL;
	ShoReference *ref = sho_reference_list_get_first_reference_by_id(reference_list, ref_id);
	if (ref) {
		ShoIValue *val = sho_reference_get_value(ref);
		if (SHO_IS_SIMPLE_VALUE(val)) {
			result = SHO_SIMPLE_VALUE(val);
		} else {
			cat_log_error("expected type ShoSimpleValue, got %s for ref_id:%s", g_type_name_from_instance((GTypeInstance *) val), cat_string_wo_getchars(ref_id));
		}
	}
	return result;
}


CatArrayWo *sho_reference_list_enlist_reference_by_id(ShoReferenceList *reference_list, CatStringWo *ref_id) {
	g_return_val_if_fail(ref_id!=NULL, NULL);
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	CatIIterator *iter = cat_array_wo_iterator(priv->references);
	CatArrayWo *result = NULL;
	while(cat_iiterator_has_next(iter)) {
		ShoReference *ref = (ShoReference *) cat_iiterator_next(iter);
		if (cat_string_wo_equal(ref_id, sho_reference_get_id(ref))) {
			if (result == NULL) {
				result = cat_array_wo_new();
			}
			cat_array_wo_append(result, (GObject *) ref);
		}
	}
	cat_unref_ptr(iter);
	return result;
}


void sho_reference_list_set_value(ShoReferenceList *reference_list, CatStringWo *ref_id, ShoIValue *new_value) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	int count = cat_array_wo_size(priv->references);
	int idx;
	for(idx=0; idx<count; idx++) {
		ShoReference *reference = (ShoReference *) cat_array_wo_get(priv->references, idx);
		CatStringWo *test_id = sho_reference_get_id(reference);
		if (cat_string_wo_equal(ref_id, test_id)) {
			sho_reference_set_value(reference, new_value);
			return;
		}
	}
	ShoReference *reference = sho_reference_new(ref_id);
	sho_reference_set_value(reference, new_value);
	sho_reference_list_add_reference(reference_list, reference);
	cat_unref_ptr(reference);
}


void sho_reference_list_set_string(ShoReferenceList *reference_list, CatStringWo *ref_id, CatStringWo *string_val) {
	ShoIValue *new_sho_value = (ShoIValue *) sho_simple_value_new_string(string_val);
	sho_reference_list_set_value(reference_list, ref_id, new_sho_value);
	cat_unref_ptr(new_sho_value);
}

void sho_reference_list_set_boolean(ShoReferenceList *reference_list, CatStringWo *ref_id, gboolean val) {
	ShoIValue *new_sho_value = (ShoIValue *) sho_simple_value_new_string(val ? CAT_S(sho_s_true) : CAT_S(sho_s_false));
	sho_reference_list_set_value(reference_list, ref_id, new_sho_value);
	cat_unref_ptr(new_sho_value);
}

gboolean sho_reference_list_get_boolean(ShoReferenceList *reference_list, CatStringWo *ref_id, gboolean default_value) {
	ShoReference *ref = sho_reference_list_get_first_reference_by_id(reference_list, ref_id);
	if (ref) {
		ShoIValue *value = sho_reference_get_value(ref);
		if (SHO_IS_SIMPLE_VALUE(value)) {
			default_value = sho_simple_value_get_boolean((ShoSimpleValue *) value, default_value);
		}
	}
	return default_value;
}


void sho_reference_list_set_int(ShoReferenceList *reference_list, CatStringWo *ref_id, int val) {
	ShoIValue *new_sho_value = (ShoIValue *) sho_simple_value_new_integer(val);
	sho_reference_list_set_value(reference_list, ref_id, new_sho_value);
	cat_unref_ptr(new_sho_value);
}

int sho_reference_list_get_int(ShoReferenceList *reference_list, CatStringWo *ref_id, int default_value) {
	ShoReference *ref = sho_reference_list_get_first_reference_by_id(reference_list, ref_id);
	if (ref) {
		ShoIValue *value = sho_reference_get_value(ref);
		if (SHO_IS_SIMPLE_VALUE(value)) {
			default_value = sho_simple_value_get_integer((ShoSimpleValue *) value, default_value);
		}
	}
	return default_value;
}

CatStringWo *sho_reference_list_get_string(ShoReferenceList *reference_list, CatStringWo *ref_id, CatStringWo *default_value) {
	ShoReference *ref = sho_reference_list_get_first_reference_by_id(reference_list, ref_id);
	if (ref) {
		ShoIValue *value = sho_reference_get_value(ref);
		if (SHO_IS_SIMPLE_VALUE(value)) {
			CatStringWo *p = sho_simple_value_get_string((ShoSimpleValue *) value);
			if (p) {
				default_value = p;
			}
		}
	}
	return default_value;
}


int sho_reference_list_count(ShoReferenceList *reference_list) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	return cat_array_wo_size(priv->references);
}

CatIIterator *sho_reference_list_iterator(ShoReferenceList *reference_list) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	return cat_array_wo_iterator(priv->references);
}


CatArrayWo *sho_reference_list_get_list(ShoReferenceList *reference_list) {
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(reference_list);
	return priv->references;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ShoReferenceList *instance = SHO_REFERENCE_LIST(self);
	ShoReferenceListPrivate *priv = sho_reference_list_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p %p]", iname, self, priv->references);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
