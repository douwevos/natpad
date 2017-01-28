/*
   File:    chaenrichmentdata.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 21, 2015
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

#ifndef CHAENRICHMENTDATA_H_
#define CHAENRICHMENTDATA_H_

#include "chaenrichmentdatamapwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_ENRICHMENT_DATA              (cha_enrichment_data_get_type())
#define CHA_ENRICHMENT_DATA(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_enrichment_data_get_type(), ChaEnrichmentData))
#define CHA_ENRICHMENT_DATA_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_ENRICHMENT_DATA, ChaEnrichmentDataClass))
#define CHA_IS_ENRICHMENT_DATA(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_ENRICHMENT_DATA))
#define CHA_IS_ENRICHMENT_DATA_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_ENRICHMENT_DATA))
#define CHA_ENRICHMENT_DATA_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_ENRICHMENT_DATA, ChaEnrichmentDataClass))


typedef struct _ChaEnrichmentData               ChaEnrichmentData;
typedef struct _ChaEnrichmentDataPrivate        ChaEnrichmentDataPrivate;
typedef struct _ChaEnrichmentDataClass          ChaEnrichmentDataClass;


struct _ChaEnrichmentData {
	GObject parent;
};

struct _ChaEnrichmentDataClass {
	GObjectClass parent_class;
};


GType cha_enrichment_data_get_type();

ChaEnrichmentData *cha_enrichment_data_new(ChaEnrichmentDataMapWo *a_enrichment_map);
ChaEnrichmentData *cha_enrichment_data_new_lock(ChaEnrichmentDataMapWo *a_enrichment_map, CatLock *lock);

CatLock *cha_enrichment_data_get_lock(ChaEnrichmentData *enrichment_data);

void cha_enrichment_data_remap(ChaEnrichmentData *enrichment_data, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);

ChaEnrichmentDataMapWo *cha_enrichment_data_get_map(const ChaEnrichmentData *enrichment_data);

GObject *cha_enrichment_data_get_slot_content_ref(const ChaEnrichmentData *enrichment_data, int slot_index, GObject *slot_key);
void cha_enrichment_data_set_slot_content(const ChaEnrichmentData *enrichment_data, int slot_index, GObject *slot_key, GObject *content);

G_END_DECLS

#endif /* CHAENRICHMENTDATA_H_ */
