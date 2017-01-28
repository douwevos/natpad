/*
   File:    elkdefaultmapper.h
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 3, 2014
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

#ifndef ELKDEFAULTMAPPER_H_
#define ELKDEFAULTMAPPER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_DEFAULT_MAPPER              (elk_default_mapper_get_type())
#define ELK_DEFAULT_MAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_default_mapper_get_type(), ElkDefaultMapper))
#define ELK_DEFAULT_MAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_DEFAULT_MAPPER, ElkDefaultMapperClass))
#define ELK_IS_DEFAULT_MAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_DEFAULT_MAPPER))
#define ELK_IS_DEFAULT_MAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_DEFAULT_MAPPER))
#define ELK_DEFAULT_MAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_DEFAULT_MAPPER, ElkDefaultMapperClass))


typedef struct _ElkDefaultMapper               ElkDefaultMapper;
typedef struct _ElkDefaultMapperPrivate        ElkDefaultMapperPrivate;
typedef struct _ElkDefaultMapperClass          ElkDefaultMapperClass;


struct _ElkDefaultMapper {
	GObject parent;
};

struct _ElkDefaultMapperClass {
	GObjectClass parent_class;
};


GType elk_default_mapper_get_type();

ElkDefaultMapper *elk_default_mapper_new();

G_END_DECLS

#endif /* ELKDEFAULTMAPPER_H_ */
