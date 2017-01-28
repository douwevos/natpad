/*
   File:    leaiattachable.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jul 31, 2009
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


#ifndef LEAIATTACHABLE_H_
#define LEAIATTACHABLE_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <caterpillar.h>

#define LEA_TYPE_IATTACHABLE                 (lea_iattachable_get_type ())
#define LEA_IATTACHABLE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), LEA_TYPE_IATTACHABLE, LeaIAttachable))
#define LEA_IS_IATTACHABLE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_IATTACHABLE))
#define LEA_IATTACHABLE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), LEA_TYPE_IATTACHABLE, LeaIAttachableInterface))


typedef struct _LeaIAttachable               LeaIAttachable; /* dummy object */
typedef struct _LeaIAttachableInterface      LeaIAttachableInterface;

struct _LeaIAttachableInterface {
	GTypeInterface parent_iface;
	void (*sensitivitySet)(LeaIAttachable *self, gboolean sensitivity);
	void (*visibilitySet)(LeaIAttachable *self, gboolean visibility);
};

GType lea_iattachable_get_type(void);

void lea_iattachable_sensitivity_set(LeaIAttachable *self, gboolean sensitivity);
void lea_iattachable_visibility_set(LeaIAttachable *self, gboolean visibility);


#endif /* LEAIATTACHABLE_H_ */
