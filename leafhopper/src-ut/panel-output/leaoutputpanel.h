/*
   File:    leaoutputpanel.h
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 26, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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


#ifndef LEAOUTPUTPANEL_H_
#define LEAOUTPUTPANEL_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <caterpillar.h>
#include "../../src/layout/leapanel.h"
#include "../../src/layout/leaipanelowner.h"
#include "../../src/leaframe.h"

G_BEGIN_DECLS

#define LEA_TYPE_OUTPUT_PANEL            (lea_output_panel_get_type())
#define LEA_OUTPUT_PANEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_output_panel_get_type(), LeaOutputPanel))
#define LEA_OUTPUT_PANEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_OUTPUT_PANEL, LeaOutputPanelClass))
#define LEA_IS_OUTPUT_PANEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_OUTPUT_PANEL))
#define LEA_IS_OUTPUT_PANEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_OUTPUT_PANEL))
#define LEA_OUTPUT_PANEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_OUTPUT_PANEL, LeaOutputPanelClass))


typedef struct _LeaOutputPanel	   		LeaOutputPanel;
typedef struct _LeaOutputPanelClass	  	LeaOutputPanelClass;


struct _LeaOutputPanel {
	LeaPanel parent;
};

struct _LeaOutputPanelClass {
	LeaPanelClass parent_class;
};


GType lea_output_panel_get_type();

LeaOutputPanel *lea_output_panel_new(LeaIPanelOwner *panel_owner, LeaFrame *frame);

G_END_DECLS

#endif /* LEAOUTPUTPANEL_H_ */
