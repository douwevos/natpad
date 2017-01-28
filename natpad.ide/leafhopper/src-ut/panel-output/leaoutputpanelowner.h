/*
   File:    leaoutputpanelowner.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 13, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#ifndef LEAOUTPUTPANELOWNER_H_
#define LEAOUTPUTPANELOWNER_H_

#include <caterpillar.h>
#include "../../src/leaframe.h"

G_BEGIN_DECLS

#define LEA_TYPE_OUTPUT_PANEL_OWNER            (lea_output_panel_owner_get_type())
#define LEA_OUTPUT_PANEL_OWNER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_output_panel_owner_get_type(), LeaOutputPanelOwner))
#define LEA_OUTPUT_PANEL_OWNER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_OUTPUT_PANEL_OWNER, LeaOutputPanelOwnerClass))
#define LEA_IS_OUTPUT_PANEL_OWNER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_OUTPUT_PANEL_OWNER))
#define LEA_IS_OUTPUT_PANEL_OWNER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_OUTPUT_PANEL_OWNER))
#define LEA_OUTPUT_PANEL_OWNER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_OUTPUT_PANEL_OWNER, LeaOutputPanelOwnerClass))


typedef struct _LeaOutputPanelOwner	   		    LeaOutputPanelOwner;
typedef struct _LeaOutputPanelOwnerClass	  	LeaOutputPanelOwnerClass;


struct _LeaOutputPanelOwner {
	GObject parent;
	LeaFrame *frame;
	CatArrayWo *e_panel_list;
	LeaActionGroup *action_group;
};

struct _LeaOutputPanelOwnerClass {
	GObjectClass parent_class;
};

GType lea_output_panel_owner_get_type();

LeaOutputPanelOwner *lea_output_panel_owner_new();

G_END_DECLS

#endif /* LEAOUTPUTPANELOWNER_H_ */
