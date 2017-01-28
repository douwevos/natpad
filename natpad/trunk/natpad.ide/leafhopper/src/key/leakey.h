/*
   File:    leakey.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 13, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#ifndef LEAKEY_H_
#define LEAKEY_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

extern CatS lea_s_key_modifier_shift;
extern CatS lea_s_key_modifier_control;
extern CatS lea_s_key_modifier_alt;
extern CatS lea_s_key_modifier_super;

#define LEA_TYPE_KEY              (lea_key_get_type())
#define LEA_KEY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_key_get_type(), LeaKey))
#define LEA_KEY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_KEY, LeaKeyClass))
#define LEA_IS_KEY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_KEY))
#define LEA_IS_KEY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_KEY))
#define LEA_KEY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_KEY, LeaKeyClass))
#define LEA_KEY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_KEY, LeaKeyPrivate))


typedef struct _LeaKey              LeaKey;
typedef struct _LeaKeyClass	   	    LeaKeyClass;
typedef struct _LeaKeyPrivate       LeaKeyPrivate;

#define LEA_KM_SHIFT_DOWN		0x00001
#define LEA_KM_SHIFT_UP			0x00002
#define LEA_KM_SHIFT			0x00004
#define LEA_KM_CONTROL_DOWN		0x00010
#define LEA_KM_CONTROL_UP		0x00020
#define LEA_KM_CONTROL			0x00040
#define LEA_KM_ALT_DOWN			0x00100
#define LEA_KM_ALT_UP			0x00200
#define LEA_KM_ALT				0x00400
#define LEA_KM_SUPER_DOWN		0x01000
#define LEA_KM_SUPER_UP			0x02000
#define LEA_KM_SUPER			0x04000

struct _LeaKey {
	GObject parent;
};

struct _LeaKeyClass {
	GObjectClass parent_class;
};


GType lea_key_get_type();

LeaKey *lea_key_new(int key, int modifiers);

LeaKey *lea_key_from_gkd_event(GdkEventKey *event, gboolean is_pressed);

LeaKey *lea_key_from_string(CatStringWo *a_txt);

CatStringWo *lea_key_get_key_text(LeaKey *key);
CatStringWo *lea_key_to_string(LeaKey *key);

gboolean lea_key_equal(LeaKey *key_a, LeaKey *key_b);
int lea_key_hashcode(LeaKey *key);

G_END_DECLS

#endif /* LEAKEY_H_ */
