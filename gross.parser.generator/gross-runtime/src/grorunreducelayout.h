/*
   File:    grorunreducelayout.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Nov 3, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef GRORUNREDUCELAYOUT_H_
#define GRORUNREDUCELAYOUT_H_

#include "grorunsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_REDUCE_LAYOUT              (grorun_reduce_layout_get_type())
#define GRORUN_REDUCE_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_reduce_layout_get_type(), GroRunReduceLayout))
#define GRORUN_REDUCE_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_REDUCE_LAYOUT, GroRunReduceLayoutClass))
#define GRORUN_IS_REDUCE_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_REDUCE_LAYOUT))
#define GRORUN_IS_REDUCE_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_REDUCE_LAYOUT))
#define GRORUN_REDUCE_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_REDUCE_LAYOUT, GroRunReduceLayoutClass))


typedef struct _GroRunReduceLayout               GroRunReduceLayout;
typedef struct _GroRunReduceLayoutPrivate        GroRunReduceLayoutPrivate;
typedef struct _GroRunReduceLayoutClass          GroRunReduceLayoutClass;


struct _GroRunReduceLayout {
	GObject parent;
};

struct _GroRunReduceLayoutClass {
	GObjectClass parent_class;
};


GType grorun_reduce_layout_get_type();

GroRunReduceLayout *grorun_reduce_layout_new(GroRunSymbol *lhs, int rhs_count, CatIntArrayWo *nullified, int production_id);

GroRunSymbol *grorun_reduce_layout_get_lhs(GroRunReduceLayout *layout);

int grorun_reduce_layout_get_stack_rhs_count(GroRunReduceLayout *layout);

int grorun_reduce_layout_calculate_stack_offset(GroRunReduceLayout *layout, int offset);

int grorun_reduce_layout_get_production_id(GroRunReduceLayout *layout);

G_END_DECLS

#endif /* GRORUNREDUCELAYOUT_H_ */
