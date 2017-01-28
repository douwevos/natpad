/*
   File:    cushighlighter.c
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 7, 2016
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

#include "cushighlighter.h"
#include "../parser/cusplainparser.h"
#include <gross.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CusHighlighter"
#include <logging/catlog.h>

struct _CusHighlighterPrivate {
	ChaDocument *document;
	CatStringWo *a_markup_key;
	CatLock *lock;
	int markup_slot_idx;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CusHighlighter, cus_highlighter, DRA_TYPE_HIGHLIGHTER,
		G_ADD_PRIVATE(CusHighlighter)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_highlighter(DraHighlighter *highlighter, ChaRevisionWo *a_new_revision);

static void cus_highlighter_class_init(CusHighlighterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraHighlighterClass *dra_class = DRA_HIGHLIGHTER_CLASS(clazz);
	dra_class->runHighlighter = l_run_highlighter;
}

static void cus_highlighter_init(CusHighlighter *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	CusHighlighter *instance = CUS_HIGHLIGHTER(object);
//	CusHighlighterPrivate *priv = cus_highlighter_get_instance_private(instance);
	G_OBJECT_CLASS(cus_highlighter_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cus_highlighter_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CusHighlighter *cus_highlighter_new(WorService *wor_service, ChaDocument *document, CatStringWo *a_markup_key) {
	CusHighlighter *result = g_object_new(CUS_TYPE_HIGHLIGHTER, NULL);
	cat_ref_anounce(result);
	CusHighlighterPrivate *priv = cus_highlighter_get_instance_private(result);
	priv->document = cat_ref_ptr(document);
	priv->a_markup_key = cat_ref_ptr(a_markup_key);
	priv->lock = cat_lock_new();
	dra_highlighter_construct((DraHighlighter *) result, wor_service, document, a_markup_key);
	return result;
}


struct HighAndRev {
	DraHighlighter *highlighter;
	ChaRevisionWo *rev;
};

static gboolean l_idle_cb(struct HighAndRev *high_and_rev) {
	dra_highlighter_notify_document(high_and_rev->highlighter, high_and_rev->rev, NULL);
	cat_unref_ptr(high_and_rev->highlighter);
	cat_unref_ptr(high_and_rev->rev);
	cat_free_ptr(high_and_rev);
	return FALSE;
}



static void l_run_scanner(DraHighlighter *highlighter, ChaRevisionWo *a_revision) {
	CusHighlighterPrivate *priv = cus_highlighter_get_instance_private((CusHighlighter *) highlighter);

	DraKeywordPrinter *keyword_printer = dra_keyword_printer_new(a_revision, priv->a_markup_key, priv->markup_slot_idx);
	DraKeywordPrinter *line_tag_printer = dra_keyword_printer_new(a_revision, priv->a_markup_key, priv->markup_slot_idx);

	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);


	GroIScanner *scanner = (GroIScanner *) gro_scanner_new((CatIUtf8Scanner *) utf8_scanner);

    CusPlainParser *plain_parser = cus_plain_parser_new(GRO_ISCANNER(scanner), keyword_printer, line_tag_printer);
    cus_plain_parser_run(plain_parser);


	dra_keyword_printer_flush_line_tags(line_tag_printer);
	dra_keyword_printer_flush(keyword_printer);

	cat_unref_ptr(utf8_scanner);
	cat_unref_ptr(plain_parser);
	cat_unref_ptr(scanner);
    cat_unref_ptr(line_tag_printer);
    cat_unref_ptr(keyword_printer);
}


static void l_run_highlighter(DraHighlighter *highlighter, ChaRevisionWo *a_new_revision) {
	CusHighlighterPrivate *priv = cus_highlighter_get_instance_private((CusHighlighter *) highlighter);

	priv->markup_slot_idx = cha_revision_wo_get_slot_index(a_new_revision, (GObject *) priv->a_markup_key, priv->markup_slot_idx);
	cat_log_debug("Running:markup_slot_idx=%d", priv->markup_slot_idx);
	if (priv->markup_slot_idx<0) {
		return;
	}

	l_run_scanner(highlighter, a_new_revision);

	struct HighAndRev *high_and_rev = g_new(struct HighAndRev, 1);
	high_and_rev->highlighter = cat_ref_ptr(highlighter);
	high_and_rev->rev = cat_ref_ptr(a_new_revision);
	g_idle_add((GSourceFunc) l_idle_cb, high_and_rev);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CusHighlighter *instance = CUS_HIGHLIGHTER(self);
	CusHighlighterPrivate *priv = cus_highlighter_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
