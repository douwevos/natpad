/*
   File:    mooexplorer.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 10, 2013
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


#ifndef MOOEXPLORER_H_
#define MOOEXPLORER_H_

#include "render/moonoderenderregistry.h"
#include "moonodelayout.h"
#include "moolayoutmodel.h"
#include <gtk/gtk.h>
#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_EXPLORER              (moo_explorer_get_type())
#define MOO_EXPLORER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_explorer_get_type(), MooExplorer))
#define MOO_EXPLORER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_EXPLORER, MooExplorerClass))
#define MOO_IS_EXPLORER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_EXPLORER))
#define MOO_IS_EXPLORER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_EXPLORER))
#define MOO_EXPLORER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_EXPLORER, MooExplorerClass))
#define MOO_EXPLORER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_EXPLORER, MooExplorerPrivate))


typedef struct _MooExplorer               MooExplorer;
typedef struct _MooExplorerPrivate        MooExplorerPrivate;
typedef struct _MooExplorerClass          MooExplorerClass;


struct _MooExplorer {
	GtkDrawingArea parent;
	MooExplorerPrivate *priv;
};

struct _MooExplorerClass {
	GtkDrawingAreaClass parent_class;
	void (*activateNode)(MooExplorer *self, MooNodeLayout *node_layout);
};


GType moo_explorer_get_type();

void moo_explorer_construct(MooExplorer *moo_explorer, MooService *moo_service, MooNodeRenderRegistry *render_registry);


MooExplorer *moo_explorer_new(MooService *moo_service, MooNodeRenderRegistry *render_registry);

MooLayoutModel *moo_explorer_get_layout_model(MooExplorer *moo_explorer);

G_END_DECLS

#endif /* MOOEXPLORER_H_ */
