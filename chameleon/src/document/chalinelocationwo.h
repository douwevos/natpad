/*
   File:    chalinelocationwo.h
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

#ifndef CHALINELOCATIONWO_H_
#define CHALINELOCATIONWO_H_

#include "chalinewo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_LINE_LOCATION_WO              (cha_line_location_wo_get_type())
#define CHA_LINE_LOCATION_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_line_location_wo_get_type(), ChaLineLocationWo))
#define CHA_LINE_LOCATION_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_LINE_LOCATION_WO, ChaLineLocationWoClass))
#define CHA_IS_LINE_LOCATION_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_LINE_LOCATION_WO))
#define CHA_IS_LINE_LOCATION_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_LINE_LOCATION_WO))
#define CHA_LINE_LOCATION_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_LINE_LOCATION_WO, ChaLineLocationWoClass))


typedef struct _ChaLineLocationWo               ChaLineLocationWo;
typedef struct _ChaLineLocationWoPrivate        ChaLineLocationWoPrivate;
typedef struct _ChaLineLocationWoClass          ChaLineLocationWoClass;

struct _ChaRevisionWo;

struct _ChaLineLocationWo {
	CatWo parent;
};

struct _ChaLineLocationWoClass {
	CatWoClass parent_class;
};


GType cha_line_location_wo_get_type();

ChaLineLocationWo *cha_line_location_wo_new();
ChaLineLocationWo *cha_line_location_wo_new_with(int page_index, int page_line_index);

int cha_line_location_wo_get_page_index(const ChaLineLocationWo *line_location);
int cha_line_location_wo_get_page_line_index(const ChaLineLocationWo *line_location);

void cha_line_location_wo_set_page_index(ChaLineLocationWo *e_line_location, int new_index);
void cha_line_location_wo_set_page_line_index(ChaLineLocationWo *e_line_location, int new_index);
void cha_line_location_wo_add_to_page_line_index(ChaLineLocationWo *e_line_location, int to_add);

gboolean cha_line_location_wo_bind_to_revision(ChaLineLocationWo *e_line_location, struct _ChaRevisionWo *revision);

ChaLineLocationWo *cha_line_location_wo_create_editable(ChaLineLocationWo *instance);
ChaLineLocationWo *cha_line_location_wo_anchor(ChaLineLocationWo *instance, int version);
ChaLineLocationWo *cha_line_location_wo_get_original(ChaLineLocationWo *instance);
gboolean cha_line_location_wo_is_anchored(const ChaLineLocationWo *instance);

gboolean cha_line_location_wo_equal(const ChaLineLocationWo *instance_a, const ChaLineLocationWo *instance_b);
int cha_line_location_wo_compare(const ChaLineLocationWo *instance_a, const ChaLineLocationWo *instance_b);
ChaLineLocationWo *cha_line_location_wo_clone(const ChaLineLocationWo *source, CatCloneDepth clone_depth);

G_END_DECLS

#endif /* CHALINELOCATIONWO_H_ */
