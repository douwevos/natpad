/*
   File:    groruncontext.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
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

#include "groruncontext.h"
#include "full/grorunfulltoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunContext"
#include <logging/catlog.h>

struct _GroRunContextPrivate {
	GroRunITokenFactory *token_factory;
	GroRunModel *state_model;
	GroRunIActionHandler *action_handler;
	CatArrayWo *running_forks;
	CatArrayWo *accepted_forks;
	CatArrayWo *error_forks;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunContext, grorun_context, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunContext)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_context_class_init(GroRunContextClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_context_init(GroRunContext *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunContext *instance = GRORUN_CONTEXT(object);
	GroRunContextPrivate *priv = grorun_context_get_instance_private(instance);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->state_model);
	cat_unref_ptr(priv->action_handler);
	cat_unref_ptr(priv->running_forks);
	cat_unref_ptr(priv->accepted_forks);
	cat_unref_ptr(priv->error_forks);
	G_OBJECT_CLASS(grorun_context_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_context_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunContext *grorun_context_new(GroRunITokenFactory *token_factory, GroRunModel *state_model, GroRunIActionHandler *action_handler) {
	GroRunContext *result = g_object_new(GRORUN_TYPE_CONTEXT, NULL);
	cat_ref_anounce(result);
	GroRunContextPrivate *priv = grorun_context_get_instance_private(result);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->state_model = cat_ref_ptr(state_model);
	priv->action_handler = cat_ref_ptr(action_handler);
	priv->running_forks = cat_array_wo_new();
	priv->accepted_forks = cat_array_wo_new();
	priv->error_forks = cat_array_wo_new();
	return result;
}

void grorun_context_create_start_fork(GroRunContext *context, GroRunIToken *first_lah) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	cat_array_wo_clear(priv->running_forks);
	cat_array_wo_clear(priv->accepted_forks);

	GroRunFork *fork = grorun_fork_new();
	GroRunState *start_state = grorun_model_get_start_state(priv->state_model);
	grorun_fork_push(fork, start_state, first_lah);
	cat_array_wo_append(priv->running_forks, (GObject *) fork);
	cat_unref_ptr(fork);
}

GroRunSymbol *grorun_context_get_error_symbol(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return grorun_model_get_error_terminal(priv->state_model);
}

GroRunSymbol *grorun_context_get_eof_symbol(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return grorun_model_get_eof_terminal(priv->state_model);
}

GroRunIToken *grorun_context_create_error_toklen(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GroRunSymbol *error_sym = grorun_model_get_error_terminal(priv->state_model);
	GroRunLeftRight lr = {-1,-1};
	return grorun_itoken_factory_create_token(priv->token_factory, lr, error_sym, NULL, NULL, NULL);
}


void grorun_context_fork(GroRunContext *context, int fork_idx, GroRunTransition *pending_transition) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GroRunFork *orig_fork = (GroRunFork *) cat_array_wo_get(priv->running_forks, fork_idx);

	GroRunFork *new_fork = grorun_fork_duplicate(orig_fork, 0);
	grorun_fork_set_pending_transition(new_fork, pending_transition);
	cat_array_wo_insert(priv->running_forks, (GObject *) new_fork, fork_idx+1);
	cat_unref(new_fork);
}

void grorun_context_add_fork(GroRunContext *context, GroRunFork *fork) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	cat_array_wo_append(priv->running_forks, (GObject *) fork);
}


int grorun_context_fork_count(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return cat_array_wo_size(priv->running_forks);
}

GroRunFork *grorun_context_fork_at(GroRunContext *context, int fork_idx) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return (GroRunFork *) cat_array_wo_get(priv->running_forks, fork_idx);
}


void grorun_context_error_fork(GroRunContext *context, int fork_idx, GroRunTransition *pending_transition) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GroRunFork *orig_fork = (GroRunFork *) cat_array_wo_get(priv->error_forks, fork_idx);

	GroRunFork *new_fork = grorun_fork_duplicate(orig_fork, 0);
	grorun_fork_set_pending_transition(new_fork, pending_transition);
	grorun_fork_set_postponed(new_fork, NULL);
	cat_array_wo_insert(priv->error_forks, (GObject *) new_fork, fork_idx+1);
	cat_unref(new_fork);
}


int grorun_context_error_fork_count(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return cat_array_wo_size(priv->error_forks);
}

GroRunFork *grorun_context_error_fork_at(GroRunContext *context, int fork_idx) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return (GroRunFork *) cat_array_wo_get(priv->error_forks, fork_idx);
}


GroRunLocation *grorun_context_combine_locations(GroRunFork *fork, int rhs_count, GroRunLeftRight *lr) {
	GroRunLocation *result = NULL;
//	if (calculateAdditionalInformation) {
		GroRunStackEntry *left_entry = grorun_fork_get_entry(fork, rhs_count);
		GroRunStackEntry *right_entry = grorun_fork_get_entry(fork, 1);
		int begin_column;
		long begin_row;
		int end_column;
		long end_row;

		GroRunIToken *left_token = grorun_stack_entry_get_lookahead(left_entry);
		if (left_token) {
			GroRunLeftRight left_lr = grorun_itoken_get_indexes(left_token);
			lr->left = left_lr.left;
			GroRunLocation *left_loc = grorun_itoken_get_location(left_token);
			if (left_loc) {
				grorun_location_get_begin(left_loc, &begin_column, &begin_row);
			}
		}

		GroRunIToken *right_token = grorun_stack_entry_get_lookahead(right_entry);
		if (right_token) {
			GroRunLeftRight right_lr = grorun_itoken_get_indexes(right_token);
			lr->right= right_lr.right;
			GroRunLocation *right_loc = grorun_itoken_get_location(right_token);
			if (right_loc) {
				grorun_location_get_end(right_loc, &end_column, &end_row);
			}
		}


		result = grorun_location_new(begin_column, begin_row, end_column, end_row);
//	}
	return result;
}


GroRunIToken *grorun_context_reduce(GroRunContext *context, GroRunFork *fork, GroRunReduceLayout *reduce_layout, int action_id) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GroRunLeftRight lr;
	GroRunLocation *location = grorun_context_combine_locations(fork, grorun_reduce_layout_get_stack_rhs_count(reduce_layout), &lr);
	GObject *value = NULL;
	if (priv->action_handler!=NULL) {
		value = grorun_iaction_handler_run_action(priv->action_handler, fork, reduce_layout, action_id);
	}
	GroRunIToken *result = grorun_itoken_factory_create_token(priv->token_factory, lr, grorun_reduce_layout_get_lhs(reduce_layout), value, location, NULL);
	cat_unref_ptr(value);
	cat_unref_ptr(location);
	return result;
}

GroRunIToken *grorun_context_create_for_pushback(GroRunContext *context, const GroRunLeftRight lr, GroRunLocation *location, const CatStringWo *terminal_name) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GroRunSymbol *pb_sym = grorun_isymbol_provider_get_symbol_by_name(priv->state_model, terminal_name);
	GroRunIToken *result = grorun_itoken_factory_create_token(priv->token_factory, lr, pb_sym, NULL, location, NULL);
	return result;
}



void grorun_context_error_forks_clear(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	cat_array_wo_clear(priv->error_forks);
}


void grorun_context_mark_fork_as_error(GroRunContext *context, int fork_idx) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GObject *rem = NULL;
	cat_array_wo_remove(priv->running_forks, fork_idx, &rem);
	cat_array_wo_append(priv->error_forks, rem);
	cat_unref_ptr(rem);
}

void grorun_context_error_fork_remove(GroRunContext *context, int fork_idx) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	cat_array_wo_remove(priv->error_forks, fork_idx, NULL);
}


void grorun_context_mark_fork_as_accepted(GroRunContext *context, int fork_idx) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GObject *remd = NULL;
	cat_array_wo_remove(priv->running_forks, fork_idx, &remd);
	cat_array_wo_append(priv->accepted_forks, remd);
	cat_unref_ptr(remd);
}

GObject *grorun_context_get_first_accepted(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	GObject *result = NULL;
	if (cat_array_wo_size(priv->accepted_forks)>0) {
		GroRunFork *first_fork = (GroRunFork *) cat_array_wo_get_first(priv->accepted_forks);
		cat_log_debug("first_fork=%O", first_fork);
		GroRunStackEntry *se = grorun_fork_get_top_entry(first_fork);
		se = (se==NULL ? NULL : grorun_stack_entry_get_parent(se));
		GroRunIToken *lah = (se==NULL ? NULL : grorun_stack_entry_get_lookahead(se));
		result = (lah==NULL ? NULL : grorun_itoken_get_value(lah));
	}
	return result;
}

GroRunFork *grorun_context_accepted_fork_first(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return (GroRunFork *) cat_array_wo_get_first(priv->accepted_forks);
}


gboolean grorun_context_fork_has_accepted(GroRunContext *context) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	return cat_array_wo_size(priv->accepted_forks)>0;
}


void grorun_context_add_accepted(GroRunContext *context, GroRunFork *fork) {
	GroRunContextPrivate *priv = grorun_context_get_instance_private(context);
	cat_array_wo_append(priv->accepted_forks, (GObject *) fork);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunContext *instance = GRORUN_CONTEXT(self);
	GroRunContextPrivate *priv = grorun_context_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p state_model=%O]", iname, self, priv->state_model);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
