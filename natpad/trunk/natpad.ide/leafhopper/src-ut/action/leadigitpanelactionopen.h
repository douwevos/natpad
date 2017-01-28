/*
   File:    leadigitpanelactionopen.h
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 24, 2009
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


#ifndef LEADIGITPANELACTIONOPEN_H_
#define LEADIGITPANELACTIONOPEN_H_

#include <caterpillar.h>
#include "../../src/action/leaaction.h"

G_BEGIN_DECLS

#define LEA_TYPE_DIGIT_PANEL_ACTION_OPEN            (lea_digit_panel_action_open_get_type())
#define LEA_DIGIT_PANEL_ACTION_OPEN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_digit_panel_action_open_get_type(), LeaDigitPanelActionOpen))
#define LEA_DIGIT_PANEL_ACTION_OPEN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_DIGIT_PANEL_ACTION_OPEN, LeaDigitPanelActionOpenClass))
#define LEA_IS_DIGIT_PANEL_ACTION_OPEN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_DIGIT_PANEL_ACTION_OPEN))
#define LEA_IS_DIGIT_PANEL_ACTION_OPEN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_DIGIT_PANEL_ACTION_OPEN))
#define LEA_DIGIT_PANEL_ACTION_OPEN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_DIGIT_PANEL_ACTION_OPEN, LeaDigitPanelActionOpenClass))


typedef struct _LeaDigitPanelActionOpen	   		LeaDigitPanelActionOpen;
typedef struct _LeaDigitPanelActionOpenClass	  	LeaDigitPanelActionOpenClass;


struct _LeaDigitPanelActionOpen {
	LeaAction parent;
};

struct _LeaDigitPanelActionOpenClass {
	LeaActionClass parent_class;
};


GType lea_digit_panel_action_open_get_type();

LeaDigitPanelActionOpen *lea_digit_panel_action_open_new();

G_END_DECLS


#endif /* LEADIGITPANELACTIONOPEN_H_ */
