/*
   File:    catlinkedlist.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 22, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "catlinkedlist.h"
#include "catistringable.h"
#include "woo/catstringwo.h"

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatLinkedList"
#include "logging/catlog.h"

typedef struct _CatNode CatNode;

typedef struct _CatLinkedListIter	      CatLinkedListIter;
typedef struct _CatLinkedListIterClass	  CatLinkedListIterClass;

struct _CatLinkedListIter {
	GObject parent;
	CatNode *node;
};

GType cat_linked_list_iter_get_type(void);
#define CAT_TYPE_LINKED_LIST_ITER            (cat_linked_list_iter_get_type())



struct _CatNode {
	CatNode *next;
	GObject *data;
};

typedef struct _CatChunk CatChunk;

struct _CatChunk {
	struct _CatChunk *next_chunk;
	long chunk_size;
	CatNode data;
};

struct _CatLinkedListPrivate {
	void *data_chunks;
	CatNode *available;
	CatNode *head;
	int size;
	int nodes_per_chunk;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatLinkedList, cat_linked_list, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CatLinkedList)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_linked_list_class_init(CatLinkedListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_linked_list_init(CatLinkedList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatLinkedList *instance = CAT_LINKED_LIST(object);
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(instance);
	CatNode *test_node = priv->head;
	while(test_node) {
		cat_unref_ptr(test_node->data);
		test_node = test_node->next;
	}

	CatChunk *chunk = (CatChunk *) priv->data_chunks;
	priv->data_chunks = NULL;
	while(chunk) {
		CatChunk *next_chunk = chunk->next_chunk;
		g_slice_free1(chunk->chunk_size, chunk);
		chunk = next_chunk;
	}
	G_OBJECT_CLASS(cat_linked_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_linked_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatLinkedList *cat_linked_list_new() {
	CatLinkedList *result = g_object_new(CAT_TYPE_LINKED_LIST, NULL);
	cat_ref_anounce(result);
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(result);
	priv->available = NULL;
	priv->head = NULL;
	priv->data_chunks = NULL;
	priv->nodes_per_chunk = 1024;
	priv->size = 0;
	return result;
}

int cat_linked_list_size(CatLinkedList *linked_list) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	return priv->size;
}

void l_new_chunk(CatLinkedListPrivate *priv) {
	long chunk_size = sizeof(CatNode)*priv->nodes_per_chunk + sizeof(long) + sizeof(void*);
	CatChunk *new_chunk = (CatChunk *) g_slice_alloc(chunk_size);
	new_chunk->chunk_size = chunk_size;
	new_chunk->next_chunk = priv->data_chunks;
	priv->data_chunks = new_chunk;

	CatNode *node = &(new_chunk->data);
	cat_log_trace("node=%p, new_chunk=%p", node, new_chunk);
	CatNode *last_node = priv->available;
	int idx=0;
	for(idx=priv->nodes_per_chunk-1; idx>=0; idx--) {
		CatNode *next_node = node+idx;
		next_node->next = last_node;
		last_node = next_node;
	}
	priv->available = last_node;

}


CatNode *l_allocate_node(CatLinkedListPrivate *priv) {
	CatNode *result = NULL;
	if (priv->available==NULL) {
		l_new_chunk(priv);
	}
	result = priv->available;
	priv->available = result->next;
	result->next = NULL;
	result->data = NULL;
	return result;
}

void cat_linked_list_append(CatLinkedList *linked_list, GObject *data) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	CatNode *node = l_allocate_node(priv);
	node->data = cat_ref_ptr(data);

	if (priv->head) {
		CatNode *last_node = priv->head;
		while(last_node->next) {
			last_node = last_node->next;
		}
		last_node->next = node;
	} else {
		priv->head = node;
	}
	priv->size++;
}

void cat_linked_list_prepend(CatLinkedList *linked_list, GObject *data) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	CatNode *node = l_allocate_node(priv);
	node->data = cat_ref_ptr(data);
	node->next = priv->head;
	priv->head = node;
	priv->size++;
}

gboolean cat_linked_list_remove(CatLinkedList *linked_list, GObject *data) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	CatNode *last_node = NULL;
	CatNode *test_node = priv->head;
	while(test_node) {
		if (test_node->data == data) {
			if (last_node == NULL) {
				priv->head = test_node->next;
			} else {
				last_node->next = test_node->next;
			}
			cat_unref_ptr(test_node->data);
			test_node->next = priv->available;
			priv->available = test_node;
			priv->size--;
			return TRUE;
		}
		last_node = test_node;
		test_node = test_node->next;
	}
	return FALSE;
}

gboolean cat_linked_list_remove_last(CatLinkedList *linked_list, GObject **removed) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	gboolean result = FALSE;
	if (priv->head) {
		CatNode *last_node = NULL;
		CatNode *test_node = priv->head;
		while(test_node->next) {
			last_node = test_node;
			test_node = last_node->next;
		}
		result = TRUE;
		if (removed) {
			*removed = test_node->data;
		} else {
			cat_unref_ptr(test_node->data);
		}
		if (last_node == NULL) {
			priv->head = test_node->next;
		} else {
			last_node->next = test_node->next;
		}
		test_node->next = priv->available;
		priv->available = test_node;
		priv->size--;
	}
	return result;
}

gboolean cat_linked_list_remove_first(CatLinkedList *linked_list, GObject **removed) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	gboolean result = FALSE;
	if (priv->head) {
		result = TRUE;
		CatNode *test_node = priv->head;
		if (removed) {
			*removed = test_node->data;
		} else {
			cat_unref_ptr(test_node->data);
		}
		priv->head = test_node->next;
		test_node->next = priv->available;
		priv->available = test_node;
		priv->size--;
	}
	return result;
}

void cat_linked_listed_remove_tail(CatLinkedList *linked_list, int from) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	int cnt = from;
	CatNode *last_node = NULL;
	CatNode *test_node = priv->head;
	while(test_node && cnt>0) {
		last_node = test_node;
		test_node = test_node->next;
		cnt--;
	}
	if (cnt>0 || test_node==NULL) {
		return;
	}
	if (last_node==NULL) {
		priv->head = NULL;
	} else {
		last_node->next = NULL;
	}


	last_node = test_node;
	cat_log_debug("removing:%p", last_node->data);
	cat_unref_ptr(last_node->data);
	while(last_node->next) {
		last_node = last_node->next;
		cat_log_trace("removing:%p", last_node->data);
		cat_unref_ptr(last_node->data);
	}

	if (priv->available == NULL) {
		priv->available = test_node;
	} else {
		last_node->next = priv->available;
		priv->available = last_node;
	}
	priv->size = from;
}

CatIIterator *cat_linked_list_iterator(CatLinkedList *linked_list) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	CatLinkedListIter *result = g_object_new(CAT_TYPE_LINKED_LIST_ITER, NULL);
	cat_ref_anounce(result);
	result->node = priv->head;
	return (CatIIterator *) result;
}

GObject *cat_linked_list_get(CatLinkedList *linked_list, int index) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	CatNode *test_node = priv->head;
	while(test_node && index>0) {
		test_node = test_node->next;
		index--;
	}
	if (test_node) {
		return test_node->data;
	}
	return NULL;
}


void cat_linked_list_set(CatLinkedList *linked_list, int index, GObject *replacement) {
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(linked_list);
	CatNode *test_node = priv->head;
	while(test_node && index>0) {
		test_node = test_node->next;
		index--;
	}
	if (test_node) {
		cat_ref_ptr(replacement);
		cat_unref(test_node->data);
		test_node->data = replacement;
	}
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatLinkedList *instance = CAT_LINKED_LIST(self);
	CatLinkedListPrivate *priv = cat_linked_list_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, size=%d", iname, self, priv->size);
	if (priv->size>0) {
		int idx = 0;
		CatNode *out = priv->head;
		for(idx=0; idx<priv->size && idx<25; idx++) {
			if (out) {
				cat_string_wo_format(append_to, ", %p(%d)", out->data, (G_OBJECT(out->data)->ref_count));
			} else {
				cat_string_wo_format(append_to, ", [BAD-SIZE]");
			}
			out = out->next;
		}
		if (priv->size>=5) {
			cat_string_wo_append_chars(append_to, ", ...");
		}
	}
	cat_string_wo_append_chars(append_to, "]");

}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


#define CAT_LINKED_LIST_ITER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_linked_list_iter_get_type(), CatLinkedListIter))
#define CAT_LINKED_LIST_ITER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_LINKED_LIST_ITER, CatLinkedListIterClass))
#define CAT_IS_LINKED_LIST_ITER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_LINKED_LIST_ITER))
#define CAT_IS_LINKED_LIST_ITER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_LINKED_LIST_ITER))
#define CAT_LINKED_LIST_ITER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_LINKED_LIST_ITER, CatLinkedListIterClass))

struct _CatLinkedListIterClass {
	GObjectClass parent_class;
};

static gboolean l_iter_has_next(CatIIterator *self) {
	CatLinkedListIter *iter = (CatLinkedListIter *) self;
	return iter->node!=NULL;
}

static gboolean l_iter_is_last(CatIIterator *self) {
	CatLinkedListIter *iter = (CatLinkedListIter *) self;
	return iter->node && iter->node->next==NULL;
}

static gpointer l_iter_next(CatIIterator *self) {
	CatLinkedListIter *iter = (CatLinkedListIter *) self;
	gpointer result = NULL;
	if (iter->node) {
		result = iter->node->data;
		iter->node = iter->node->next;
	}
	return result;
}


static void l_iterator_interface_init(CatIIteratorInterface *iface) {
	iface->has_next = l_iter_has_next;
	iface->next = l_iter_next;
	iface->is_last = l_iter_is_last;
}


G_DEFINE_TYPE_WITH_CODE(CatLinkedListIter, cat_linked_list_iter, G_TYPE_OBJECT, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IITERATOR, l_iterator_interface_init)
);

static void cat_linked_list_iter_class_init(CatLinkedListIterClass *clazz) {
}

static void cat_linked_list_iter_init(CatLinkedListIter *array_iter) {
}
