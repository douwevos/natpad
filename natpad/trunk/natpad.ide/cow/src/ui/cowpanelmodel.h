/*
   File:    cowpanelmodel.h
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 27, 2014
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

#ifndef COWPANELMODEL_H_
#define COWPANELMODEL_H_

#include "cowipanelfactory.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define COW_TYPE_PANEL_MODEL              (cow_panel_model_get_type())
#define COW_PANEL_MODEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cow_panel_model_get_type(), CowPanelModel))
#define COW_PANEL_MODEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), COW_TYPE_PANEL_MODEL, CowPanelModelClass))
#define COW_IS_PANEL_MODEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), COW_TYPE_PANEL_MODEL))
#define COW_IS_PANEL_MODEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), COW_TYPE_PANEL_MODEL))
#define COW_PANEL_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), COW_TYPE_PANEL_MODEL, CowPanelModelClass))


typedef struct _CowPanelModel               CowPanelModel;
typedef struct _CowPanelModelPrivate        CowPanelModelPrivate;
typedef struct _CowPanelModelClass          CowPanelModelClass;


struct _CowPanelModel {
	GObject parent;
};

struct _CowPanelModelClass {
	GObjectClass parent_class;
};


GType cow_panel_model_get_type();

CowPanelModel *cow_panel_model_new();

void cow_panel_model_add_factory(CowPanelModel *model, CowIPanelFactory *panel_factory);
void cow_panel_model_remove_factory(CowPanelModel *model, CowIPanelFactory *panel_factory);

G_END_DECLS

#endif /* COWPANELMODEL_H_ */
