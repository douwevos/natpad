/*
   File:    shoserializer.h
   Project: shoveler
   Author:  Douwe Vos
   Date:    May 7, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef SHOSERIALIZER_H_
#define SHOSERIALIZER_H_

#include "../model/shomodel.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_SERIALIZER              (sho_serializer_get_type())
#define SHO_SERIALIZER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_serializer_get_type(), ShoSerializer))
#define SHO_SERIALIZER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_SERIALIZER, ShoSerializerClass))
#define SHO_IS_SERIALIZER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_SERIALIZER))
#define SHO_IS_SERIALIZER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_SERIALIZER))
#define SHO_SERIALIZER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_SERIALIZER, ShoSerializerClass))

typedef struct _ShoSerializer               ShoSerializer;
typedef struct _ShoSerializerPrivate        ShoSerializerPrivate;
typedef struct _ShoSerializerClass          ShoSerializerClass;


struct _ShoSerializer {
	GObject parent;
};

struct _ShoSerializerClass {
	GObjectClass parent_class;
};


GType sho_serializer_get_type();

ShoSerializer *sho_serializer_new(ShoModel *model);

CatStringWo *sho_serialize_to_char_array(ShoSerializer *serializer);

G_END_DECLS

#endif /* SHOSERIALIZER_H_ */
