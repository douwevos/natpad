/*
   File:    chaenrichmentdatamapwo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 22, 2015
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

#ifndef CHAENRICHMENTDATAMAPWO_H_
#define CHAENRICHMENTDATAMAPWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_ENRICHMENT_DATA_MAP_WO              (cha_enrichment_data_map_wo_get_type())
#define CHA_ENRICHMENT_DATA_MAP_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_enrichment_data_map_wo_get_type(), ChaEnrichmentDataMapWo))
#define CHA_ENRICHMENT_DATA_MAP_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_ENRICHMENT_DATA_MAP_WO, ChaEnrichmentDataMapWoClass))
#define CHA_IS_ENRICHMENT_DATA_MAP_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_ENRICHMENT_DATA_MAP_WO))
#define CHA_IS_ENRICHMENT_DATA_MAP_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_ENRICHMENT_DATA_MAP_WO))
#define CHA_ENRICHMENT_DATA_MAP_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_ENRICHMENT_DATA_MAP_WO, ChaEnrichmentDataMapWoClass))


typedef struct _ChaEnrichmentDataMapWo               ChaEnrichmentDataMapWo;
typedef struct _ChaEnrichmentDataMapWoPrivate        ChaEnrichmentDataMapWoPrivate;
typedef struct _ChaEnrichmentDataMapWoClass          ChaEnrichmentDataMapWoClass;


struct _ChaEnrichmentDataMapWo {
	CatWo parent;
};

struct _ChaEnrichmentDataMapWoClass {
	CatWoClass parent_class;
};


GType cha_enrichment_data_map_wo_get_type();

ChaEnrichmentDataMapWo *cha_enrichment_data_map_wo_new();

CatArrayWo *cha_enrichment_data_map_wo_get_default_nullified_data(const ChaEnrichmentDataMapWo *enrichment_map);

int cha_enrichment_data_map_wo_count(const ChaEnrichmentDataMapWo *enrichment_map);
int cha_enrichment_data_map_wo_find_slot_index(const ChaEnrichmentDataMapWo *enrichment_map, GObject *slot_key, int guess_index);

int cha_enrichment_data_map_wo_add_slot(ChaEnrichmentDataMapWo *e_enrichment_map, GObject *slot_key);
int cha_enrichment_data_map_wo_remove_slot(ChaEnrichmentDataMapWo *e_enrichment_map, GObject *slot_key);

CAT_WO_BASE_H(ChaEnrichmentDataMapWo, cha_enrichment_data_map_wo)

G_END_DECLS

#endif /* CHAENRICHMENTDATAMAPWO_H_ */
