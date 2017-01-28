/*
   File:    grorunmodel.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 10, 2016
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

#include "grorunmodel.h"
#include "grorunisymbolprovider.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunModel"
#include <logging/catlog.h>

struct _GroRunModelPrivate {
	CatArrayWo *symbols;
	CatArrayWo *states;
	GroRunSymbol *error_terminal;
	GroRunSymbol *eof_terminal;
};

static void l_symbol_provider_iface_init(GroRunISymbolProviderInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunModel, grorun_model, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunModel)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISYMBOL_PROVIDER, l_symbol_provider_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_model_class_init(GroRunModelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_model_init(GroRunModel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunModel *instance = GRORUN_MODEL(object);
	GroRunModelPrivate *priv = grorun_model_get_instance_private(instance);
	cat_unref_ptr(priv->symbols);
	cat_unref_ptr(priv->states);
	G_OBJECT_CLASS(grorun_model_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_model_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_extract_configuration(GroRunModel *model, const char *states_text);

GroRunModel *grorun_model_new(const GroRunSymbolDescription *symbol_desc, int symbol_desc_count, const char *states_text, int states_count) {
	GroRunModel *result = g_object_new(GRORUN_TYPE_MODEL, NULL);
	cat_ref_anounce(result);
	GroRunModelPrivate *priv = grorun_model_get_instance_private(result);
	priv->symbols = cat_array_wo_new();
	int idx;
	for(idx=0; idx<symbol_desc_count; idx++) {
		const GroRunSymbolDescription *symdesc = symbol_desc+idx;
		if (symdesc!=NULL) {
			CatStringWo *name = cat_string_wo_new_with(symdesc->name);
			GroRunSymbol *nsym = grorun_symbol_new(symdesc->is_non_terminal, name, symdesc->id);
			cat_unref_ptr(name);
			cat_log_detail("sym-name=%s, %p", symdesc->name, nsym);
			cat_array_wo_append(priv->symbols, (GObject *) nsym);

			if (strcmp(symdesc->name, "⊗")==0) {
				priv->error_terminal = nsym;
			} else if (strcmp(symdesc->name, "⟧")==0) {
				priv->eof_terminal = nsym;
			}
			cat_unref_ptr(nsym);
		} else {
			cat_array_wo_append(priv->symbols, NULL);
		}
	}


	cat_log_detail("nr of symbols=%d", cat_array_wo_size(priv->symbols));

	priv->states = cat_array_wo_new();
	for(idx=0; idx<states_count; idx++) {
		GroRunState *nst = grorun_state_new(idx);
		cat_array_wo_append(priv->states, (GObject *) nst);
		cat_unref_ptr(nst);
	}

	l_extract_configuration(result, states_text);
	return result;
}

GroRunState *grorun_model_get_start_state(GroRunModel *model) {
	GroRunModelPrivate *priv = grorun_model_get_instance_private(model);
	return (GroRunState *) cat_array_wo_get_first(priv->states);
}

GroRunSymbol *grorun_model_get_error_terminal(GroRunModel *model) {
	GroRunModelPrivate *priv = grorun_model_get_instance_private(model);
	return priv->error_terminal;
}

GroRunSymbol *grorun_model_get_eof_terminal(GroRunModel *model) {
	GroRunModelPrivate *priv = grorun_model_get_instance_private(model);
	return priv->eof_terminal;
}

static char l_next_ch(const char **text) {
	if (*text==0) {
		return 0;
	}
	char result = **text;
	*text = (*text)+1;
	return result;
}

static int l_next_number(const char **text) {
	gboolean do_invert = FALSE;
	const char *off = *text;
	int result = 0;
	while(TRUE) {
		char ch = *off;
		if (ch>='0' && ch<='9') {
			result = result*10 + (ch-'0');
		} else if (ch=='-') {
			do_invert = TRUE;
		} else {
			break;
		}
		off++;
	}
	*text = off;
	return do_invert ? -result : result;
}


static void l_extract_configuration(GroRunModel *model, const char *states_text) {
	GroRunModelPrivate *priv = grorun_model_get_instance_private(model);
	CatArrayWo *reduce_layouts = cat_array_wo_new();
	GroRunState *state = NULL;
	GroRunSymbol *for_symbol = NULL;
	gboolean not_done = TRUE;
	while (not_done) {
		char ch = l_next_ch(&states_text);
		cat_log_trace("ch=%c", ch);
		switch (ch) {
			case 'r': {
				/* reduce-layout : r<lhs-sym-id>,<rhs-count>[,<nullified...]  */
				int sym_index = l_next_number(&states_text);
				GroRunSymbol *lhs= (GroRunSymbol *) cat_array_wo_get(priv->symbols, sym_index);
				ch = l_next_ch(&states_text);
				int rhs_count = l_next_number(&states_text);
				CatIntArrayWo *nullified = NULL;
				ch = l_next_ch(&states_text);
				if (ch==',') {
					nullified = cat_int_array_wo_new();
					while(ch==',') {
						int off = l_next_number(&states_text);
						cat_int_array_wo_append(nullified, off);
						ch = l_next_ch(&states_text);
					}
				}
				int production_id = -1;
				if (ch=='p') {
					production_id = l_next_number(&states_text);
				}

				GroRunReduceLayout *red_layout = grorun_reduce_layout_new(lhs, rhs_count, nullified, production_id);
				cat_log_detail("reduce-rule:%d, layout=%O, sym_index=%d, prod_id=%d", cat_array_wo_size(reduce_layouts), red_layout, sym_index, production_id);
				cat_unref_ptr(nullified);
				cat_array_wo_append(reduce_layouts, (GObject *) red_layout);
				cat_unref_ptr(red_layout);
			} break;
			case 'A': {
				/* next number is a state-index */
				int state_index = l_next_number(&states_text);
				state = (GroRunState *) cat_array_wo_get(priv->states, state_index);
				cat_log_detail("state-index:%d, state=%p", state_index, state);
				if (state==NULL) {
					cat_log_error("incorrect state-index:%d", state_index);
					return;
				}
			} break;
			case 'B': {
				/* next number is a symbol-index */
				int sym_index = l_next_number(&states_text);
				for_symbol = (GroRunSymbol *) cat_array_wo_get(priv->symbols, sym_index);
//				cat_log_detail("sym-index:%d, sym=%p", sym_index, for_symbol);
				if (for_symbol==NULL) {
					cat_log_error("incorrect sym-index:%d", sym_index);
					return;
				}
			} break;
			case 'S': {
				/* add shift */
				int state_index = l_next_number(&states_text);
				GroRunState *shift_to_state = (GroRunState *) cat_array_wo_get(priv->states, state_index);
//				cat_log_detail("shift :state-index:%d, state=%p", state_index, shift_to_state);
				grorun_state_add_shift(state, for_symbol, shift_to_state);
			} break;
			case 'R': {
				/* add reduce */
				int red_layout_idx = l_next_number(&states_text);
				l_next_ch(&states_text);
				int action_id = l_next_number(&states_text);
//				cat_log_detail("red_layout_idx=%d", red_layout_idx);
				GroRunReduceLayout *reduce_layout = (GroRunReduceLayout *) cat_array_wo_get(reduce_layouts, red_layout_idx);
				if (reduce_layout==NULL) {
					cat_log_error("no reduce layout for symbol%O state=%O, layout_index%d", for_symbol, state, red_layout_idx);
				}
				grorun_state_add_reduce(state, for_symbol, reduce_layout, action_id);
			} break;
			case 'E': {
				grorun_state_add_accept(state, for_symbol);
			} break;
			default:
				not_done = FALSE;
				break;
		}
	}
	cat_unref_ptr(reduce_layouts);
}


/********************* start GroRunISymbolProvider implementation *********************/

static GroRunSymbol *l_get_symbol_by_name(GroRunISymbolProvider *self, const CatStringWo *name) {
	GroRunModelPrivate *priv = grorun_model_get_instance_private(GRORUN_MODEL(self));
	GroRunSymbol *result = NULL;
	CatIIterator *iter = cat_array_wo_iterator(priv->symbols);
	while(cat_iiterator_has_next(iter)) {
		GroRunSymbol *symbol = (GroRunSymbol *) cat_iiterator_next(iter);
		if (cat_string_wo_equal(grorun_symbol_get_name(symbol), name)) {
			result = symbol;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}


static void l_symbol_provider_iface_init(GroRunISymbolProviderInterface *iface) {
	iface->getSymbolByName = l_get_symbol_by_name;
}

/********************* end GroRunISymbolProvider implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunModel *instance = GRORUN_MODEL(self);
	GroRunModelPrivate *priv = grorun_model_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, symbols=%O]", iname, self, priv->symbols);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/






