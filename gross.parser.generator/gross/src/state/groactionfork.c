
/*
   File:    groactionfork.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 23, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "groactionfork.h"
#include "groiparseaction.h"
#include "groactionshift.h"
#include "groactionreduce.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroActionFork"
#include <logging/catlog.h>

struct _GroActionForkPrivate {
	int fork_index;
	CatArrayWo *actions;
};

static void l_parse_action_iface_init(GroIParseActionInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroActionFork, gro_action_fork, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroActionFork)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IPARSE_ACTION, l_parse_action_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_action_fork_class_init(GroActionForkClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_action_fork_init(GroActionFork *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroActionFork *instance = GRO_ACTION_FORK(object);
//	GroActionForkPrivate *priv = gro_action_fork_get_instance_private(instance);
	G_OBJECT_CLASS(gro_action_fork_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_action_fork_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroActionFork *gro_action_fork_new(int fork_index, GroIParseAction *parse_action) {
	GroActionFork *result = g_object_new(GRO_TYPE_ACTION_FORK, NULL);
	cat_ref_anounce(result);
	GroActionForkPrivate *priv = gro_action_fork_get_instance_private(result);
	priv->fork_index = fork_index;
	priv->actions = cat_array_wo_new();
	cat_array_wo_append(priv->actions, (GObject *) parse_action);
	return result;
}

int gro_action_fork_get_index(const GroActionFork *action_fork) {
	GroActionForkPrivate *priv = gro_action_fork_get_instance_private((GroActionFork *) action_fork);
	return priv->fork_index;
}

int gro_action_fork_action_count(const GroActionFork *action_fork) {
	GroActionForkPrivate *priv = gro_action_fork_get_instance_private((GroActionFork *) action_fork);
	return cat_array_wo_size(priv->actions);
}

CatIIterator *gro_action_fork_action_iterator(GroActionFork *action_fork) {
	GroActionForkPrivate *priv = gro_action_fork_get_instance_private((GroActionFork *) action_fork);
	return cat_array_wo_iterator(priv->actions);
}


static gboolean l_shift_actions_equal(GroActionShift *act_a, GroActionShift *act_b);
static gboolean l_reduce_actions_equal(GroActionReduce *act_a, GroActionReduce *act_b);


void gro_action_fork_add_action(GroActionFork *action_fork, GroIParseAction *parse_action) {
	GroActionForkPrivate *priv = gro_action_fork_get_instance_private(action_fork);
	GroActionKind kind = gro_iparse_action_kind(parse_action);
	CatIIterator *act_iter = cat_array_wo_iterator(priv->actions);
	gboolean found = FALSE;
	while(!found && cat_iiterator_has_next(act_iter)) {
		GroIParseAction *other = (GroIParseAction *) cat_iiterator_next(act_iter);
		GroActionKind okind = gro_iparse_action_kind(other);
		if (okind==kind) {
			if (kind==GRO_ACTION_KIND_SHIFT) {
				found = l_shift_actions_equal((GroActionShift *) parse_action, (GroActionShift *) other);
			} else if (kind==GRO_ACTION_KIND_REDUCE) {
				found = l_reduce_actions_equal((GroActionReduce *) parse_action, (GroActionReduce *) other);
			} else {
				cat_log_fatal("non supported kind:%d", kind);
			}
		}
	}
	if (!found) {
		cat_array_wo_append(priv->actions, (GObject *) parse_action);
	}
}

gboolean gro_action_fork_actions_equal(GroIParseAction *parse_action_a, GroIParseAction *parse_action_b) {
	gboolean result = FALSE;
	GroActionKind kind_a = gro_iparse_action_kind(parse_action_a);
	GroActionKind kind_b = gro_iparse_action_kind(parse_action_b);
	if (kind_a==kind_b) {
		if (kind_a==GRO_ACTION_KIND_SHIFT) {
			result = l_shift_actions_equal((GroActionShift *) parse_action_a, (GroActionShift *) parse_action_b);
		} else if (kind_a==GRO_ACTION_KIND_REDUCE) {
			result = l_reduce_actions_equal((GroActionReduce *) parse_action_a, (GroActionReduce *) parse_action_b);
		}
	}
	return result;
}

static gboolean l_shift_actions_equal(GroActionShift *act_a, GroActionShift *act_b) {
	return gro_action_shift_get_shift_to(act_a)==gro_action_shift_get_shift_to(act_b);
}

static gboolean l_reduce_actions_equal(GroActionReduce *act_a, GroActionReduce *act_b) {
	return gro_action_reduce_get_production(act_a)==gro_action_reduce_get_production(act_b);
}

gint gro_action_fork_compare(const GroActionFork *a, const GroActionFork *b) {
	GroActionForkPrivate *priv_a = gro_action_fork_get_instance_private((GroActionFork *) a);
	GroActionForkPrivate *priv_b = gro_action_fork_get_instance_private((GroActionFork *) b);

	int idx_a = priv_a->fork_index;
	int idx_b = priv_b->fork_index;
	return idx_a<idx_b ? -1 : (idx_a>idx_b ? 1 : 0);
}



/********************* start GroIParseAction implementation *********************/

static GroActionKind l_parse_action_kind(GroIParseAction *action) {
	return GRO_ACTION_KIND_FORK;
}

static void l_parse_action_iface_init(GroIParseActionInterface *iface) {
	iface->kind = l_parse_action_kind;
}

/********************* end GroIParseAction implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroActionFork *instance = GRO_ACTION_FORK(self);
	GroActionForkPrivate *priv = gro_action_fork_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, fork-idx=%d]", iname, self, priv->fork_index);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
