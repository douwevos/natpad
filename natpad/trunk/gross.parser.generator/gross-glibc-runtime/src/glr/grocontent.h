
/*
   File:    grocontent.h
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 24, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef GLR_GROCONTENT_H_
#define GLR_GROCONTENT_H_

#include "../groglibctypes.h"

G_BEGIN_DECLS

#define GRO_TYPE_CONTENT              (gro_content_get_type())
#define GRO_CONTENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_content_get_type(), GroContent))
#define GRO_CONTENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_CONTENT, GroContentClass))
#define GRO_IS_CONTENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_CONTENT))
#define GRO_IS_CONTENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_CONTENT))
#define GRO_CONTENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_CONTENT, GroContentClass))


typedef struct _GroContent               GroContent;
typedef struct _GroContentPrivate        GroContentPrivate;
typedef struct _GroContentClass          GroContentClass;


struct _GroContent {
	GObject parent;
};

struct _GroContentClass {
	GObjectClass parent_class;
};


GType gro_content_get_type();

GroContent *gro_content_new(GroContent *linked, GObject *value, long long begin_row, int begin_column, long long end_row, int end_column);

GroContent *gro_content_get_linked(GroContent *linked);
GroContent *gro_content_get_linked_at(GroContent *linked, int skip);

void gro_content_get_left(GroContent *content, long long *left_row, int *left_column);
void gro_content_get_right(GroContent *content, long long *right_row, int *right_column);

GObject *gro_content_get_value(GroContent *linked);

G_END_DECLS

#endif /* GLR_GROCONTENT_H_ */
