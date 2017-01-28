/*
   File:    leadigitpanelactiondigitenter.h
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


#ifndef LEADIGITPANELACTIONDIGITENTER_H_
#define LEADIGITPANELACTIONDIGITENTER_H_

#include <caterpillar.h>
#include "../../src/action/leaaction.h"
#include "../../src/leaframe.h"


G_BEGIN_DECLS

#define LEA_TYPE_DIGIT_PANEL_ACTION_DIGIT_ENTER            (lea_digit_panel_action_digit_enter_get_type())
#define LEA_DIGIT_PANEL_ACTION_DIGIT_ENTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_digit_panel_action_digit_enter_get_type(), LeaDigitPanelActionDigitEnter))
#define LEA_DIGIT_PANEL_ACTION_DIGIT_ENTER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_DIGIT_PANEL_ACTION_DIGIT_ENTER, LeaDigitPanelActionDigitEnterClass))
#define LEA_IS_DIGIT_PANEL_ACTION_DIGIT_ENTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_DIGIT_PANEL_ACTION_DIGIT_ENTER))
#define LEA_IS_DIGIT_PANEL_ACTION_DIGIT_ENTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_DIGIT_PANEL_ACTION_DIGIT_ENTER))
#define LEA_DIGIT_PANEL_ACTION_DIGIT_ENTER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_DIGIT_PANEL_ACTION_DIGIT_ENTER, LeaDigitPanelActionDigitEnterClass))


typedef struct _LeaDigitPanelActionDigitEnter	   		LeaDigitPanelActionDigitEnter;
typedef struct _LeaDigitPanelActionDigitEnterClass	  	LeaDigitPanelActionDigitEnterClass;


struct _LeaDigitPanelActionDigitEnter {
	LeaAction parent;
	int digit;
	LeaFrame *frame;
};

struct _LeaDigitPanelActionDigitEnterClass {
	LeaActionClass parent_class;
};


GType lea_digit_panel_action_digit_enter_get_type();

LeaDigitPanelActionDigitEnter *lea_digit_panel_action_digit_enter_new(LeaFrame *frame, int digit);

G_END_DECLS


#endif /* LEADIGITPANELACTIONDIGITENTER_H_ */
