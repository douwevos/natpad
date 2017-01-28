/*
   File:    leadigitpanel.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 20, 2009
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

#ifndef LEADIGITPANEL_H_
#define LEADIGITPANEL_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <caterpillar.h>
#include "../../src/layout/leapanel.h"
#include "../../src/leaframe.h"
#include "../../src/layout/leaipanelowner.h"

G_BEGIN_DECLS

#define LEA_TYPE_DIGIT_PANEL            (lea_digit_panel_get_type())
#define LEA_DIGIT_PANEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_digit_panel_get_type(), LeaDigitPanel))
#define LEA_DIGIT_PANEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_DIGIT_PANEL, LeaDigitPanelClass))
#define LEA_IS_DIGIT_PANEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_DIGIT_PANEL))
#define LEA_IS_DIGIT_PANEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_DIGIT_PANEL))
#define LEA_DIGIT_PANEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_DIGIT_PANEL, LeaDigitPanelClass))

typedef struct _LeaDigitPanel       LeaDigitPanel;
typedef struct _LeaDigitPanelClass  LeaDigitPanelClass;


struct _LeaDigitPanel {
	LeaPanel parent;
};


struct _LeaDigitPanelClass {
	LeaPanelClass parent_class;
};


GType lea_digit_panel_get_type(void);

LeaDigitPanel *lea_digit_panel_new(LeaIPanelOwner *panel_owner, LeaFrame *frame);

G_END_DECLS

#endif /* LEADIGITPANEL_H_ */
