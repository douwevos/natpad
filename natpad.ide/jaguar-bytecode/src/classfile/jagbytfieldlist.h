/*
   File:    jagbytfieldlist.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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


#ifndef JAGBYTFIELDLIST_H_
#define JAGBYTFIELDLIST_H_


#include "jagbytfield.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_FIELD_LIST              (jag_byt_field_list_get_type())
#define JAG_BYT_FIELD_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_field_list_get_type(), JagBytFieldList))
#define JAG_BYT_FIELD_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_FIELD_LIST, JagBytFieldListClass))
#define JAG_BYT_IS_FIELD_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_FIELD_LIST))
#define JAG_BYT_IS_FIELD_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_FIELD_LIST))
#define JAG_BYT_FIELD_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_FIELD_LIST, JagBytFieldListClass))
#define JAG_BYT_FIELD_LIST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_FIELD_LIST, JagBytFieldListPrivate))

typedef struct _JagBytFieldList               JagBytFieldList;
typedef struct _JagBytFieldListPrivate        JagBytFieldListPrivate;
typedef struct _JagBytFieldListClass          JagBytFieldListClass;


struct _JagBytFieldList {
	GInitiallyUnowned parent;
	JagBytFieldListPrivate *priv;
};

struct _JagBytFieldListClass {
	GInitiallyUnownedClass parent_class;
};


GType jag_byt_field_list_get_type();

JagBytFieldList *jag_byt_field_list_new(CatArrayWo *a_list);

int jag_byt_field_list_count(JagBytFieldList *field_list);

JagBytField *jag_byt_field_list_get_at(JagBytFieldList *field_list, int index);

CatIIterator *jag_byt_field_list_iterator(JagBytFieldList *field_list);


G_END_DECLS



#endif /* JAGBYTFIELDLIST_H_ */
