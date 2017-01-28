/*
   File:    leaevent.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 25, 2014
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

#ifndef LEAEVENT_H_
#define LEAEVENT_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_EVENT              (lea_event_get_type())
#define LEA_EVENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_event_get_type(), LeaEvent))
#define LEA_EVENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_EVENT, LeaEventClass))
#define LEA_IS_EVENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_EVENT))
#define LEA_IS_EVENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_EVENT))
#define LEA_EVENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_EVENT, LeaEventClass))


typedef struct _LeaEvent               LeaEvent;
typedef struct _LeaEventPrivate        LeaEventPrivate;
typedef struct _LeaEventClass          LeaEventClass;


struct _LeaEvent {
	GObject parent;
};

struct _LeaEventClass {
	GObjectClass parent_class;
};


GType lea_event_get_type();

void lea_event_construct(LeaEvent *event);

G_END_DECLS

#endif /* LEAEVENT_H_ */
