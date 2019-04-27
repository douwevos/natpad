/*
   File:    moolayoutmodel.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Dec 25, 2013
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

#ifndef MOOLAYOUTMODEL_H_
#define MOOLAYOUTMODEL_H_

#include "moonodelayout.h"
#include "mooilayoutmodellistener.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MOO_TYPE_LAYOUT_MODEL              (moo_layout_model_get_type())
#define MOO_LAYOUT_MODEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_layout_model_get_type(), MooLayoutModel))
#define MOO_LAYOUT_MODEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_LAYOUT_MODEL, MooLayoutModelClass))
#define MOO_IS_LAYOUT_MODEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_LAYOUT_MODEL))
#define MOO_IS_LAYOUT_MODEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_LAYOUT_MODEL))
#define MOO_LAYOUT_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_LAYOUT_MODEL, MooLayoutModelClass))

typedef struct _MooLayoutModel               MooLayoutModel;
typedef struct _MooLayoutModelPrivate        MooLayoutModelPrivate;
typedef struct _MooLayoutModelClass          MooLayoutModelClass;

struct _MooLayoutModel {
	GObject parent;
};

struct _MooLayoutModelClass {
	GObjectClass parent_class;
};

GType moo_layout_model_get_type();

MooLayoutModel *moo_layout_model_new(MooNodeRenderRegistry *render_registry);

MooNodeLayout *moo_layout_model_node_at(MooLayoutModel *layout_model, int xpos, int ypos);

void moo_layout_model_get_view_size(MooLayoutModel *layout_model, int *width, int *height);

void moo_layout_model_add_listener(MooLayoutModel *layout_model, MooILayoutModelListener *model_listener);

void moo_layout_model_remove_listener(MooLayoutModel *layout_model, MooILayoutModelListener *model_listener);

G_END_DECLS

#endif /* MOOLAYOUTMODEL_H_ */
