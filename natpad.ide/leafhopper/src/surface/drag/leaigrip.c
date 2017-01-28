/*
   File:    leaigrip.c
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

#include "leaigrip.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaIGrip"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(LeaIGrip, lea_igrip, G_TYPE_OBJECT);

static void lea_igrip_default_init (LeaIGripInterface *iface) {
	iface->isIdentical = NULL;
}

LeaSimilarity lea_igrip_is_identical(LeaIGrip *self, LeaIGrip *other) {
	if (self==other) {
		return LEA_SIMILARITY_COMPLETE;
	} else if (self==NULL || other==NULL) {
		return LEA_SIMILARITY_NONE;
	}
	g_return_val_if_fail(LEA_IS_IGRIP(self), LEA_SIMILARITY_NONE);
	return LEA_IGRIP_GET_INTERFACE(self)->isIdentical(self, other);
}

void lea_igrip_add_notify(LeaIGrip *self, struct _LeaFrame *frame) {
	g_return_if_fail(LEA_IS_IGRIP(self));
	LEA_IGRIP_GET_INTERFACE(self)->addNotify(self, frame);
}

void lea_igrip_remove_notify(LeaIGrip *self) {
	g_return_if_fail(LEA_IS_IGRIP(self));
	LEA_IGRIP_GET_INTERFACE(self)->removeNotify(self);
}

void lea_igrip_motion_event(LeaIGrip *self, GdkEventMotion *event) {
	g_return_if_fail(LEA_IS_IGRIP(self));
	LEA_IGRIP_GET_INTERFACE(self)->motionEvent(self, event);
}

void lea_igrip_release_event(LeaIGrip *self, GdkEventButton *event) {
	g_return_if_fail(LEA_IS_IGRIP(self));
	LEA_IGRIP_GET_INTERFACE(self)->releaseEvent(self, event);
}

