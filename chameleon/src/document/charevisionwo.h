/*
   File:    charevisionwo.h
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

#ifndef CHAREVISIONWO_H_
#define CHAREVISIONWO_H_

#include "chapagewo.h"
#include "chaformwo.h"
#include "chacursorwo.h"
#include "chalinelocationwo.h"
#include "chaenrichmentdatamapwo.h"
#include "chaloadtoken.h"
#include <caterpillar.h>

G_BEGIN_DECLS



#define CHA_TYPE_REVISION_WO              (cha_revision_wo_get_type())
#define CHA_REVISION_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_revision_wo_get_type(), ChaRevisionWo))
#define CHA_REVISION_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_REVISION_WO, ChaRevisionWoClass))
#define CHA_IS_REVISION_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_REVISION_WO))
#define CHA_IS_REVISION_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_REVISION_WO))
#define CHA_REVISION_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_REVISION_WO, ChaRevisionWoClass))


typedef struct _ChaRevisionWo               ChaRevisionWo;
typedef struct _ChaRevisionWoPrivate        ChaRevisionWoPrivate;
typedef struct _ChaRevisionWoClass          ChaRevisionWoClass;


struct _ChaRevisionWo {
	CatWo parent;
};

struct _ChaRevisionWoClass {
	CatWoClass parent_class;
};


GType cha_revision_wo_get_type();

ChaRevisionWo *cha_revision_wo_new();


int cha_revision_wo_get_content_version(ChaRevisionWo *revision);

void cha_revision_wo_enrich(ChaRevisionWo *revision, ChaEnrichmentDataMapWo *a_map);
void cha_revision_wo_impoverish(ChaRevisionWo *revision);
void cha_revision_wo_enrichment_remap(ChaRevisionWo *revision, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);
int cha_revision_wo_get_slot_index(ChaRevisionWo *revision, GObject *slot_key, int slot_key_idx);
GObject *cha_revision_wo_get_slot_content_ref(ChaRevisionWo *revision, int slot_index, GObject *slot_key);
void cha_revision_wo_set_slot_content(ChaRevisionWo *revision, int slot_index, GObject *slot_key, GObject *content);

ChaLineEnd cha_revision_wo_get_line_ends(const ChaRevisionWo *revision);
gboolean cha_revision_wo_get_line_ends_are_mixed(const ChaRevisionWo *revision);

void cha_revision_wo_set_line_ends(ChaRevisionWo *e_revision, ChaLineEnd line_ends, gboolean line_ends_are_mixed);


ChaLoadToken *cha_revision_wo_get_load_token(ChaRevisionWo *revision);
void cha_revision_wo_set_load_token(ChaRevisionWo *e_revision, ChaLoadToken *load_token);

void cha_revision_wo_insert(ChaRevisionWo *e_revision, CatArrayWo *ce_lines_2_insert);
void cha_revision_wo_remove(ChaRevisionWo *e_revision, ChaCursorWo *cursor);

void cha_revision_wo_replace_unichar(ChaRevisionWo *e_revision, ChaCursorMWo *e_insert_location, gunichar unichar, gboolean handle_form);

void cha_revision_wo_insert_lines(ChaRevisionWo *e_revision, ChaCursorMWo *e_insert_location, CatArrayWo *lines_to_insert, gboolean handle_form);
void cha_revision_wo_remove_between_cursors(ChaRevisionWo *e_revision, ChaCursorMWo *cursor_a, ChaCursorMWo *cursor_b, gboolean modify_form, ChaCursorMWo **new_cursor);

void cha_revision_wo_line_multi_replace(ChaRevisionWo *e_revision, long long row, ChaPageWo *e_page, int page_line_index, ChaLineMultiReplace *line_multiple_replace, int *left_result, int *right_result);

ChaCursorWo *cha_revision_wo_create_end_of_revision_cursor(ChaRevisionWo *revision);


ChaCursorMWo *cha_revision_wo_to_cursorm(const ChaRevisionWo *revision, const ChaCursorWo *cursor);

ChaCursorWo *cha_revision_wo_get_cursor(ChaRevisionWo *revision);
void cha_revision_wo_set_cursor(ChaRevisionWo *e_revision, ChaCursorWo *new_cursor);
ChaCursorWo *cha_revision_wo_get_editable_cursor(ChaRevisionWo *e_revision);

int cha_revision_wo_get_page_list_version(const ChaRevisionWo *revision);
int cha_revision_wo_page_count(const ChaRevisionWo *revision);
long long cha_revision_wo_count_lines(const ChaRevisionWo *revision);
long long cha_revision_wo_calculate_row(const ChaRevisionWo *revision, const ChaLineLocationWo *line_location);
ChaPageWo *cha_revision_wo_page_at(ChaRevisionWo *revision, int page_idx);
ChaPageWo *cha_revision_wo_editable_page_at(ChaRevisionWo *e_revision, int page_idx);

ChaLineWo *cha_revision_wo_line_at_location(const ChaRevisionWo *revision, const ChaLineLocationWo *line_location);

void cha_revision_wo_clear(ChaRevisionWo *e_revision);

gboolean cha_revision_wo_move_lines_to(ChaRevisionWo *e_revision, ChaLineLocationWo *ll_first, ChaLineLocationWo *ll_last, int delta);

gboolean cha_revision_wo_lines_remove(ChaRevisionWo *e_revision, long long first_row, long long last_row);

CatArrayWo *cha_revision_wo_get_lines_between_cursors(ChaRevisionWo *revision, ChaCursorMWo *start_cursor, ChaCursorMWo *end_cursor);
ChaLineLocationWo *cha_revision_wo_calculate_line_location(const ChaRevisionWo *revision, long long row);

void cha_revision_wo_append_page(ChaRevisionWo *e_revision, ChaPageWo *page);

void cha_revision_wo_set_form(ChaRevisionWo *e_revision, ChaFormWo *form);
ChaFormWo *cha_revision_wo_get_form(ChaRevisionWo *revision);
ChaFormWo *cha_revision_wo_get_editable_form(ChaRevisionWo *e_revision);

void cha_revision_wo_apply_form_changes(ChaRevisionWo *e_revision);


enum _ChaRevisionChangeFlags {
	CHA_REVISION_CF_CURSOR	= 1<<0,
	CHA_REVISION_CF_PAGES	= 1<<1,
	CHA_REVISION_CF_FORM	= 1<<2,
};

typedef enum _ChaRevisionChangeFlags ChaRevisionChangeFlags;

ChaRevisionChangeFlags cha_revision_wo_compare(const ChaRevisionWo *a_revision1, const ChaRevisionWo *a_revision2);

CAT_WO_BASE_H(ChaRevisionWo, cha_revision_wo)

G_END_DECLS

#endif /* CHAREVISIONWO_H_ */
