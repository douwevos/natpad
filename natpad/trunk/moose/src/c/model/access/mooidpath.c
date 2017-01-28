#include "mooidpath.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooIdPath"
#include <logging/catlog.h>

struct _MooIdPathPrivate {
	int count;
	long long *ids;
	int *index_cache;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooIdPath, moo_id_path, G_TYPE_INITIALLY_UNOWNED, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_id_path_class_init(MooIdPathClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooIdPathPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_id_path_init(MooIdPath *instance) {
	MooIdPathPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_ID_PATH, MooIdPathPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooIdPath *instance = MOO_ID_PATH(object);
	MooIdPathPrivate *priv = instance->priv;
	cat_free_ptr(priv->ids);
	cat_free_ptr(priv->index_cache);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooIdPath *moo_id_path_new() {
	MooIdPath *result = g_object_new(MOO_TYPE_ID_PATH, NULL);
	cat_ref_anounce(result);
	MooIdPathPrivate *priv = result->priv;
	priv->count = 0;
	priv->ids = NULL;
	priv->index_cache = NULL;
	return result;
}


MooIdPath *moo_id_path_append_id(MooIdPath *base_path, long long unique_id, int index) {
	MooIdPathPrivate *priv = MOO_ID_PATH_GET_PRIVATE(base_path);
	MooIdPath *result = moo_id_path_new();
	MooIdPathPrivate *rpriv = MOO_ID_PATH_GET_PRIVATE(result);
	if (priv->count<=0) {
		rpriv->count = 1;
		rpriv->ids = g_malloc(sizeof(long long));
		rpriv->ids[0] = unique_id;
		rpriv->index_cache = g_malloc(sizeof(int));
		rpriv->index_cache[0] = index;
	} else {
		int count = priv->count + 1;
		rpriv->count = count;
		rpriv->ids = g_malloc(sizeof(long long)*count);
		rpriv->index_cache = g_malloc(sizeof(int)*count);
		count--;
		memcpy(rpriv->ids, priv->ids, sizeof(long long)*count);
		memcpy(rpriv->index_cache, priv->index_cache, sizeof(int)*count);
		rpriv->ids[count] = unique_id;
		rpriv->index_cache[count] = index;
	}
	return result;
}

MooIdPath *moo_id_path_append_multiple_ids(MooIdPath *base_path, long long *unique_ids, int *indexes, int count2add) {
	MooIdPathPrivate *priv = MOO_ID_PATH_GET_PRIVATE(base_path);
	MooIdPath *result = moo_id_path_new();
	MooIdPathPrivate *rpriv = MOO_ID_PATH_GET_PRIVATE(result);
	int count = 0;
	if (priv->count>0) {
		count = priv->count;
	}
	count += count2add;
	rpriv->count = count;
	rpriv->ids = g_malloc(sizeof(long long)*count);
	rpriv->index_cache = g_malloc(sizeof(int)*count);

	int offset = 0;
	if (priv->count>0) {
		offset = priv->count;
		memcpy(rpriv->ids, priv->ids, sizeof(long long)*priv->count);
		memcpy(rpriv->index_cache, priv->index_cache, sizeof(int)*priv->count);
	}

	memcpy(rpriv->ids+offset, unique_ids, sizeof(long long)*count2add);
	if (indexes!=NULL) {
		memcpy(rpriv->index_cache+offset, indexes, sizeof(int)*count2add);
	} else {
		int idx;
		for(idx=0; idx<count2add; idx++) {
			rpriv->index_cache[offset+idx] = -1;
		}
	}

	return result;
}




MooNodeWo *moo_id_path_create_editable_node(MooIdPath *id_path, MooNodeWo *root_node) {
	MooIdPathPrivate *priv = MOO_ID_PATH_GET_PRIVATE(id_path);

	if (moo_node_wo_is_fixed(root_node)) {
		cat_log_error("root node is fixed:%o", root_node);
		return NULL;
	}

	int idx;
	for(idx=0; idx<priv->count; idx++) {
		long nextId = priv->ids[idx];
		int root_count = moo_node_wo_child_count(root_node);
		int cached_idx = priv->index_cache[idx];
		if (cached_idx>=0 && cached_idx<root_count) {
			MooNodeWo *childAt = moo_node_wo_child_at(root_node, cached_idx);
			if (childAt!=NULL && moo_node_wo_get_unique_id(childAt)==nextId) {
				root_node = moo_node_wo_get_editable_child_at(root_node, cached_idx);
				continue;
			}
		}
		MooNodeWo *nextNode = NULL;
		int cidx;
		for(cidx=root_count-1; cidx>=0; cidx--) {
			MooNodeWo *childAt = moo_node_wo_child_at(root_node, cidx);
			if (childAt!=NULL && moo_node_wo_get_unique_id(childAt)==nextId) {
				nextNode = moo_node_wo_get_editable_child_at(root_node, cidx);
				priv->index_cache[idx] = cidx;
				cidx = -1;
			}
		}
		if (nextNode==NULL) {
			return NULL;
		}
		root_node = nextNode;
	}
	return root_node;
}


MooNodeWo *moo_id_path_get_tail(MooIdPath *id_path, MooNodeWo *root_node) {
	MooIdPathPrivate *priv = MOO_ID_PATH_GET_PRIVATE(id_path);

	int idx;
	for(idx=0; idx<priv->count; idx++) {
		long nextId = priv->ids[idx];
		int root_count = moo_node_wo_child_count(root_node);
		int cached_idx = priv->index_cache[idx];
		if (cached_idx>=0 && cached_idx<root_count) {
			MooNodeWo *childAt = moo_node_wo_child_at(root_node, cached_idx);
			if (childAt!=NULL && moo_node_wo_get_unique_id(childAt)==nextId) {
				root_node = moo_node_wo_child_at(root_node, cached_idx);
				continue;
			}
		}
		MooNodeWo *nextNode = NULL;
		int cidx;
		for(cidx=root_count-1; cidx>=0; cidx--) {
			MooNodeWo *childAt = moo_node_wo_child_at(root_node, cidx);
			if (childAt!=NULL && moo_node_wo_get_unique_id(childAt)==nextId) {
				nextNode = moo_node_wo_child_at(root_node, cidx);
				priv->index_cache[idx] = cidx;
				cidx = -1;
			}
		}
		if (nextNode==NULL) {
			return NULL;
		}
		root_node = nextNode;
	}
	return root_node;
}


int moo_id_path_count(MooIdPath *id_path) {
	MooIdPathPrivate *priv = MOO_ID_PATH_GET_PRIVATE(id_path);
	return priv->count;
}



MooNodeWo *moo_id_path_get_at(MooIdPath *id_path, MooNodeWo *root_node, int index) {
	MooIdPathPrivate *priv = MOO_ID_PATH_GET_PRIVATE(id_path);

	if (index<0 || index>=priv->count) {
		return NULL;
	}

	int idx;
	for(idx=0; idx<=index; idx++) {
		long nextId = priv->ids[idx];
		int root_count = moo_node_wo_child_count(root_node);
		int cached_idx = priv->index_cache[idx];
		if (cached_idx>=0 && cached_idx<root_count) {
			MooNodeWo *childAt = moo_node_wo_child_at(root_node, cached_idx);
			if (childAt!=NULL && moo_node_wo_get_unique_id(childAt)==nextId) {
				root_node = moo_node_wo_child_at(root_node, cached_idx);
				continue;
			}
		}
		MooNodeWo *nextNode = NULL;
		int cidx;
		for(cidx=root_count-1; cidx>=0; cidx--) {
			MooNodeWo *childAt = moo_node_wo_child_at(root_node, cidx);
			if (childAt!=NULL && moo_node_wo_get_unique_id(childAt)==nextId) {
				nextNode = moo_node_wo_child_at(root_node, cidx);
				priv->index_cache[idx] = cidx;
				cidx = -1;
			}
		}
		if (nextNode==NULL) {
			return NULL;
		}
		root_node = nextNode;
	}
	return root_node;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooIdPathPrivate *priv = MOO_ID_PATH_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_append_chars(append_to, iname);
	cat_string_wo_append_char(append_to, '[');

	int idx;
	for(idx=0; idx<priv->count; idx++) {
		if (idx>0) {
			cat_string_wo_append_char(append_to, ',');
		}
		cat_string_wo_append_decimal(append_to, priv->ids[idx]);
		cat_string_wo_append_char(append_to, '[');
		cat_string_wo_append_decimal(append_to, priv->index_cache[idx]);
		cat_string_wo_append_char(append_to, ']');
	}
	cat_string_wo_append_char(append_to, ']');
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
