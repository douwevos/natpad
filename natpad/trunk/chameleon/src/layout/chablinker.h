/*
   File:    chablinker.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 26, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef LAYOUT_CHABLINKER_H_
#define LAYOUT_CHABLINKER_H_

#include <caterpillar.h>
#include "../chaeditor.h"

G_BEGIN_DECLS

#define CHA_TYPE_BLINKER              (cha_blinker_get_type())
#define CHA_BLINKER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_blinker_get_type(), ChaBlinker))
#define CHA_BLINKER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_BLINKER, ChaBlinkerClass))
#define CHA_IS_BLINKER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_BLINKER))
#define CHA_IS_BLINKER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_BLINKER))
#define CHA_BLINKER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_BLINKER, ChaBlinkerClass))


typedef struct _ChaBlinker               ChaBlinker;
typedef struct _ChaBlinkerPrivate        ChaBlinkerPrivate;
typedef struct _ChaBlinkerClass          ChaBlinkerClass;


struct _ChaBlinker {
	GObject parent;
};

struct _ChaBlinkerClass {
	GObjectClass parent_class;
};


GType cha_blinker_get_type();

ChaBlinker *cha_blinker_new();

void cha_blinker_set_active_editor(ChaBlinker *blinker, ChaEditor *active_editor);

void cha_blinker_inform_activity(ChaBlinker *blinker);

void cha_blinker_request_shutdown(ChaBlinker *blinker);

void cha_blinker_set_blinker_active(ChaBlinker *blinker, gboolean active);


G_END_DECLS

#endif /* LAYOUT_CHABLINKER_H_ */
