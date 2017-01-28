
/*
   File:    groglrparsercontext.c
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 24, 2016
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

#include "groglrparsercontext.h"

struct _GroGlrParserContextPrivate {
	GroIScanner *scanner;
	GPtrArray *fork_entries;
	gboolean parsing_done;
};


G_DEFINE_TYPE_WITH_CODE(GroGlrParserContext, gro_glr_parser_context, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroGlrParserContext)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_glr_parser_context_class_init(GroGlrParserContextClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_glr_parser_context_init(GroGlrParserContext *instance) {
}

static void l_dispose(GObject *object) {
	gro_log("dispose:%p", object);
//	GroGlrParserContext *instance = GRO_GLR_PARSER_CONTEXT(object);
//	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(instance);
	G_OBJECT_CLASS(gro_glr_parser_context_parent_class)->dispose(object);
	gro_log("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	gro_log("finalize:%p", object);
	G_OBJECT_CLASS(gro_glr_parser_context_parent_class)->finalize(object);
	gro_log("finalized:%p", object);
}


GroGlrParserContext *gro_glr_parser_context_new(GroIScanner *scanner) {
	GroGlrParserContext *result = g_object_new(GRO_TYPE_GLR_PARSER_CONTEXT, NULL);
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(result);
	priv->scanner = gro_ref_ptr(scanner);
	priv->fork_entries = g_ptr_array_new();
	priv->parsing_done = FALSE;
	return result;
}

gboolean gro_glr_parser_context_parsing_done(GroGlrParserContext *context) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	return priv->parsing_done || (priv->fork_entries->len==0);
}

void gro_glr_parser_context_set_parsing_done(GroGlrParserContext *context, gboolean new_val) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	priv->parsing_done = new_val;
}


GroForkEntry *gro_glr_parser_context_get_best_parse_result(GroGlrParserContext *context, gboolean allow_incomplete_parsing) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	gro_log("FORK_ENTRY:go");
	if (!allow_incomplete_parsing && !priv->parsing_done) {
		return NULL;
	}
	GroForkEntry *result = NULL;
	int best_error_rec_cnt = 0;
	GroForkEntryState best_state = GRO_FORK_ENTRY_ERROR_SYNCING;
	if (priv->fork_entries->len>0) {
		int idx;
		for(idx=0; idx<priv->fork_entries->len; idx++) {
			GroForkEntry *t = (GroForkEntry *) g_ptr_array_index(priv->fork_entries, idx);
			gro_log("FORK_ENTRY[%d], status=%d, err_cnt=%d", idx, gro_fork_entry_get_status(t), gro_fork_entry_get_error_recovery_count(t));
			gboolean is_better = result==NULL;
			if (!is_better) {
				GroForkEntryState status = gro_fork_entry_get_status(t);
				if (status!=best_state) {
					if (best_state==GRO_FORK_ENTRY_ACCEPTED) {
						continue;
					}
					is_better = TRUE;
				}


			}
			if (!is_better) {
				int t_ec = gro_fork_entry_get_error_recovery_count(t);
				is_better =  t_ec<best_error_rec_cnt;
			}


			if (is_better) {
				result = t;
				best_error_rec_cnt = gro_fork_entry_get_error_recovery_count(t);
				best_state = gro_fork_entry_get_status(t);
			}
		}
	}

	return result;
}

void gro_glr_parser_context_init_state(GroGlrParserContext *context, int start_state) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);

	GroStateAndSymbol *init_sas = gro_state_and_symbol_new(NULL, start_state, 0, TRUE);
	GroContent *content = gro_content_new(NULL, NULL, 0,0,0,0);
	GroForkEntry *fork_entry = gro_fork_entry_new(init_sas, content);
	g_ptr_array_add(priv->fork_entries, fork_entry);
}

int gro_glr_parser_context_fork_entry_count(GroGlrParserContext *context) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	return priv->fork_entries->len;
}

GroForkEntry *gro_glr_parser_context_fork_entry_at(GroGlrParserContext *context, int index) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	return (GroForkEntry *) g_ptr_array_index(priv->fork_entries, index);
}


void gro_glr_parser_context_insert_fork_entry(GroGlrParserContext *context, GroForkEntry *fe, int index) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	g_ptr_array_insert(priv->fork_entries, index, fe);
}

void gro_glr_parser_context_remove_fork_entry(GroGlrParserContext *context, int index) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	gro_unref_ptr(g_ptr_array_index(priv->fork_entries, index));
	g_ptr_array_remove_index(priv->fork_entries, index);

}


GroToken *gro_glr_parser_context_scan_next(GroGlrParserContext *context) {
	GroGlrParserContextPrivate *priv = gro_glr_parser_context_get_instance_private(context);
	GroToken *token = gro_iscanner_next_token(priv->scanner);
	return token;
}

