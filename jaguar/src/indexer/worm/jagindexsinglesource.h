/*
   File:    jagindexsinglesource.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 5, 2014
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

#ifndef JAGINDEXSINGLESOURCE_H_
#define JAGINDEXSINGLESOURCE_H_

#include <caterpillar.h>
#include <worm.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_INDEX_SINGLE_SOURCE              (jag_index_single_source_get_type())
#define JAG_INDEX_SINGLE_SOURCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_index_single_source_get_type(), JagIndexSingleSource))
#define JAG_INDEX_SINGLE_SOURCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEX_SINGLE_SOURCE, JagIndexSingleSourceClass))
#define JAG_IS_INDEX_SINGLE_SOURCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEX_SINGLE_SOURCE))
#define JAG_IS_INDEX_SINGLE_SOURCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEX_SINGLE_SOURCE))
#define JAG_INDEX_SINGLE_SOURCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEX_SINGLE_SOURCE, JagIndexSingleSourceClass))

typedef struct _JagIndexSingleSource               JagIndexSingleSource;
typedef struct _JagIndexSingleSourcePrivate        JagIndexSingleSourcePrivate;
typedef struct _JagIndexSingleSourceClass          JagIndexSingleSourceClass;

struct _JagIndexSingleSource {
	WorRequest parent;
};

struct _JagIndexSingleSourceClass {
	WorRequestClass parent_class;
};

GType jag_index_single_source_get_type();

JagIndexSingleSource *jag_index_single_source_new(MooService *moo_service, MooIdPath *jag_source_id_path);

G_END_DECLS

#endif /* JAGINDEXSINGLESOURCE_H_ */
