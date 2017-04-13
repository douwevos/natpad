/*
   File:    groedcontexteditor.c
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Nov 20, 2016
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

#include "groedcontexteditor.h"
#include "../parser/groedparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroEdContextEditor"
#include <logging/catlog.h>

struct _GroEdContextEditorPrivate {
	GroEdEditorConnector *connector;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_ac_content_provider_iface_init(DraIAcContentProviderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroEdContextEditor, groed_context_editor, DRA_TYPE_CONTEXT_EDITOR,
		G_ADD_PRIVATE(GroEdContextEditor)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_IAC_CONTENT_PROVIDER, l_ac_content_provider_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_format(DraContextEditor *context_editor);

static void groed_context_editor_class_init(GroEdContextEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraContextEditorClass *dra_class = DRA_CONTEXT_EDITOR_CLASS(clazz);
	dra_class->format = l_format;
}

static void groed_context_editor_init(GroEdContextEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroEdContextEditor *instance = GROED_CONTEXT_EDITOR(object);
	GroEdContextEditorPrivate *priv = groed_context_editor_get_instance_private(instance);
	cat_unref_ptr(priv->connector);
	G_OBJECT_CLASS(groed_context_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(groed_context_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroEdContextEditor *groed_context_editor_new(struct _DraEditorPanel *editor_panel, GroEdEditorConnector *connector) {
	GroEdContextEditor *result = g_object_new(GROED_TYPE_CONTEXT_EDITOR, NULL);
	cat_ref_anounce(result);
	GroEdContextEditorPrivate *priv = groed_context_editor_get_instance_private(result);
	priv->connector = cat_ref_ptr(connector);
	dra_context_editor_construct((DraContextEditor *) result, editor_panel);
	return result;
}

static void l_format(DraContextEditor *context_editor) {
	DraEditorPanel *panel = dra_context_editor_get_panel(context_editor);
	DraEditor *editor = dra_editor_panel_get_editor(panel);

	ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	GroRunModel *model = grorun_model_new(gro_parser_config_symbols, gro_parser_config_nr_of_symbols, gro_parser_config_states_text, gro_parser_config_nr_of_states);

	ChaRevisionReader *revision_reader = cha_revision_reader_new(e_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);

	GroRunITokenFactory *token_factory = (GroRunITokenFactory *) grorun_full_token_factory_new();
	GroScannerFactory *scanner_factory = gro_scanner_factory_new(GRORUN_ISYMBOL_PROVIDER(model), token_factory);
	GroScanner *scanner = gro_scanner_factory_create_scanner(scanner_factory, utf8_scanner);

	GroEdParser *parser = groed_parser_new(scanner, token_factory, model);
	cat_unref_ptr(token_factory);
	GroAstSpec *spec = groed_parser_run(parser);
	cat_log_debug("spec=%p", spec);
	if (spec) {
		CatArrayWo *token_cash = groed_parser_get_token_cash(parser);

		CatArrayWo *token_moves = cat_array_wo_new_size(cat_array_wo_size(token_cash));
		cat_array_wo_pad(token_moves, cat_array_wo_size(token_cash), NULL);


		/* lets format Production-definitions in relation with each other */
		GroAstProductionDefinitionList *prod_def_list = groast_spec_get_pl(spec);
		cat_log_debug("prod_def_list=%p", prod_def_list);
		CatArrayWo *prod_def_array = groast_production_definition_list_enlist(prod_def_list);
		CatIIterator *iter = cat_array_wo_iterator(prod_def_array);
		while(cat_iiterator_has_next(iter)) {
			GroRunIToken *tk_prod_def = cat_iiterator_next(iter);
			GroRunLeftRight lr_prod_def = grorun_itoken_get_indexes(tk_prod_def);


			GroRunLocation *loc_right = NULL;
			GroRunIToken *token_right = NULL;
			int offset = lr_prod_def.left;
			gboolean keep_looping = TRUE;
			while(keep_looping && offset>=0) {
				GroRunIToken *token_left = (GroRunIToken *) cat_array_wo_get(token_cash, offset);
				cat_log_detail("token_left=%O", token_left);
				GroRunLocation *loc_left = grorun_itoken_get_location(token_left);
				int usr_idx = grorun_full_token_get_user_index((GroRunFullToken *) token_left);
				if (token_right) {
					/* the token right must be on the next line compared to the token_left */

					int left_col, right_col;
					long int left_row, right_row;
					grorun_location_get_end(loc_left, &left_col, &left_row);
					grorun_location_get_begin(loc_right, &right_col, &right_row);



					int exp_right_row = left_row+1;

					cat_log_detail("usr_idx=%d", usr_idx);

					if (usr_idx!=GRO_SYM_EOL_COMMENT) {
						exp_right_row = left_row+2;
					} else {
						/* token is a comment lets see if the token before is a non comment token */
						if (offset>0) {
							GroRunIToken *token_lah = (GroRunIToken *) cat_array_wo_get(token_cash, offset-1);
							cat_log_detail("token_lah=%O", token_lah);
							int lah_usr_idx = grorun_full_token_get_user_index((GroRunFullToken *) token_lah);
							if (lah_usr_idx!=GRO_SYM_EOL_COMMENT) {
								/* the token before is a non comment token. our comment token may be following this token */
								int lah_col , lah2_col;
								long int lah_row, lah2_row;
								grorun_location_get_begin(loc_left, &lah2_col, &lah2_row);
								grorun_location_get_end(grorun_itoken_get_location(token_lah), &lah_col, &lah_row);
								if (lah_row==lah2_row) {
									/* this comment token belongs to the previous statement. Leave it there we just need to keep our distance */
									keep_looping = FALSE;
									exp_right_row = left_row+2;
								}
							}
						}
					}


					cat_log_debug("exp_right_row=%d, right_row=%d", exp_right_row, right_row);

					if (exp_right_row!=right_row || right_col>0) {
						CatStringWo *replace = cat_string_wo_new("");
						int rr;
						for(rr=left_row; rr<exp_right_row; rr++) {
							cat_string_wo_append_chars(replace, "\n");
						}
						cat_log_debug("replacing left_row=%d, exp_right_row=%d, offset=%d, replace=%O", left_row, exp_right_row, offset, replace);
						cat_array_wo_set(token_moves, (GObject *) replace, offset+1, NULL);
						cat_unref_ptr(replace);
					}

				}
				if (usr_idx==GRO_SYM_EOL_COMMENT || token_right==NULL) {
					offset--;
				} else {
					break;
				}
				token_right = token_left;
				loc_right = loc_left;
			}
		}
		cat_unref_ptr(iter);



		/* lets format the single rules */
		CatHashMapWo *non_term_map = groed_parser_get_non_terminal_map(parser);
		CatArrayWo *nterm_kes = cat_hash_map_wo_enlist_keys(non_term_map, NULL);
		int base_prod_indent= 0;
		CatIIterator *ntiter = cat_array_wo_iterator(nterm_kes);
		while(cat_iiterator_has_next(ntiter)) {
			CatStringWo *t = cat_iiterator_next(ntiter);
			int len = cat_string_wo_length(t);		// TODO should be utf8 length
			if (len>base_prod_indent) {
				base_prod_indent = len;
			}
		}
		cat_unref_ptr(ntiter);
		base_prod_indent += 5;

		cat_log_debug("base_prod_indent=%d", base_prod_indent);

		iter = cat_array_wo_iterator(prod_def_array);
		while(cat_iiterator_has_next(iter)) {
			GroRunIToken *tk_prod_def = cat_iiterator_next(iter);
			GroAstProductionDefinition *prod_def = (GroAstProductionDefinition *) grorun_itoken_get_value(tk_prod_def);

			GroAstRhsRuleList *rhs_rule_list = groast_production_definition_get_ruleList(prod_def);
			if (rhs_rule_list) {
				CatStringWo *lhs_name = groast_production_definition_get_lhsName(prod_def);
				int col = cat_string_wo_length(lhs_name);
				CatArrayWo *rhs_rules = groast_rhs_rule_list_enlist(rhs_rule_list);
				CatIIterator *rule_iter = cat_array_wo_iterator(rhs_rules);
				while(cat_iiterator_has_next(rule_iter)) {
					GroRunIToken *tk_rule = (GroRunIToken *) cat_iiterator_next(rule_iter);
					cat_log_debug("tk_rule=%O", tk_rule);

					GroRunIToken *tk_most_left = tk_rule;

					GroRunLeftRight lr_most_left = grorun_itoken_get_indexes(tk_most_left);
					cat_array_wo_get(token_cash, lr_most_left.left);

					CatStringWo *replace = cat_string_wo_new();
					if (col==0) {
						cat_string_wo_append_chars(replace, "\n");
					}

					while(col<base_prod_indent) {
						cat_string_wo_append_chars(replace, " ");
						col++;
					}
					col = 0;
					cat_array_wo_set(token_moves, (GObject *) replace, lr_most_left.left, NULL);
					cat_unref_ptr(replace);

				}
				cat_unref_ptr(rule_iter);
			}


		}
		cat_unref_ptr(iter);


		cat_log_debug("REPLACING !!!!!!!!!!!! %d", cat_array_wo_size(token_moves));

		int token_idx;
		for(token_idx=cat_array_wo_size(token_moves)-1; token_idx>=0; token_idx--) {
			CatStringWo *replacement = (CatStringWo *) cat_array_wo_get(token_moves, token_idx);
			if (replacement) {
				cat_log_debug("token_idx=%d, rep=%O", token_idx, replacement);
				long row = 0;
				int column = 0;
				if (token_idx>0) {
					GroRunFullToken *prev_token = (GroRunFullToken *) cat_array_wo_get(token_cash, token_idx-1);
					GroRunLocation *location = grorun_full_token_get_location(prev_token);
					cat_log_debug("prev_token=%O", prev_token);
					grorun_location_get_end(location, &column, &row);
				}


				ChaCursorMWo *left_cursor = cha_cursorm_wo_new(row, column);

				GroRunFullToken *end_token = (GroRunFullToken *) cat_array_wo_get(token_cash, token_idx);
				cat_log_debug("end_token=%O", end_token);
				GroRunLocation *location = grorun_full_token_get_location(end_token);
				grorun_location_get_begin(location, &column, &row);
				ChaCursorMWo *right_cursor = cha_cursorm_wo_new(row, column);

				cat_log_debug("left_cursor=%O", left_cursor);
				cat_log_debug("right_cursor=%O", right_cursor);

				cha_revision_wo_remove_between_cursors(e_revision, left_cursor, right_cursor, TRUE, NULL);
				ChaLineLocationWo *nll = cha_revision_wo_calculate_line_location(e_revision, cha_cursorm_wo_get_row(left_cursor));

				ChaCursorWo *lcur = cha_cursor_wo_new_ll_offset(nll, cha_cursorm_wo_get_column(left_cursor));
				cha_revision_wo_set_cursor(e_revision, lcur);
				CatArrayWo *line_list_out = cha_document_create_line_list(document, replacement);
				cha_revision_wo_insert(e_revision, line_list_out);
			}
		}
	}
	cha_document_anchor_document(document);
}


/********************* start DraIAcContentProvider implementation *********************/

static void l_enlist_content(DraIAcContentProvider *content_provider, DraAcContext *ac_context) {
	DraEditorPanel *panel = dra_context_editor_get_panel((DraContextEditor *) content_provider);
	DraEditor *editor = dra_editor_panel_get_editor(panel);
	ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
	ChaRevisionWo *rev_ref = cha_document_get_current_revision_ref(document);

	GroEdContextEditor *instance = GROED_CONTEXT_EDITOR(content_provider);
	GroEdContextEditorPrivate *priv = groed_context_editor_get_instance_private(instance);


	GroEdParser *ed_parser = groed_editor_connector_create_parser(priv->connector, rev_ref);
	groed_parser_run(ed_parser);

	CatHashMapWo *nt_map = groed_parser_get_non_terminal_map(ed_parser);
	CatIMapIterator *miter = cat_hash_map_wo_iterator(nt_map);
	CatStringWo *sym_name = NULL;
	while(cat_imap_iterator_next(miter, &sym_name, NULL)) {
		DraAcReplaceEntry *rep_entry = dra_ac_replace_entry_new(sym_name, sym_name);
		dra_ac_add_entry(ac_context, rep_entry);
		cat_unref_ptr(rep_entry);
	}
	cat_unref_ptr(miter);


	CatHashMapWo *t_map = groed_parser_get_terminal_map(ed_parser);
	miter = cat_hash_map_wo_iterator(t_map);
	while(cat_imap_iterator_next(miter, &sym_name, NULL)) {
		DraAcReplaceEntry *rep_entry = dra_ac_replace_entry_new(sym_name, sym_name);
		dra_ac_add_entry(ac_context, rep_entry);
		cat_unref_ptr(rep_entry);
	}
	cat_unref_ptr(miter);

	cat_unref_ptr(ed_parser)

	cat_unref_ptr(rev_ref);
}


static void l_ac_content_provider_iface_init(DraIAcContentProviderInterface *iface) {
	iface->enlistContent = l_enlist_content;
}

/********************* end DraIAcContentProvider implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



