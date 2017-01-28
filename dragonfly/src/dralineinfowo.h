/*
   File:    dralineinfo.h
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

#ifndef DRALINEINFO_H_
#define DRALINEINFO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_LINE_INFO_WO              (dra_line_info_wo_get_type())
#define DRA_LINE_INFO_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_line_info_wo_get_type(), DraLineInfoWo))
#define DRA_LINE_INFO_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_LINE_INFO_WO, DraLineInfoWoClass))
#define DRA_IS_LINE_INFO_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_LINE_INFO_WO))
#define DRA_IS_LINE_INFO_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_LINE_INFO_WO))
#define DRA_LINE_INFO_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_LINE_INFO_WO, DraLineInfoWoClass))


typedef struct _DraLineInfoWo               DraLineInfoWo;
typedef struct _DraLineInfoWoPrivate        DraLineInfoWoPrivate;
typedef struct _DraLineInfoWoClass          DraLineInfoWoClass;


struct _DraLineInfoWo {
	CatWo parent;
};

struct _DraLineInfoWoClass {
	CatWoClass parent_class;
};


GType dra_line_info_wo_get_type();

DraLineInfoWo *dra_line_info_wo_new();

CatArrayWo *dra_line_info_wo_get_ab_list(DraLineInfoWo *line_info);

void dra_line_info_wo_set_ab_list(DraLineInfoWo *e_line_info, CatArrayWo *new_ab_list);
void dra_line_info_wo_append_ab_list(DraLineInfoWo *e_line_info, CatArrayWo *new_ab_list);

CatArrayWo *dra_line_info_wo_get_line_tags(DraLineInfoWo *line_info);

void dra_line_info_wo_set_line_tags(DraLineInfoWo *e_line_info, CatArrayWo *new_line_tags);
void dra_line_info_wo_append_line_tags(DraLineInfoWo *e_line_info, CatArrayWo *new_line_tags);

CAT_WO_BASE_H(DraLineInfoWo,dra_line_info_wo);

G_END_DECLS

#endif /* DRALINEINFO_H_ */
