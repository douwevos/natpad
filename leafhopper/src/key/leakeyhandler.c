/*
   File:    leakeyhandler.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 12, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "leakeyhandler.h"
#include "map/leakeynode.h"
#include "map/leakeynodecontext.h"
#include "map/leakeynodekey.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaKeyHandler"
#include <logging/catlog.h>


struct _LeaKeyHandlerPrivate {
	GType active_panel_type;
	LeaKeyNode *binding_root;
	LeaKeyNode *running_sequence;
};

G_DEFINE_TYPE (LeaKeyHandler, lea_key_handler, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_key_handler_class_init(LeaKeyHandlerClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaKeyHandlerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_key_handler_init(LeaKeyHandler *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaKeyHandlerPrivate *priv = LEA_KEY_HANDLER_GET_PRIVATE(object);
	cat_unref_ptr(priv->binding_root);
	cat_log_detail("end-dispose:%p", object);
}


static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_key_handler_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

LeaKeyHandler *lea_key_handler_new() {
	LeaKeyHandler *result = g_object_new(LEA_TYPE_KEY_HANDLER, NULL);
	cat_ref_anounce(result);
	LeaKeyHandlerPrivate *priv = LEA_KEY_HANDLER_GET_PRIVATE(result);
	priv->active_panel_type = 0;
	priv->binding_root = lea_key_node_new();
	priv->running_sequence = NULL;
	return result;
}

static gboolean l_attempt_in_sequence(LeaKeyHandlerPrivate *priv, LeaKeyNode *key_in_node, LeaKeyNode *key_in_context) {
	gboolean result = FALSE;
	if (priv->running_sequence) {
		LeaKeyNode *next_in_sequence = lea_key_node_get(priv->running_sequence, key_in_node);
		cat_log_debug("next_in_sequence=%o, key_in_node=%o", next_in_sequence, key_in_node);
		priv->running_sequence = next_in_sequence;
		if (next_in_sequence) {
			LeaKeyNode *act_context = lea_key_node_get(next_in_sequence, key_in_context);
			cat_log_debug("act_context=%o", act_context);
			if (act_context) {
				LeaIKeyBinding *binding = lea_key_node_get_key_binding(act_context);
				cat_log_debug("binding=%o", binding);
				if (binding) {
					lea_ikey_binding_activate(binding);
					result = TRUE;
					priv->running_sequence = NULL;
				}
			}

			if (!result) {
				LeaIKeyBinding *binding = lea_key_node_get_key_binding(next_in_sequence);
				cat_log_debug("binding=%o", binding);
				if (binding) {
					lea_ikey_binding_activate(binding);
					result = TRUE;
					priv->running_sequence = NULL;
				}
			}
			if (!lea_key_node_has_children(next_in_sequence)) {
				priv->running_sequence = NULL;
			}
		}
	}
	cat_log_debug("running_sequence=%o, result=%d", priv->running_sequence, result);
	return result;
}

gboolean lea_key_handler_run(LeaKeyHandler *handler, LeaKey *key, LeaKeyContext *context) {
	gboolean result = FALSE;
	LeaKeyHandlerPrivate *priv = LEA_KEY_HANDLER_GET_PRIVATE(handler);

	LeaKeyNode *key_in_node = (LeaKeyNode *) lea_key_node_key_new(key);
	LeaKeyNode *key_in_context = (LeaKeyNode *) lea_key_node_context_new(context);

	cat_log_debug("key_in_node=%o", key_in_node);

	/* first try running sequence */
	result = l_attempt_in_sequence(priv, key_in_node, key_in_context);

	/* Next try new sequence  */
	if (priv->running_sequence==NULL && !result) {
		priv->running_sequence = priv->binding_root;
		result = l_attempt_in_sequence(priv, key_in_node, key_in_context);
	}
	cat_unref_ptr(key_in_node);
	cat_unref_ptr(key_in_context);
	return result;
}

void lea_key_handler_add_binding(LeaKeyHandler *handler, LeaIKeyBinding *key_binding) {
	cat_log_debug("add:key_binding=%o", key_binding);
	LeaKeyHandlerPrivate *priv = LEA_KEY_HANDLER_GET_PRIVATE(handler);
	LeaKeyNode *key_node = priv->binding_root;

	LeaKeySequence *sequence = lea_ikey_binding_get_key_sequence(key_binding);
	if (sequence==NULL) {
		return;
	}

	cat_log_detail("sequence=%o", sequence);

	CatIIterator *iter = lea_key_sequence_iterator(sequence);
	while(cat_iiterator_has_next(iter)) {
		LeaKey *key = (LeaKey *) cat_iiterator_next(iter);
		cat_log_trace("key=%o", key);
		LeaKeyNodeKey *lknk = lea_key_node_key_new(key);
		key_node = lea_key_node_add(key_node, (LeaKeyNode *) lknk);
		cat_unref_ptr(lknk);
	}
	cat_unref_ptr(iter);


	LeaKeyContext *key_context = lea_ikey_binding_get_key_context(key_binding);
	cat_log_detail("key_context=%o", key_context);
	if (key_context) {
		LeaKeyNodeContext *lknk = lea_key_node_context_new(key_context);
		key_node = lea_key_node_add(key_node, (LeaKeyNode *) lknk);
		cat_unref_ptr(lknk);
	}

	lea_key_node_set_key_binding(key_node, key_binding);
}

void lea_key_handler_remove_binding(LeaKeyHandler *handler, LeaIKeyBinding *key_binding) {
	cat_log_debug("remove:key_binding=%o", key_binding);
	LeaKeyHandlerPrivate *priv = LEA_KEY_HANDLER_GET_PRIVATE(handler);
	LeaKeyNode *key_node = priv->binding_root;
	CatArrayWo *e_enlisted = cat_array_wo_new();
	cat_array_wo_append(e_enlisted, (GObject *) key_node);

	LeaKeySequence *sequence = lea_ikey_binding_get_key_sequence(key_binding);
	if (sequence==NULL) {
		return;
	}

	CatIIterator *iter = lea_key_sequence_iterator(sequence);
	while((key_node!=NULL) && cat_iiterator_has_next(iter)) {
		LeaKey *key = (LeaKey *) cat_iiterator_next(iter);
		LeaKeyNodeKey *lknk = lea_key_node_key_new(key);
		key_node = lea_key_node_get(key_node, (LeaKeyNode *) lknk);
		cat_unref_ptr(lknk);
		cat_array_wo_append(e_enlisted, (GObject *) key_node);
	}
	cat_unref_ptr(iter);

	LeaKeyContext *key_context = lea_ikey_binding_get_key_context(key_binding);
	if ((key_node!=NULL) && (key_context!=NULL)) {
		LeaKeyNodeContext *lknk = lea_key_node_context_new(key_context);
		key_node = lea_key_node_get(key_node, (LeaKeyNode *) lknk);
		cat_unref_ptr(lknk);
		cat_array_wo_append(e_enlisted, (GObject *) key_node);
	}


	if (key_node!=NULL) {
		lea_key_node_set_key_binding(key_node, NULL);
		int idx;
		LeaKeyNode *child = key_node;
		LeaKeyNode *dad = key_node;
		for(idx=cat_array_wo_size(e_enlisted)-2; idx>=0; idx--) {
			child = dad;
			dad = (LeaKeyNode *) cat_array_wo_get(e_enlisted, idx);
			if (lea_key_node_is_shallow(child)) {
				lea_key_node_remove(dad, child);
			} else {
				break;
			}
		}
	}
	cat_unref_ptr(e_enlisted);
}

void lea_key_handler_update(LeaKeyHandler *handler, LeaActionGroup *action_group) {
	cat_log_debug("handler-update:start");
	LeaKeyHandlerPrivate *priv = LEA_KEY_HANDLER_GET_PRIVATE(handler);
	CatArrayWo *e_enlisted_bindings = cat_array_wo_new();
	lea_action_group_enlist_recursive(action_group, e_enlisted_bindings, TRUE);

	CatArrayWo *e_work = cat_array_wo_new();
	cat_array_wo_append(e_work, (GObject *) priv->binding_root);
	while(cat_array_wo_size(e_work)>0) {
		LeaKeyNode *work_key_node = NULL;
		cat_array_wo_remove_last(e_work, (GObject **) &work_key_node);
		cat_log_debug("work_key_node=%o", work_key_node);

		if (lea_key_node_has_children(work_key_node)) {
			lea_key_node_enlist_children(work_key_node, e_work);
		} else {
			LeaIKeyBinding *binding = lea_key_node_get_key_binding(work_key_node);
			if (LEA_IS_ACTION(binding)) {
				int index = cat_array_wo_find_index(e_enlisted_bindings, binding,  -1);
				if (index>=0) {
					cat_array_wo_remove(e_enlisted_bindings, index, NULL);
				} else {
					lea_key_handler_remove_binding(handler, binding);
				}
			}
		}
		cat_unref_ptr(work_key_node);
	}

	if (cat_array_wo_size(e_enlisted_bindings)>0) {
		CatIIterator *iter = cat_array_wo_iterator(e_enlisted_bindings);
		while(cat_iiterator_has_next(iter)) {
			LeaIKeyBinding *key_binding = (LeaIKeyBinding *) cat_iiterator_next(iter);
			lea_key_handler_add_binding(handler, key_binding);
		}
		cat_unref_ptr(iter);
	}
	cat_unref_ptr(e_work);
	cat_unref_ptr(e_enlisted_bindings);
	cat_log_debug("handler-update:end");
}


