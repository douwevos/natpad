/*
   File:    chalinewo.h
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

#ifndef CHALINEWO_H_
#define CHALINEWO_H_

#include "chaenrichmentdata.h"
#include <caterpillar.h>

G_BEGIN_DECLS


typedef enum {
	CHA_LINE_END_NONE,
	CHA_LINE_END_CR,	/* 13 */
	CHA_LINE_END_LF,	/* 10 */
	CHA_LINE_END_CRLF,	/* 13 10 */
	CHA_LINE_END_LFCR,	/* 10 13 */

} ChaLineEnd;


#define CHA_TYPE_LINE_WO              (cha_line_wo_get_type())
#define CHA_LINE_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_line_wo_get_type(), ChaLineWo))
#define CHA_LINE_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_LINE_WO, ChaLineWoClass))
#define CHA_IS_LINE_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_LINE_WO))
#define CHA_IS_LINE_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_LINE_WO))
#define CHA_LINE_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_LINE_WO, ChaLineWoClass))


typedef struct _ChaLineWo               ChaLineWo;
typedef struct _ChaLineWoPrivate        ChaLineWoPrivate;
typedef struct _ChaLineWoClass          ChaLineWoClass;


struct _ChaLineWo {
	CatWo parent;
};

struct _ChaLineWoClass {
	CatWoClass parent_class;
};


GType cha_line_wo_get_type();

ChaLineWo *cha_line_wo_new();
ChaLineWo *cha_line_wo_new_with(CatStringWo *c_text, ChaLineEnd line_end);
ChaLineWo *cha_line_wo_new_anchored(CatStringWo *c_text, ChaLineEnd line_end);

void cha_line_wo_enrich(ChaLineWo *line, ChaEnrichmentDataMapWo *a_map, CatLock *lock);
void cha_line_wo_impoverish(ChaLineWo *line);
void cha_line_wo_enrichment_remap(ChaLineWo *line, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx);

GObject *cha_line_wo_get_slot_content_ref(ChaLineWo *line, int slot_index, GObject *slot_key);
void cha_line_wo_set_slot_content(ChaLineWo *line, int slot_index, GObject *slot_key, GObject *content);

CatStringWo *cha_line_wo_get_text(ChaLineWo *line);
void cha_line_wo_set_text(ChaLineWo *e_line, CatStringWo *c_text);
void cha_line_wo_set_line_end(ChaLineWo *e_line, ChaLineEnd line_end);

ChaLineEnd cha_line_wo_get_line_end(ChaLineWo *line);
int cha_line_wo_byte_count(const ChaLineWo *line);
int cha_line_wo_byte_count_real(const ChaLineWo *line);

ChaLineWo *cha_line_wo_create_editable(ChaLineWo *instance);
ChaLineWo *cha_line_wo_anchor(ChaLineWo *instance, int version);
ChaLineWo *cha_line_wo_get_original(ChaLineWo *instance);
gboolean cha_line_wo_is_anchored(const ChaLineWo *instance);

gboolean cha_line_wo_equal(const ChaLineWo *instance_a, const ChaLineWo *instance_b);
ChaLineWo *cha_line_wo_clone(const ChaLineWo *source, CatCloneDepth clone_depth);

void cha_line_end_to_string(ChaLineEnd line_end, CatStringWo *e_string);

G_END_DECLS

#endif /* CHALINEWO_H_ */
