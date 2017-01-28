/*
   File:    chapangoattrholder.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 22, 2015
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

#ifndef CHAPANGOATTRHOLDER_H_
#define CHAPANGOATTRHOLDER_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_PANGO_ATTR_HOLDER              (cha_pango_attr_holder_get_type())
#define CHA_PANGO_ATTR_HOLDER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_pango_attr_holder_get_type(), ChaPangoAttrHolder))
#define CHA_PANGO_ATTR_HOLDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PANGO_ATTR_HOLDER, ChaPangoAttrHolderClass))
#define CHA_IS_PANGO_ATTR_HOLDER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PANGO_ATTR_HOLDER))
#define CHA_IS_PANGO_ATTR_HOLDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PANGO_ATTR_HOLDER))
#define CHA_PANGO_ATTR_HOLDER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PANGO_ATTR_HOLDER, ChaPangoAttrHolderClass))


typedef struct _ChaPangoAttrHolder               ChaPangoAttrHolder;
typedef struct _ChaPangoAttrHolderPrivate        ChaPangoAttrHolderPrivate;
typedef struct _ChaPangoAttrHolderClass          ChaPangoAttrHolderClass;


struct _ChaPangoAttrHolder {
	GObject parent;
};

struct _ChaPangoAttrHolderClass {
	GObjectClass parent_class;
};


GType cha_pango_attr_holder_get_type();

ChaPangoAttrHolder *cha_pango_attr_holder_new(PangoAttribute *pango_attr);

PangoAttribute *cha_pango_attr_holder_get(ChaPangoAttrHolder *holder);

gboolean cha_pango_attr_holder_equal(const ChaPangoAttrHolder *holder_a, const ChaPangoAttrHolder *holder_b);

G_END_DECLS

#endif /* CHAPANGOATTRHOLDER_H_ */
