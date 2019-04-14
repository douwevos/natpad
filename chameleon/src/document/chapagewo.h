/*
   File:    chapagewo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 24, 2015
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

#ifndef CHAPAGEWO_H_
#define CHAPAGEWO_H_

#include "chalinewo.h"
#include "chalinemultireplace.h"
#include "../charset/chaiconverter.h"
#include <caterpillar.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define CHA_PAGE_SIZE_LOW	2048
#define CHA_PAGE_SIZE_PREF	8192
#define CHA_PAGE_SIZE_MAX	16384

//#define CHA_PAGE_SIZE_LOW	8
//#define CHA_PAGE_SIZE_PREF	16
//#define CHA_PAGE_SIZE_MAX	32

typedef struct _ChaUtf8Text ChaUtf8Text;

struct _ChaUtf8Text {
	const char *text;
	int text_len;
	ChaLineEnd line_end;
	gboolean only_ascii;
	gboolean has_tabs;
	gboolean text_needs_cleanup;
};

#define CHA_TYPE_PAGE_WO              (cha_page_wo_get_type())
#define CHA_PAGE_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_page_wo_get_type(), ChaPageWo))
#define CHA_PAGE_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PAGE_WO, ChaPageWoClass))
#define CHA_IS_PAGE_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PAGE_WO))
#define CHA_IS_PAGE_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PAGE_WO))
#define CHA_PAGE_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PAGE_WO, ChaPageWoClass))


typedef struct _ChaPageWo               ChaPageWo;
typedef struct _ChaPageWoPrivate        ChaPageWoPrivate;
typedef struct _ChaPageWoClass          ChaPageWoClass;


struct _ChaPageWo {
	CatWo parent;
};

typedef struct _ChaWriteReq ChaWriteReq;

struct _ChaWriteReq {
	GOutputStream *out_stream;
	ChaIConverter *charset_converter;
	GError *error;
	gboolean needs_conversion;
	ChaLineEnd line_ends;
	gboolean line_ends_are_mixed;
};

struct _ChaPageWoClass {
	CatWoClass parent_class;

	gboolean (*writeToStream)(ChaPageWo *page, ChaWriteReq *write_req);

	void (*enrich)(ChaPageWo *page, ChaEnrichmentDataMapWo *a_map);
	void (*impoverish)(ChaPageWo *page);
	void (*enrichmentRemap)(ChaPageWo *page, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);

	void (*holdLines)(ChaPageWo *page);
	void (*releaseLines)(ChaPageWo *page);

	int (*lineCount)(ChaPageWo *page);
	ChaLineWo *(*lineRefAt)(ChaPageWo *page, int index);

	const ChaUtf8Text (*utf8At)(ChaPageWo *page, int line_index, gboolean do_scan);

	void (*addLine)(ChaPageWo *e_page, ChaLineWo *line);
	void (*insertLine)(ChaPageWo *e_page, ChaLineWo *line, int index);
	void (*removeRange)(ChaPageWo *e_page, int first, int last);
	ChaLineWo *(*editableLineAt)(ChaPageWo *e_page, int index);

	void (*search)(const ChaPageWo *page, CatArrayWo *e_result, const CatStringWo *text, gboolean match_case);
};


GType cha_page_wo_get_type();

//ChaPageWo *cha_page_wo_new();
//ChaPageWo *cha_page_wo_new_from(ChaPageWo *src_page, int version);

gboolean cha_page_wo_write_to_stream(ChaPageWo *page, ChaWriteReq *write_req);

CatLock *cha_page_wo_get_lock(ChaPageWo *page);

void cha_page_wo_enrich(ChaPageWo *page, ChaEnrichmentDataMapWo *a_map);
void cha_page_wo_impoverish(ChaPageWo *page);
void cha_page_wo_enrichment_remap(ChaPageWo *page, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);

GObject *cha_page_wo_get_slot_content_ref(ChaPageWo *page, int slot_index, GObject *slot_key);
void cha_page_wo_set_slot_content(ChaPageWo *page, int slot_index, GObject *slot_key, GObject *content);

void cha_page_wo_hold_lines(ChaPageWo *page);
void cha_page_wo_release_lines(ChaPageWo *page);

int cha_page_wo_line_count(ChaPageWo *page);
ChaLineWo *cha_page_wo_line_at(ChaPageWo *page, int index);

const ChaUtf8Text cha_page_wo_utf8_at(ChaPageWo *page, int line_index, gboolean do_scan);


void cha_utf8_text_cleanup(const ChaUtf8Text *text);

void cha_page_wo_add_line(ChaPageWo *e_page, ChaLineWo *line);
void cha_page_wo_insert_line(ChaPageWo *e_page, ChaLineWo *line, int index);
void cha_page_wo_remove_range(ChaPageWo *e_page, int first, int last);

ChaLineWo *cha_page_wo_editable_line_at(ChaPageWo *e_page, int index);
void cha_page_wo_line_multi_replace(ChaPageWo *e_page, int line_index, ChaLineMultiReplace *line_multi_replace, int *left_result, int *right_result);

gboolean cha_page_wo_is_line_marked(ChaPageWo *e_page, int line_index);

void cha_page_wo_set_line_mark(ChaPageWo *e_page, int line_index);
void cha_page_wo_unset_line_mark(ChaPageWo *e_page, int line_index);
void cha_page_wo_toggle_line_mark(ChaPageWo *e_page, int line_index);
void cha_page_wo_unmark_all_lines(ChaPageWo *e_page);
void cha_page_wo_mark_all_lines(ChaPageWo *e_page);
void cha_page_wo_mark_toggle_all_lines(ChaPageWo *e_page);

gboolean cha_page_wo_has_marked_lines(const ChaPageWo *page);

CAT_WO_BASE_H(ChaPageWo, cha_page_wo);

typedef gboolean (*ChaScannedLine)(char *off_line_start, char *off_line_end, ChaLineEnd line_end, void *data);

void cha_scan_lines(char *text, char *end, ChaScannedLine cha_scanned_line, void *data);

G_END_DECLS

#endif /* CHAPAGEWO_H_ */
