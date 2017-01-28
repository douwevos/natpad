/*
   File:    leaactionscientific.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 7, 2013
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

#ifndef LEAACTIONSCIENTIFIC_H_
#define LEAACTIONSCIENTIFIC_H_

#include "../../../src/action/leaaction.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_ACTION_SCIENTIFIC              (lea_action_scientific_get_type())
#define LEA_ACTION_SCIENTIFIC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_action_scientific_get_type(), LeaActionScientific))
#define LEA_ACTION_SCIENTIFIC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_ACTION_SCIENTIFIC, LeaActionScientificClass))
#define LEA_IS_ACTION_SCIENTIFIC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_ACTION_SCIENTIFIC))
#define LEA_IS_ACTION_SCIENTIFIC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_ACTION_SCIENTIFIC))
#define LEA_ACTION_SCIENTIFIC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_ACTION_SCIENTIFIC, LeaActionScientificClass))
#define LEA_ACTION_SCIENTIFIC_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_ACTION_SCIENTIFIC, LeaActionScientificPrivate))


typedef struct _LeaActionScientific               LeaActionScientific;
typedef struct _LeaActionScientificPrivate        LeaActionScientificPrivate;
typedef struct _LeaActionScientificClass          LeaActionScientificClass;


struct _LeaActionScientific {
	LeaAction parent;
	LeaActionScientificPrivate *priv;
};

struct _LeaActionScientificClass {
	LeaActionClass parent_class;
};


GType lea_action_scientific_get_type();

LeaActionScientific *lea_action_scientific_new();

G_END_DECLS

#endif /* LEAACTIONSCIENTIFIC_H_ */
