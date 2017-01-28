/*
   File:    cowpaneldescription.h
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#ifndef COWPANELDESCRIPTION_H_
#define COWPANELDESCRIPTION_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define COW_TYPE_PANEL_DESCRIPTION              (cow_panel_description_get_type())
#define COW_PANEL_DESCRIPTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cow_panel_description_get_type(), CowPanelDescription))
#define COW_PANEL_DESCRIPTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), COW_TYPE_PANEL_DESCRIPTION, CowPanelDescriptionClass))
#define COW_IS_PANEL_DESCRIPTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), COW_TYPE_PANEL_DESCRIPTION))
#define COW_IS_PANEL_DESCRIPTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), COW_TYPE_PANEL_DESCRIPTION))
#define COW_PANEL_DESCRIPTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), COW_TYPE_PANEL_DESCRIPTION, CowPanelDescriptionClass))


typedef struct _CowPanelDescription               CowPanelDescription;
typedef struct _CowPanelDescriptionPrivate        CowPanelDescriptionPrivate;
typedef struct _CowPanelDescriptionClass          CowPanelDescriptionClass;


struct _CowPanelDescription {
	GObject parent;
};

struct _CowPanelDescriptionClass {
	GObjectClass parent_class;
};


GType cow_panel_description_get_type();

void cow_panel_description_construct(CowPanelDescription *description, CatStringWo *c_entry_name);

CowPanelDescription *cow_panel_description_new(CatStringWo *c_entry_name);

CatStringWo *cow_panel_description_get_entry_name(CowPanelDescription *panel_descr);

G_END_DECLS

#endif /* COWPANELDESCRIPTION_H_ */
