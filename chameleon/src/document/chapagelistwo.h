/*
   File:    chapagelistwo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 25, 2015
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

#ifndef CHAPAGELISTWO_H_
#define CHAPAGELISTWO_H_

#include "chapagewo.h"
#include "chalinelocationwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_PAGE_LIST_WO              (cha_page_list_wo_get_type())
#define CHA_PAGE_LIST_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_page_list_wo_get_type(), ChaPageListWo))
#define CHA_PAGE_LIST_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PAGE_LIST_WO, ChaPageListWoClass))
#define CHA_IS_PAGE_LIST_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PAGE_LIST_WO))
#define CHA_IS_PAGE_LIST_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PAGE_LIST_WO))
#define CHA_PAGE_LIST_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PAGE_LIST_WO, ChaPageListWoClass))


typedef struct _ChaPageListWo               ChaPageListWo;
typedef struct _ChaPageListWoPrivate        ChaPageListWoPrivate;
typedef struct _ChaPageListWoClass          ChaPageListWoClass;


struct _ChaPageListWo {
	CatWo parent;
};

struct _ChaPageListWoClass {
	CatWoClass parent_class;
};


GType cha_page_list_wo_get_type();

ChaPageListWo *cha_page_list_wo_new();

void cha_page_list_wo_enrich(ChaPageListWo *page_list, ChaEnrichmentDataMapWo *a_map);
void cha_page_list_wo_impoverish(ChaPageListWo *page_list);
void cha_page_list_wo_enrichment_remap(ChaPageListWo *page_list, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);

long long cha_page_list_wo_calculate_row(const ChaPageListWo *page_list, const ChaLineLocationWo *line_location);
long long cha_page_list_wo_page_line_count(ChaPageListWo *page_list);
long long cha_page_list_wo_line_count_till_page(const ChaPageListWo *page_list, int page_index);


ChaLineLocationWo *cha_page_list_wo_calculate_line_location(const ChaPageListWo *page_list, long long row);
ChaLineLocationWo *cha_page_list_wo_calculate_line_location_end(const ChaPageListWo *page_list);


int cha_page_list_wo_page_count(ChaPageListWo *page_list);

ChaPageWo *cha_page_list_wo_page_at(ChaPageListWo *page_list, int page_index);
ChaPageWo *cha_page_list_wo_editable_page_at(ChaPageListWo *e_page_list, int page_index);

void cha_page_list_wo_add_page(ChaPageListWo *e_page_list, ChaPageWo *page);
void cha_page_list_wo_add_page_at(ChaPageListWo *e_page_list, ChaPageWo *page, int index);
void cha_page_list_wo_set_page_at(ChaPageListWo *e_page_list, ChaPageWo *page, int index);
void cha_page_list_wo_remove_page_at(ChaPageListWo *e_page_list, int index);
void cha_page_list_wo_remove_range(ChaPageListWo *e_page_list, int first, int last);


ChaLineEnd cha_page_list_wo_get_line_ends(const ChaPageListWo *page_list);
gboolean cha_page_list_wo_get_line_ends_are_mixed(const ChaPageListWo *page_list) ;
void cha_page_list_wo_set_line_ends(ChaPageListWo *e_page_list, ChaLineEnd line_ends, gboolean line_ends_are_mixed);

void cha_page_list_wo_clear(ChaPageListWo *e_page_list);

ChaPageListWo *cha_page_list_wo_create_editable(ChaPageListWo *instance);
ChaPageListWo *cha_page_list_wo_anchor(ChaPageListWo *instance, int version);
ChaPageListWo *cha_page_list_wo_get_original(ChaPageListWo *instance);
gboolean cha_page_list_wo_is_anchored(const ChaPageListWo *instance);

gboolean cha_page_list_wo_equal(const ChaPageListWo *instance_a, const ChaPageListWo *instance_b);
ChaPageListWo *cha_page_list_wo_clone(const ChaPageListWo *source, CatCloneDepth clone_depth);

void cha_page_list_wo_dump(const ChaPageListWo *list, CatStringWo *e_buf);

G_END_DECLS

#endif /* CHAPAGELISTWO_H_ */
