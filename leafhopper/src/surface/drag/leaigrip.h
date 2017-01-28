/*
   File:    leaigrip.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 18, 2014
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

#ifndef LEAIGRIP_H_
#define LEAIGRIP_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS


typedef enum {
	LEA_SIMILARITY_NONE,
	LEA_SIMILARITY_SAME_ITEM,
	LEA_SIMILARITY_COMPLETE,
} LeaSimilarity;

#define LEA_TYPE_IGRIP                 (lea_igrip_get_type())
#define LEA_IGRIP(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IGRIP, LeaIGrip))
#define LEA_IS_IGRIP(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IGRIP))
#define LEA_IGRIP_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IGRIP, LeaIGripInterface))

typedef struct _LeaIGrip               LeaIGrip; /* dummy object */
typedef struct _LeaIGripInterface      LeaIGripInterface;

struct _LeaFrame;

struct _LeaIGripInterface {
	GTypeInterface parent_iface;

	LeaSimilarity (*isIdentical)(LeaIGrip *self, LeaIGrip *other);
	void (*addNotify)(LeaIGrip *self, struct _LeaFrame *frame);
	void (*removeNotify)(LeaIGrip *self);
	void (*motionEvent)(LeaIGrip *self, GdkEventMotion *event);
	void (*releaseEvent)(LeaIGrip *self, GdkEventButton *event);
};

GType lea_igrip_get_type(void);

LeaSimilarity lea_igrip_is_identical(LeaIGrip *self, LeaIGrip *other);
void lea_igrip_add_notify(LeaIGrip *self, struct _LeaFrame *frame);
void lea_igrip_remove_notify(LeaIGrip *self);
void lea_igrip_motion_event(LeaIGrip *self, GdkEventMotion *event);
void lea_igrip_release_event(LeaIGrip *self, GdkEventButton *event);

G_END_DECLS

#endif /* LEAIGRIP_H_ */
