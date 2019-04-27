/*
   File:    catunichar.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 2, 2010
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

#ifndef CATUNICHAR_H_
#define CATUNICHAR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_UNICHAR            (cat_unichar_get_type())
#define CAT_UNICHAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_unichar_get_type(), CatUnichar))
#define CAT_UNICHAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_UNICHAR, CatUnicharClass))
#define CAT_IS_UNICHAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_UNICHAR))
#define CAT_IS_UNICHAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_UNICHAR))
#define CAT_UNICHAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_UNICHAR, CatUnicharClass))


typedef struct _CatUnichar	       CatUnichar;
typedef struct _CatUnicharPrivate  CatUnicharPrivate;
typedef struct _CatUnicharClass	   CatUnicharClass;

struct _CatUnichar {
	GObject parent;
};

struct _CatUnicharClass {
	GObjectClass parent_class;
};

CatUnichar *cat_unichar_new(gunichar uch);

int cat_unichar_hash(const CatUnichar *unichar);

gboolean cat_unichar_equal(const CatUnichar *unichar_a, const CatUnichar *unichar_b);


G_END_DECLS

#endif /* CATUNICHAR_H_ */
