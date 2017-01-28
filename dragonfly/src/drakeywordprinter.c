/*
   File:    drakeywordprinter.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 4, 2015
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

#include "drakeywordprinter.h"
#include "dralinelayout.h"
#include "attr/draiattributebuilder.h"
#include "attr/draforeground.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraKeywordPrinter"
#include <logging/catlog.h>

struct _DraKeywordPrinterPrivate {
	ChaRevisionWo *a_revision;

	ChaLineIterator *line_iter;

	int current_line_index;

	CatArrayWo *e_ab_list;

	CatArrayWo *e_errors_warnings;


	CatStringWo *a_markup_key;
	int markup_idx;
	gboolean update;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraKeywordPrinter, dra_keyword_printer, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraKeywordPrinter)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_keyword_printer_class_init(DraKeywordPrinterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_keyword_printer_init(DraKeywordPrinter *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraKeywordPrinter *instance = DRA_KEYWORD_PRINTER(object);
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(instance);
	cat_unref_ptr(priv->a_revision);
	cat_unref_ptr(priv->e_ab_list);
	cat_unref_ptr(priv->e_errors_warnings);
	cat_unref_ptr(priv->a_markup_key);
	cat_unref_ptr(priv->line_iter);

	G_OBJECT_CLASS(dra_keyword_printer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_keyword_printer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraKeywordPrinter *dra_keyword_printer_new(ChaRevisionWo *a_revision, CatStringWo *a_markup_key, int markup_idx) {
	DraKeywordPrinter *result = g_object_new(DRA_TYPE_KEYWORD_PRINTER, NULL);
	cat_ref_anounce(result);
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(result);
	priv->a_revision = cat_ref_ptr(a_revision);
	priv->current_line_index = 0;
	priv->e_ab_list = NULL;
	priv->e_errors_warnings = NULL;
	priv->a_markup_key = cat_ref_ptr(a_markup_key);
	priv->markup_idx = markup_idx;
	priv->line_iter = cha_line_iterator_new(a_revision, 0);
	priv->update = FALSE;
	return result;
}

void dra_keyword_printer_reset(DraKeywordPrinter *printer) {
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(printer);
	cat_unref_ptr(priv->e_ab_list);
	cat_unref_ptr(priv->e_errors_warnings);
	cat_unref_ptr(priv->line_iter);
	priv->line_iter = cha_line_iterator_new(priv->a_revision, 0);
	priv->current_line_index = 0;
	priv->update = TRUE;
}




void dra_keyword_printer_print(DraKeywordPrinter *printer, int begin_row, int begin_column, int end_row, int end_column, DraIAttributeBuilder *ab) {
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(printer);
	if (begin_row<priv->current_line_index) {
		/* there is no way back */
		return;
	}

	while(TRUE) {
		if (begin_row == priv->current_line_index) {

			if (priv->e_ab_list==NULL) {
				priv->e_ab_list = cat_array_wo_new();
			}

			int sidx = begin_column;
			int eidx = end_column;
			if (begin_row!=end_row) {
				eidx = 1000000;
			}

			DraIAttributeBuilderInterface *ab_iface = DRA_IATTRIBUTE_BUILDER_GET_INTERFACE(ab);
			ab_iface->setBeginAndEnd(ab, sidx, eidx);
			DraIAttributeBuilder *new_ab = ab_iface->createCopy(ab);
			cat_array_wo_append(priv->e_ab_list, (GObject *) new_ab);
			cat_unref_ptr(new_ab);

			if (begin_row==end_row) {
				return;
			}
			begin_row++;
			begin_column = 0;
		} else {

			const ChaLineLocationWo *a_line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) priv->line_iter);
			if (a_line_location==NULL) {
				return;
			}

			if (priv->update==FALSE || priv->e_ab_list!=NULL) {
				ChaLineWo *a_line = cha_revision_wo_line_at_location(priv->a_revision, a_line_location);

				DraLineInfoWo *line_info = (DraLineInfoWo *)
						cha_line_wo_get_slot_content_ref(a_line, priv->markup_idx, (GObject *) priv->a_markup_key);

				DraLineInfoWo *e_line_info = NULL;
				if (line_info==NULL) {
					e_line_info = dra_line_info_wo_new();
				} else {
					e_line_info = dra_line_info_wo_create_editable(line_info);
				}

				if (priv->update) {
					dra_line_info_wo_append_ab_list(e_line_info, priv->e_ab_list);
				} else {
					dra_line_info_wo_set_ab_list(e_line_info, priv->e_ab_list);
				}
				e_line_info = dra_line_info_wo_anchor(e_line_info, 0);
				cat_unref_ptr(priv->e_ab_list);
				if (e_line_info!=line_info) {
					cha_line_wo_set_slot_content(a_line, priv->markup_idx, (GObject *) priv->a_markup_key, (GObject *) e_line_info);
				}
				cat_unref_ptr(e_line_info);
				cat_unref_ptr(line_info);
				cat_unref_ptr(a_line);
			}

			priv->current_line_index++;
		}
	}
}



void dra_keyword_printer_flush(DraKeywordPrinter *printer) {
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(printer);

	while(TRUE) {

		const ChaLineLocationWo *a_line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) priv->line_iter);
		if (a_line_location==NULL) {
			return;
		}


		if (priv->update==FALSE || priv->e_ab_list!=NULL) {
			ChaLineWo *a_line = cha_revision_wo_line_at_location(priv->a_revision, a_line_location);

			DraLineInfoWo *line_info = (DraLineInfoWo *)
					cha_line_wo_get_slot_content_ref(a_line, priv->markup_idx, (GObject *) priv->a_markup_key);

			DraLineInfoWo *e_line_info = NULL;
			if (line_info==NULL) {
				e_line_info = dra_line_info_wo_new();
			} else {
				e_line_info = dra_line_info_wo_create_editable(line_info);
			}

			if (priv->update) {
				dra_line_info_wo_append_ab_list(e_line_info, priv->e_ab_list);
			} else {
				dra_line_info_wo_set_ab_list(e_line_info, priv->e_ab_list);
			}
			e_line_info = dra_line_info_wo_anchor(e_line_info, 0);
			cat_unref_ptr(priv->e_ab_list);
			if (e_line_info!=line_info) {
				cha_line_wo_set_slot_content(a_line, priv->markup_idx, (GObject *) priv->a_markup_key, (GObject *) e_line_info);
			}
			cat_unref_ptr(e_line_info);
			cat_unref_ptr(line_info);
			cat_unref_ptr(a_line);
		}

		priv->current_line_index++;
	}
}


void dra_keyword_printer_print_fg_color(DraKeywordPrinter *printer, int begin_row, int begin_column, int end_row, int end_column, int color_index) {
	color_index = color_index & 0xF;
    DraForeground *fg_ab = dra_foreground_new(color_index);
    dra_keyword_printer_print(printer, begin_row, begin_column, end_row, end_column, (DraIAttributeBuilder *) fg_ab);
    cat_unref_ptr(fg_ab);
}


void dra_keyword_printer_flush_line_tags(DraKeywordPrinter *printer) {
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(printer);

	while(TRUE) {

		const ChaLineLocationWo *a_line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) priv->line_iter);
		if (a_line_location==NULL) {
			return;
		}

		if (priv->update==FALSE || priv->e_errors_warnings!=NULL) {
			ChaLineWo *a_line = cha_revision_wo_line_at_location(priv->a_revision, a_line_location);

			DraLineInfoWo *line_info = (DraLineInfoWo *)
					cha_line_wo_get_slot_content_ref(a_line, priv->markup_idx, (GObject *) priv->a_markup_key);


			DraLineInfoWo *e_line_info = NULL;

			if (line_info==NULL) {
				e_line_info = dra_line_info_wo_new();
			} else {
				e_line_info = dra_line_info_wo_create_editable(line_info);
			}

			cat_log_debug("priv->e_errors_warnings=%o", priv->e_errors_warnings);
			if (priv->update) {
				dra_line_info_wo_append_line_tags(e_line_info, priv->e_errors_warnings);
			} else {
				dra_line_info_wo_set_line_tags(e_line_info, priv->e_errors_warnings);
			}
			e_line_info = dra_line_info_wo_anchor(e_line_info, 0);
			cat_unref_ptr(priv->e_errors_warnings);
			if (e_line_info!=line_info) {
				cha_line_wo_set_slot_content(a_line, priv->markup_idx, (GObject *) priv->a_markup_key, (GObject *) e_line_info);
			}
			cat_unref_ptr(e_line_info);
			cat_unref_ptr(line_info);
			cat_unref_ptr(a_line);
		}
		priv->current_line_index++;
	}
}


void dra_keyword_printer_print_line_tag(DraKeywordPrinter *printer, DraLineTagWo *c_line_tag_wo) {
	cat_stacktrace_print();
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(printer);
	int row = dra_line_tag_wo_get_row(c_line_tag_wo);
	if (row<priv->current_line_index) {
		/* there is no way back */
		cat_unref_ptr(c_line_tag_wo);
		return;
	}

	while(TRUE) {
		if (row == priv->current_line_index) {

			if (priv->e_errors_warnings==NULL) {
				priv->e_errors_warnings = cat_array_wo_new();
			}

			cat_log_error("added :%O", c_line_tag_wo);
			cat_array_wo_append(priv->e_errors_warnings, (GObject *) c_line_tag_wo);
			cat_unref_ptr(c_line_tag_wo);
			return;
		} else {
			/* close current edited line and move to next line */

			const ChaLineLocationWo *a_line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) priv->line_iter);
			if (a_line_location==NULL) {
				cat_unref_ptr(c_line_tag_wo);
				return;
			}

			if (priv->update==FALSE || priv->e_errors_warnings!=NULL) {
				ChaLineWo *a_line = cha_revision_wo_line_at_location(priv->a_revision, a_line_location);

				DraLineInfoWo *line_info = (DraLineInfoWo *)
						cha_line_wo_get_slot_content_ref(a_line, priv->markup_idx, (GObject *) priv->a_markup_key);


				DraLineInfoWo *e_line_info = NULL;

				if (line_info==NULL) {
					e_line_info = dra_line_info_wo_new();
				} else {
					e_line_info = dra_line_info_wo_create_editable(line_info);
				}

				cat_log_debug("priv->e_errors_warnings=%o", priv->e_errors_warnings);
				if (priv->update) {
					dra_line_info_wo_append_line_tags(e_line_info, priv->e_errors_warnings);
				} else {
					dra_line_info_wo_set_line_tags(e_line_info, priv->e_errors_warnings);
				}
				e_line_info = dra_line_info_wo_anchor(e_line_info, 0);
				cat_unref_ptr(priv->e_errors_warnings);
				if (e_line_info!=line_info) {
					cha_line_wo_set_slot_content(a_line, priv->markup_idx, (GObject *) priv->a_markup_key, (GObject *) e_line_info);
				}
				cat_unref_ptr(e_line_info);
				cat_unref_ptr(line_info);
				cat_unref_ptr(a_line);
			}
			priv->current_line_index++;
		}
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraKeywordPrinter *instance = DRA_KEYWORD_PRINTER(self);
	DraKeywordPrinterPrivate *priv = dra_keyword_printer_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p line_iter=%o]", iname, self, priv->line_iter);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
