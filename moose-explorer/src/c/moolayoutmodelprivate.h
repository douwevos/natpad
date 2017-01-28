/*
   File:    moolayoutmodel.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 25, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef MOOLAYOUTMODELPRIVATE_H_
#define MOOLAYOUTMODELPRIVATE_H_

#include "moolayoutmodel.h"

G_BEGIN_DECLS

gboolean moo_layout_model_select(MooLayoutModel *layout_model, MooNodeLayout *nodeAt, gboolean tryToggleFirst, gboolean ctrlPressed, gboolean shiftPressed);

void moo_layout_model_paint(MooLayoutModel *layout_model, PangoContext *pango_context, cairo_t *cairo);

void moo_layout_model_set_root_node(MooLayoutModel *layout_model, MooNodeWo *node);

void moo_layout_model_validate(MooLayoutModel *layout_model, cairo_t *cairo, PangoContext *pango_context);

G_END_DECLS

#endif /* MOOLAYOUTMODELPRIVATE_H_ */
