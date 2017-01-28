/*
   File:    MooContentMapWo.c
   Project: moose
   Author:  Douwe Vos
   Date:    Oct 31, 2013
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

#include "moocontentmapwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooContentMapWo"
#include <logging/catlog.h>

struct _WoInfo {
	MooContentMapWo *original;
	CatHashMapWo *e_original_map;
};

typedef struct _WoInfo WoInfo;

struct _MooContentMapWoPrivate {
	CatHashMapWo *e_map;		// <CatStringWo *, MooIContent *>
	int version;
	WoInfo *wo_info;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooContentMapWo, moo_content_map_wo, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_content_map_wo_class_init(MooContentMapWoClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooContentMapWoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_content_map_wo_init(MooContentMapWo *instance) {
	MooContentMapWoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_CONTENT_MAP_WO, MooContentMapWoPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooContentMapWo *instance = moo_content_map_wo(object);
	MooContentMapWoPrivate *priv = instance->priv;
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_unref_ptr(priv->wo_info->e_original_map);
		cat_free_ptr(priv->wo_info);
	}
	cat_unref_ptr(priv->e_map);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooContentMapWo *moo_content_map_wo_new() {
	MooContentMapWo *result = g_object_new(MOO_TYPE_CONTENT_MAP_WO, NULL);
	cat_ref_anounce(result);
	MooContentMapWoPrivate *priv = result->priv;
	priv->e_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	priv->version = 1;
	priv->wo_info = g_new0(WoInfo, 1);
	return result;
}

MooContentMapWo *moo_content_map_wo_ensure_editable(MooContentMapWo *content_map) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map);
	if (priv->wo_info==NULL) {
		MooContentMapWo *result = g_object_new(MOO_TYPE_CONTENT_MAP_WO, NULL);
		cat_ref_anounce(result);
		MooContentMapWoPrivate *rpriv = result->priv;
		rpriv->wo_info = g_new0(WoInfo, 1);
		rpriv->wo_info->original = cat_ref_ptr(content_map);
		rpriv->wo_info->e_original_map = cat_ref_ptr(priv->e_map);
		rpriv->version = priv->version+1;
		rpriv->e_map = cat_ref_ptr(priv->e_map);
		return result;
	}
	return cat_ref_ptr(content_map);
}


gboolean moo_content_map_wo_is_fixed(MooContentMapWo *content_map) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map);
	return priv->wo_info == NULL;
}

MooContentMapWo *moo_content_map_wo_anchor(MooContentMapWo *content_map, int version) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map);
	if (priv->wo_info == NULL) {
		return content_map;
	}

	if (priv->e_map != priv->wo_info->e_original_map) {
		/* anchor all MooIContent entries */
		CatArrayWo *e_keys = cat_hash_map_wo_enlist_keys(priv->e_map, NULL);
		if (e_keys!=NULL) {
			CatIIterator *iter = cat_array_wo_iterator(e_keys);
			while(cat_iiterator_has_next(iter)) {
				CatStringWo *key = (CatStringWo *) cat_iiterator_next(iter);
				cat_ref_ptr(key);
				GObject *val = cat_hash_map_wo_get(priv->e_map, (GObject *) key);
				if (MOO_IS_ICONTENT(val)) {
					MooIContent *content = (MooIContent *) cat_ref_ptr(val);
					MooIContent *new_content = moo_icontent_anchor(content, version);

					cat_log_trace("content=%o", content);
					cat_log_trace("new_content=%o", new_content);
					if (content!=new_content) {
						cat_hash_map_wo_put(priv->e_map, (GObject *) key, (GObject *) new_content);
					}
					cat_unref_ptr(new_content);
				}
				cat_unref_ptr(key);
			}
			cat_unref_ptr(iter);
			cat_unref_ptr(e_keys);
		}

		/* compare hash-map entries */
		if (!cat_hash_map_wo_equal(priv->e_map, priv->wo_info->e_original_map, NULL)) {
			/* return the new version */
			priv->version = version;
			cat_unref_ptr(priv->wo_info->e_original_map);
			cat_unref_ptr(priv->wo_info->original);
			cat_free_ptr(priv->wo_info);
			return content_map;
		}
	}

	/* return original */
	MooContentMapWo *result = priv->wo_info->original;
	priv->wo_info->original = NULL;
	cat_unref_ptr(content_map);
	return result;
}

gboolean moo_content_map_wo_equal(MooContentMapWo *content_map_a, MooContentMapWo *content_map_b) {
	if (content_map_a==content_map_b) {
		return TRUE;
	}
	if (content_map_a==NULL || content_map_b==NULL) {
		return FALSE;
	}

	MooContentMapWoPrivate *priv_a = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map_a);
	MooContentMapWoPrivate *priv_b = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map_b);
	if (priv_a->e_map==priv_b->e_map) {
		return TRUE;
	}
	if (cat_hash_map_wo_size(priv_a->e_map)!=cat_hash_map_wo_size(priv_b->e_map)) {
		return FALSE;
	}
	gboolean result = TRUE;
	CatArrayWo *e_keys_a = cat_hash_map_wo_enlist_keys(priv_a->e_map, NULL);
	CatIIterator *iter = cat_array_wo_iterator(e_keys_a);
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *key_a = (CatStringWo *) cat_iiterator_next(iter);
		GObject *val_a = cat_hash_map_wo_get(priv_a->e_map, key_a);
		GObject *val_b = cat_hash_map_wo_get(priv_b->e_map, key_a);
		if (val_a==val_b) {
			continue;
		}
		if (MOO_IS_ICONTENT(val_a) && MOO_IS_ICONTENT(val_b)) {
			MooIContent *content_a = (MooIContent *) val_a;
			MooIContent *content_b = (MooIContent *) val_b;
			if (!moo_icontent_equal(content_a, content_b)) {
				result = FALSE;
				break;
			}
		} else if (CAT_IS_STRING_WO(val_a) && CAT_IS_STRING_WO(val_b)) {
			CatStringWo *txt_a = (CatStringWo *) val_a;
			CatStringWo *txt_b = (CatStringWo *) val_b;
			if (!cat_string_wo_equal(txt_a, txt_b)) {
				result = FALSE;
				break;
			}
		} else {
			result = FALSE;
			break;
		}
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(e_keys_a);

	return result;
}


#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", node); \
			return rval; \
		} \


#define CHECK_IF_WRITABLE_LIST(nlist, rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", nlist); \
			return rval; \
		} \
		if (priv->wo_info->e_original_map == priv->e_map) { \
			cat_unref_ptr(priv->e_map); \
			priv->e_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal); \
			if (priv->wo_info->e_original_map) { \
				cat_hash_map_wo_put_all(priv->e_map, priv->wo_info->e_original_map); \
			} \
		} \


void moo_content_map_wo_set(MooContentMapWo *content_map, MooIContent *content) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map);
	CHECK_IF_WRITABLE_LIST(content_map, )
	CatStringWo *key = moo_icontent_get_key(content);
	cat_hash_map_wo_put(priv->e_map, (GObject*) key, (GObject *) content);
}

void moo_content_map_wo_set_with_key(MooContentMapWo *content_map, MooIContent *content, CatStringWo *key) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map);
	CHECK_IF_WRITABLE_LIST(content_map, )
	cat_hash_map_wo_put(priv->e_map, (GObject*) key, (GObject *) content);
}

void moo_content_map_wo_create_link(MooContentMapWo *content_map, CatStringWo *key, CatStringWo *link_to) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map);
	CHECK_IF_WRITABLE_LIST(content_map, )
	cat_hash_map_wo_put(priv->e_map, (GObject*) key, (GObject *) link_to);
}


MooIContent *moo_content_map_wo_get(MooContentMapWo *content_map, CatStringWo *key) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(content_map);
	while(TRUE) {
		GObject *val = cat_hash_map_wo_get(priv->e_map, key);
		if (MOO_IS_ICONTENT(val)) {
			return (MooIContent *) (val);
		} else if (CAT_IS_STRING_WO(val)) {
			key = (CatStringWo *) val;
		} else {
			if (val!=NULL) {
				cat_log_error("invalid content: for key=%o", key);
			}
			break;
		}
	}
	return NULL;
}


GObject *moo_content_map_wo_get_raw(MooContentMapWo *self, CatStringWo *key) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(self);
	return cat_hash_map_wo_get(priv->e_map, key);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooContentMapWoPrivate *priv = MOO_CONTENT_MAP_WO_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_append_chars(append_to, iname);
	cat_string_wo_append_chars(append_to, "[version=");
	cat_string_wo_append_decimal(append_to, priv->version);
	cat_string_wo_append_char(append_to, ',');

	if (priv->e_map==NULL) {
		cat_string_wo_append_chars(append_to, "no-content-map");
	} else {
		CatArrayWo *e_key_list = cat_hash_map_wo_enlist_keys(priv->e_map, NULL);
		if (e_key_list!=NULL) {
			CatIIterator *iter = cat_array_wo_iterator(e_key_list);
			while(cat_iiterator_has_next(iter)) {
				GObject *key = cat_iiterator_next(iter);
				GObject *content = cat_hash_map_wo_get(priv->e_map, key);
				cat_string_wo_format(append_to, "%o=%o", key, content);
				if (!cat_iiterator_is_last(iter)) {
					cat_string_wo_append_char(append_to, ',');
				}
			}
			cat_unref_ptr(iter);
			cat_unref_ptr(e_key_list);
		}
	}
	cat_string_wo_append_char(append_to, ']');
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
