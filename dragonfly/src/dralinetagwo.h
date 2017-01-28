/*
   File:    dralinetagwo.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jul 7, 2015
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

#ifndef DRALINETAGWO_H_
#define DRALINETAGWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_LINE_TAG_WO              (dra_line_tag_wo_get_type())
#define DRA_LINE_TAG_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_line_tag_wo_get_type(), DraLineTagWo))
#define DRA_LINE_TAG_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_LINE_TAG_WO, DraLineTagWoClass))
#define DRA_IS_LINE_TAG_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_LINE_TAG_WO))
#define DRA_IS_LINE_TAG_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_LINE_TAG_WO))
#define DRA_LINE_TAG_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_LINE_TAG_WO, DraLineTagWoClass))


typedef struct _DraLineTagWo               DraLineTagWo;
typedef struct _DraLineTagWoPrivate        DraLineTagWoPrivate;
typedef struct _DraLineTagWoClass          DraLineTagWoClass;


struct _DraLineTagWo {
	CatWo parent;
};

struct _DraLineTagWoClass {
	CatWoClass parent_class;
};

typedef enum _DraTagType DraTagType;

enum _DraTagType {
	DRA_TAG_TYPE_PARSER_ERROR,
	DRA_TAG_TYPE_SCANNER_ERROR,
	DRA_TAG_TYPE_SPELL_ERROR
};

GType dra_line_tag_wo_get_type();

DraLineTagWo *dra_line_tag_wo_new(long long row, DraTagType tag_type);

void dra_line_tag_wo_get_color(DraLineTagWo *line_tag, double *red, double *green, double *blue);
void dra_line_tag_wo_set_color(DraLineTagWo *e_line_tag, double red, double green, double blue);

void dra_line_tag_wo_get_start_and_end_index(DraLineTagWo *line_tag, int *start, int *end);
void dra_line_tag_wo_set_end_index(DraLineTagWo *e_line_tag, int right);
void dra_line_tag_wo_set_start_and_end_index(DraLineTagWo *e_line_tag, int left, int right);


void dra_line_tag_wo_set_text(DraLineTagWo *e_line_tag, CatStringWo *text);
CatStringWo *dra_line_tag_wo_get_text(DraLineTagWo *e_line_tag);

void dra_line_tag_wo_set_extra_data(DraLineTagWo *e_line_tag, GObject *extra_data);
GObject *dra_line_tag_wo_get_extra_data(DraLineTagWo *e_line_tag);


DraTagType dra_line_tag_wo_get_tag_type(DraLineTagWo *line_tag);
long long dra_line_tag_wo_get_row(DraLineTagWo *line_tag);

CAT_WO_BASE_H(DraLineTagWo,dra_line_tag_wo);

G_END_DECLS

#endif /* DRALINETAGWO_H_ */
