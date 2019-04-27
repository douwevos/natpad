/*
   File:    jagbytmethodlist.h
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

#ifndef JAGBYTMETHODLIST_H_
#define JAGBYTMETHODLIST_H_

#include "jagbytmethod.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_METHOD_LIST              (jag_byt_method_list_get_type())
#define JAG_BYT_METHOD_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_method_list_get_type(), JagBytMethodList))
#define JAG_BYT_METHOD_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_METHOD_LIST, JagBytMethodListClass))
#define JAG_BYT_IS_METHOD_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_METHOD_LIST))
#define JAG_BYT_IS_METHOD_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_METHOD_LIST))
#define JAG_BYT_METHOD_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_METHOD_LIST, JagBytMethodListClass))

typedef struct _JagBytMethodList               JagBytMethodList;
typedef struct _JagBytMethodListPrivate        JagBytMethodListPrivate;
typedef struct _JagBytMethodListClass          JagBytMethodListClass;


struct _JagBytMethodList {
	GInitiallyUnowned parent;
};

struct _JagBytMethodListClass {
	GInitiallyUnownedClass parent_class;
};


GType jag_byt_method_list_get_type();

JagBytMethodList *jag_byt_method_list_new(CatArrayWo *a_method_list);


int jag_byt_method_list_count(JagBytMethodList *method_list);

JagBytMethod *jag_byt_method_list_get_at(JagBytMethodList *method_list, int index);

CatIIterator *jag_byt_method_list_iterator(JagBytMethodList *method_list);

G_END_DECLS

#endif /* JAGBYTMETHODLIST_H_ */
