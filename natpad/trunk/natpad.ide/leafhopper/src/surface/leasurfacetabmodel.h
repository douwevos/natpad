/*
   File:    leasurfacetabmodel.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 18, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef LEASURFACETABMODEL_H_
#define LEASURFACETABMODEL_H_

#include "leasurfacetab.h"
#include "leaitabmodellistener.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_SURFACE_TAB_MODEL              (lea_surface_tab_model_get_type())
#define LEA_SURFACE_TAB_MODEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_surface_tab_model_get_type(), LeaSurfaceTabModel))
#define LEA_SURFACE_TAB_MODEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SURFACE_TAB_MODEL, LeaSurfaceTabModelClass))
#define LEA_IS_SURFACE_TAB_MODEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SURFACE_TAB_MODEL))
#define LEA_IS_SURFACE_TAB_MODEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SURFACE_TAB_MODEL))
#define LEA_SURFACE_TAB_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SURFACE_TAB_MODEL, LeaSurfaceTabModelClass))


typedef struct _LeaSurfaceTabModel               LeaSurfaceTabModel;
typedef struct _LeaSurfaceTabModelPrivate        LeaSurfaceTabModelPrivate;
typedef struct _LeaSurfaceTabModelClass          LeaSurfaceTabModelClass;


struct _LeaSurfaceTabModel {
	GObject parent;
};

struct _LeaSurfaceTabModelClass {
	GObjectClass parent_class;
};


GType lea_surface_tab_model_get_type();

LeaSurfaceTabModel *lea_surface_tab_model_new();

void lea_surface_tab_model_add_tab(LeaSurfaceTabModel *tab_model, LeaSurfaceTab *tab);
void lea_surface_tab_model_add_tab_at(LeaSurfaceTabModel *tab_model, LeaSurfaceTab *tab, int index);

void lea_surface_tab_model_remove_tab_at(LeaSurfaceTabModel *tab_model, int idx);

LeaSurfaceTab *lea_surface_tab_model_get_tab_at(LeaSurfaceTabModel *tab_model, int idx);
int lea_surface_tab_model_count(LeaSurfaceTabModel *tab_model);
CatIIterator *lea_surface_tab_model_iterator(LeaSurfaceTabModel *tab_model);

int lea_surface_tab_model_find_tab_owning_widget(LeaSurfaceTabModel *tab_model, GtkWidget *widget_on_tab);

LeaSurfaceTab *lea_surface_tab_model_get_selected_tab(LeaSurfaceTabModel *tab_model);
int lea_surface_tab_model_get_selected_tab_index(LeaSurfaceTabModel *tab_model);
void lea_surface_tab_model_set_selected_tab(LeaSurfaceTabModel *tab_model, int index);

void lea_surface_tab_model_add_listener(LeaSurfaceTabModel *tab_model, LeaITabModelListener *listener);
void lea_surface_tab_model_remove_listener(LeaSurfaceTabModel *tab_model, LeaITabModelListener *listener);


void lea_surface_tab_model_destroy(LeaSurfaceTabModel *tab_model);

G_END_DECLS


#endif /* LEASURFACETABMODEL_H_ */
