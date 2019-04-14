/*
   File:    charevisionprinter.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 5, 2015
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

#include "charevisionprinter.h"
#include "chaprintpage.h"
#include "../layout/chadocumentview.h"
#include "../document/chalineiterator.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaRevisionPrinter"
#include <logging/catlog.h>

struct _ChaRevisionPrinterPrivate {
	ChaPrintSettings *print_settings;
	ChaEditor *editor;
	CatStringWo *a_markup_hook_key;
	ChaSurfacePool *surface_pool;
	cairo_t *cairo;
	PangoContext *pango_context;
	PangoLayout *pango_layout;
	double page_width;
	double page_height;
	CatArrayWo *e_pages;

	double page_text_height;
	double page_text_width;
	double p_number_width;
	double p_space_width;
	double p_tab_width;
	double p_pango_font_height;

	double margin_top, margin_bottom, margin_left, margin_right;
	int tab_size;

	ChaPrefsColorMapWo *color_map;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaRevisionPrinter, cha_revision_printer, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaRevisionPrinter)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_revision_printer_class_init(ChaRevisionPrinterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_revision_printer_init(ChaRevisionPrinter *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaRevisionPrinter *instance = CHA_REVISION_PRINTER(object);
	ChaRevisionPrinterPrivate *priv = cha_revision_printer_get_instance_private(instance);
	cat_unref_ptr(priv->color_map);
	cat_unref_ptr(priv->a_markup_hook_key);
	cat_unref_ptr(priv->e_pages);
	cat_unref_ptr(priv->surface_pool);
	G_OBJECT_CLASS(cha_revision_printer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_revision_printer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_create_pages(ChaRevisionPrinter *revision_print);

ChaRevisionPrinter *cha_revision_printer_new(ChaPrintSettings *print_settings, ChaEditor *editor, CatStringWo *a_markup_hook_key, cairo_t *cairo, PangoContext *pango_context, PangoLayout *pango_layout, double page_width, double page_height, double margin_top, double margin_bottom, double margin_left, double margin_right) {
	ChaRevisionPrinter *result = g_object_new(CHA_TYPE_REVISION_PRINTER, NULL);
	cat_ref_anounce(result);
	ChaRevisionPrinterPrivate *priv = cha_revision_printer_get_instance_private(result);
	cat_log_debug("page_width=%lf, page_height=%lf", page_width, page_height);
	priv->print_settings = cat_ref_ptr(print_settings);
	priv->editor = cat_ref_ptr(editor);
	priv->a_markup_hook_key = cat_ref_ptr(a_markup_hook_key);
	priv->cairo = cairo;
	priv->pango_context = pango_context;
	priv->pango_layout = pango_layout;
	priv->page_width = page_width;
	priv->page_height = page_height;
	priv->e_pages = cat_array_wo_new();
	priv->surface_pool = cha_surface_pool_new();
	priv->margin_top = margin_top;
	priv->margin_bottom = margin_bottom;
	priv->margin_left = margin_left;
	priv->margin_right = margin_right;


	ChaDocumentView *document_view = cha_editor_get_document_view(editor);
	priv->color_map = cha_document_view_get_color_map(document_view);
	cat_ref_ptr(priv->color_map);



	cat_log_debug("top/left=%d,%d", (int) priv->margin_top, (int) priv->margin_left);

	l_create_pages(result);
	return result;
}

int cha_revision_printer_get_page_count(ChaRevisionPrinter *editor_printer) {
	ChaRevisionPrinterPrivate *priv = cha_revision_printer_get_instance_private(editor_printer);
	return cat_array_wo_size(priv->e_pages);
}



static void l_create_pages(ChaRevisionPrinter *revision_print) {
	ChaRevisionPrinterPrivate *priv = cha_revision_printer_get_instance_private(revision_print);
	ChaDocumentView *document_view = cha_editor_get_document_view(priv->editor);
	ChaDocument *document = cha_editor_get_document(priv->editor);
	ChaRevisionWo *a_revision = cha_document_get_current_revision_ref(document);


	priv->page_text_height = priv->page_height - priv->margin_top - priv->margin_bottom;
	priv->page_text_width = priv->page_width - priv->margin_left - priv->margin_right;

	const PangoFontDescription *font_decscr = pango_layout_get_font_description(priv->pango_layout);

	PangoFontDescription *new_font_descr = NULL;
	CatStringWo *font_name = cha_print_settings_get_font_name(priv->print_settings);
	new_font_descr = pango_font_description_from_string(cat_string_wo_getchars(font_name));
	cat_log_warn("new_font_descr=%p, font_decscr=%p", new_font_descr, font_decscr);
//	pango_font_description_set_weight(new_font_descr, cha_print_settings_get_font_is_bold(priv->print_settings) ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);

	cat_log_error("fontje: %o", font_name);
	cat_log_error("fontje:%p, %s", pango_font_description_to_string(new_font_descr), pango_font_description_to_string(new_font_descr));



	pango_layout_set_font_description(priv->pango_layout, new_font_descr);
	PangoFontMetrics *pango_font_metrics = pango_context_get_metrics(priv->pango_context, new_font_descr, gtk_get_default_language());
	// TODO free new font descr ?

	pango_context_set_font_description(priv->pango_context, new_font_descr);

	cat_log_error("ascent=%d, descent=%d", pango_font_metrics_get_ascent(pango_font_metrics), pango_font_metrics_get_descent(pango_font_metrics));


	const ChaPreferencesWo *a_e_prefs = cha_editor_get_preferences(priv->editor);
	int tab_size = cha_preferences_wo_get_tab_size(a_e_prefs);

	char *str = g_strnfill(tab_size, ' ');
	pango_layout_set_text(priv->pango_layout, str, -1);
	g_free (str);
	int tab_width = 0;
	pango_layout_get_size (priv->pango_layout, &tab_width, NULL);
	PangoTabArray *tabs = pango_tab_array_new(1, FALSE);
	pango_tab_array_set_tab(tabs, 0, PANGO_TAB_LEFT, tab_width);

	pango_layout_set_tabs(priv->pango_layout, tabs);
	pango_tab_array_free(tabs);

	priv->tab_size = tab_width;



	priv->p_space_width = pango_font_metrics_get_approximate_char_width(pango_font_metrics)/PANGO_SCALE;
	priv->p_tab_width = priv->p_space_width*8;
	priv->p_pango_font_height = ((double) (pango_font_metrics_get_ascent(pango_font_metrics)+pango_font_metrics_get_descent(pango_font_metrics)))/PANGO_SCALE;

	priv->p_number_width = 0;
	if (cha_print_settings_print_line_numbers(priv->print_settings)) {
		double digit_width = (double) pango_font_metrics_get_approximate_digit_width(pango_font_metrics)/PANGO_SCALE;
		long long line_count = cha_revision_wo_count_lines(a_revision);
		priv->p_number_width += digit_width / 2;
		while(line_count!=0) {
			line_count /= 10;
			priv->p_number_width += digit_width;
		}
	}


	/* header and footer */
	if (cha_print_settings_get_print_header_and_footer(priv->print_settings)) {
		priv->page_text_height -= (priv->p_pango_font_height+3)*2;
	}

//	CatStringWo *slot_key = priv->a_markup_hook_key;
//	int slot_idx = cha_revision_wo_get_slot_index(a_revision, slot_key, -1);

	long long line_number = 0;

	gboolean wrap_lines = cha_print_settings_get_wrap_lines(priv->print_settings);
	gboolean do_print_markup = cha_print_settings_get_print_markup(priv->print_settings);

	ChaLineEnd line_ends = cha_revision_wo_get_line_ends(a_revision);
	gboolean line_ends_are_mixed = cha_revision_wo_get_line_ends_are_mixed(a_revision);

	ChaPrintPage *last_page = NULL;

	CatIIterator *line_iterator = (CatIIterator *) cha_line_iterator_new(a_revision, 0);
	while(cat_iiterator_has_next(line_iterator)) {
		ChaLineLocationWo *line_location = (ChaLineLocationWo *) cat_iiterator_next(line_iterator);
		ChaLineWo *line = cha_revision_wo_line_at_location(a_revision, line_location);
		CatStringWo *a_text = cha_line_wo_get_text(line);
		ChaLineEnd line_end = cha_line_wo_compute_line_end(line, line_ends, line_ends_are_mixed);

		ChaLineLayout *line_layout = NULL;
		ChaDocumentViewClass *dv_class = CHA_DOCUMENT_VIEW_GET_CLASS(document_view);
		if (dv_class->createLineLayout) {
			line_layout = dv_class->createLineLayout(document_view);
		} else {
			line_layout = cha_line_layout_new();
		}


		cha_line_layout_set_text(line_layout, a_text, line_end, wrap_lines, priv->tab_size, priv->page_text_width-priv->p_number_width-priv->p_pango_font_height, 0);

		if (do_print_markup && dv_class->onInvalidateLine) {
			int page_index = cha_line_location_wo_get_page_index(line_location);
			int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
			dv_class->onInvalidateLine(document_view, page_index, page_line_index, line, line_layout);
		}


		cha_line_layout_update(line_layout, priv->pango_context, priv->color_map);

		PangoLayout *pango_layout = cha_line_layout_get_pango_layout(line_layout);
		int line_count = pango_layout_get_line_count(pango_layout);
		int line_idx;
		for(line_idx=0; line_idx<line_count; line_idx++) {
			PangoLayoutLine *pango_ll = pango_layout_get_line_readonly(pango_layout, line_idx);
			PangoRectangle sub_line_rect;
			pango_layout_line_get_pixel_extents(pango_ll, NULL, &sub_line_rect);
			int sub_line_height = sub_line_rect.height;

			if (last_page==NULL) {
				last_page = cha_print_page_new();
				cat_array_wo_append(priv->e_pages, (GObject *) last_page);
				cat_unref(last_page);
			}

			double lines_view_height = cha_print_page_get_lines_view_height(last_page);
			if (lines_view_height+sub_line_height>priv->page_text_height) {
				last_page = cha_print_page_new();
				cat_array_wo_append(priv->e_pages, (GObject *) last_page);
				cat_unref(last_page);
			}

			ChaPrintLine *print_line = cha_print_line_new(line_layout, line_idx, sub_line_height, line_number);
			cha_print_page_append_line(last_page, print_line);
		}
		cat_unref_ptr(line_layout);
		cat_unref_ptr(line);
		line_number++;
	}
	cat_unref_ptr(line_iterator);
	cat_unref_ptr(a_revision);
}

static void l_print_line(ChaRevisionPrinterPrivate *priv, ChaPrintLine *print_line, double *y_out, ChaSubLineCacheUpdate *update_ctx) {
	ChaDocumentView *document_view = cha_editor_get_document_view(priv->editor);

	ChaLineLayout *line_layout = cha_print_line_get_line_layout(print_line);
	cha_line_layout_lock(line_layout, TRUE);

	cha_line_layout_mark_attributes_dirty(line_layout);


	cha_line_layout_update(line_layout, priv->pango_context, priv->color_map);

	PangoLayout *pango_layout = cha_line_layout_get_pango_layout(line_layout);


	int sub_line_idx = cha_print_line_get_sub_line_index(print_line);

	update_ctx->line_nr = cha_print_line_get_line_nr(print_line);
	update_ctx->pango_layout = pango_layout;


	int line_view_height = cha_print_line_get_view_height(print_line);

	cha_surface_pool_set_image_dimension(priv->surface_pool, (int) priv->page_text_width, line_view_height);

	ChaDocumentViewClass *doc_view_class = CHA_DOCUMENT_VIEW_GET_CLASS(document_view);
	ChaSubLineCache *sub_line_cache = NULL;
	if (doc_view_class->createSubLineCache) {
		sub_line_cache = doc_view_class->createSubLineCache(priv->surface_pool, line_layout, sub_line_idx);
	} else {
		sub_line_cache = cha_sub_line_cache_new(priv->surface_pool, line_layout, sub_line_idx);
	}

	int ll_width = 0;
	int ll_count = 0;
	cha_line_layout_get_width_and_sub_line_count(line_layout, &ll_width, &ll_count);
	update_ctx->page_sub_line_y = *y_out;
	update_ctx->sub_line_height = line_view_height;

//	cat_log_debug("ll_height=%d, line_view_height=%d", ll_height, line_view_height);


	ChaCairoSurfaceWrapper *wrapper = cha_cairo_surface_wrapper_new_cairo(priv->cairo);
	cairo_save(priv->cairo);
	cairo_translate(priv->cairo, 0, *y_out);

	cha_sub_line_cache_set_surface_wrapper(sub_line_cache, wrapper, update_ctx->view_width, line_view_height);
	cha_sub_line_cache_update(sub_line_cache, update_ctx);

	cairo_restore(priv->cairo);


	cha_sub_line_cache_draw(sub_line_cache, priv->cairo, 0, 0);

	cat_unref_ptr(sub_line_cache);
	*y_out = (*y_out) + line_view_height;
	cha_line_layout_unlock(line_layout);

}

static CatS ast_s_txt_page_nr = CAT_S_DEF("[%page%]");
static CatS ast_s_txt_page_count = CAT_S_DEF("[%page-count%]");

CatStringWo *l_replace_all_with(CatStringWo *in, const CatStringWo *replace, const CatStringWo *with) {
	cat_log_debug("in=%o, replace=%o, with=%o", in, replace, with);
	int replen = cat_string_wo_length(replace);
	while(TRUE) {
		int idx = cat_string_wo_index_of_string(in, replace);
		cat_log_debug("idx=%d", idx);
		if (idx>=0) {
			cat_string_wo_replace_range_with(in, idx, replen, with);
		} else {
			break;
		}
	}
	return in;
}

CatStringWo *l_convert_header_footer_text(ChaRevisionPrinter *revision_print, CatStringWo *e_text, int page_index) {
	ChaRevisionPrinterPrivate *priv = cha_revision_printer_get_instance_private(revision_print);

	char txt[30];
	sprintf(txt, "%d", (page_index+1));
	const CatStringWo *txt_page_nr = cat_string_wo_new_data(txt);
	e_text = l_replace_all_with(e_text, CAT_S(ast_s_txt_page_nr), txt_page_nr);
	cat_unref_ptr(txt_page_nr);

	sprintf(txt, "%d", cat_array_wo_size(priv->e_pages));
	const CatStringWo *txt_page_cnt = cat_string_wo_new_data(txt);
	e_text = l_replace_all_with(e_text, CAT_S(ast_s_txt_page_count), txt_page_cnt);
	cat_unref_ptr(txt_page_cnt);

	return e_text;
}

void cha_revision_printer_draw_page(ChaRevisionPrinter *revision_print, int page_index) {
	ChaRevisionPrinterPrivate *priv = cha_revision_printer_get_instance_private(revision_print);

	ChaPrintPage *page_print = (ChaPrintPage *) cat_array_wo_get(priv->e_pages, page_index);
	if (page_print) {

		ChaSubLineCacheUpdate update_ctx;
		update_ctx.window = NULL;
		update_ctx.pango_layout = priv->pango_layout;
		update_ctx.a_preferences = NULL;
		update_ctx.form_fields_on_line = NULL;
		update_ctx.form_fields_hash = 0;
		update_ctx.selection_markings = NULL;
		update_ctx.page_sub_line_y = 0;
		update_ctx.view_x = 0;
		update_ctx.view_width = priv->page_text_width;
		update_ctx.sub_line_height = priv->p_pango_font_height;
		update_ctx.attributes_version = 0;
		update_ctx.cursor_x_pos = -1;
		update_ctx.line_nr = 1;
		update_ctx.line_nr_view_width = priv->p_number_width;
		update_ctx.scratch = pango_layout_copy(priv->pango_layout);	// TODO free
		update_ctx.paint_mark = FALSE;
		update_ctx.show_whitespace = FALSE;
		update_ctx.cache_dirty = TRUE;
		update_ctx.highlight_current_line = FALSE;
		update_ctx.color_map = priv->color_map;



		int line_count = cha_print_page_line_count(page_print);
		int line_idx;
		double y_out = priv->margin_top;

		/* header */
		GdkRGBA num_color = cha_print_settings_get_line_number_color(priv->print_settings);

		if (cha_print_settings_get_print_header_and_footer(priv->print_settings)) {
			cairo_set_source_rgb(priv->cairo, num_color.red, num_color.green, num_color.blue);
			CatStringWo *e_header_text = cha_print_settings_get_header_text(priv->print_settings);
			if (e_header_text && cat_string_wo_length(e_header_text)>0) {
				e_header_text = cat_string_wo_clone(e_header_text, CAT_CLONE_DEPTH_MAIN);

				e_header_text = l_convert_header_footer_text(revision_print, e_header_text, page_index);

				cairo_move_to(priv->cairo, 0, y_out);
				pango_layout_set_text(update_ctx.scratch, cat_string_wo_getchars(e_header_text), -1);
				pango_cairo_show_layout(priv->cairo, update_ctx.scratch);
				cat_unref_ptr(e_header_text);
				cat_log_debug("priv->p_pango_font_height=%d", (int) priv->p_pango_font_height);
				y_out += priv->p_pango_font_height+3;

				cairo_set_line_width(priv->cairo, 0.5);
				cairo_move_to(priv->cairo, 0.0, y_out-1.5);
				cairo_line_to(priv->cairo, priv->page_width, y_out-1.5);
				cairo_stroke(priv->cairo);
			}
		}


		for(line_idx=0; line_idx<line_count; line_idx++) {
			ChaPrintLine *print_line = cha_print_page_line_at(page_print, line_idx);
//			cat_log_debug("line_idx=%d, y_out=%d", line_idx, (int) y_out);
			l_print_line(priv, print_line, &y_out, &update_ctx);
		}

		if (cha_print_settings_get_print_header_and_footer(priv->print_settings)) {
			cairo_set_source_rgb(priv->cairo, (double) (num_color.red)/(double) 0xFFFF, (double) (num_color.green)/(double) 0xFFFF, (double) (num_color.blue)/(double) 0xFFFF);
			CatStringWo *e_footer_text = cha_print_settings_get_footer_text(priv->print_settings);
			if (e_footer_text && cat_string_wo_length(e_footer_text)>0) {
				e_footer_text = cat_string_wo_create_editable(e_footer_text);

				e_footer_text = l_convert_header_footer_text(revision_print, e_footer_text, page_index);

				y_out = priv->margin_top+priv->page_text_height+(priv->p_pango_font_height+3);

				cairo_move_to(priv->cairo, 0, y_out+3);
				pango_layout_set_text(priv->pango_layout, cat_string_wo_getchars(e_footer_text), -1);
				pango_cairo_show_layout(priv->cairo, priv->pango_layout);
				cat_unref_ptr(e_footer_text);
			}

			cairo_set_line_width(priv->cairo, 0.5);
			cairo_move_to(priv->cairo, 0.0, y_out+1.5);
			cairo_line_to(priv->cairo, priv->page_width, y_out-1.5);
			cairo_stroke(priv->cairo);
		}

	}
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaRevisionPrinter *instance = CHA_REVISION_PRINTER(self);
	ChaRevisionPrinterPrivate *priv = cha_revision_printer_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor=%p]", iname, self, priv->editor);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
