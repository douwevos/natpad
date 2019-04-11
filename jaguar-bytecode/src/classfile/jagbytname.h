/*
   File:    jagbytname.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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

#ifndef JAGBYTNAME_H_
#define JAGBYTNAME_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_NAME              (jag_byt_name_get_type())
#define JAG_BYT_NAME(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_name_get_type(), JagBytName))
#define JAG_BYT_NAME_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_NAME, JagBytNameClass))
#define JAG_BYT_IS_NAME(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_NAME))
#define JAG_BYT_IS_NAME_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_NAME))
#define JAG_BYT_NAME_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_NAME, JagBytNameClass))
#define JAG_BYT_NAME_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_NAME, JagBytNamePrivate))

typedef struct _JagBytName               JagBytName;
typedef struct _JagBytNamePrivate        JagBytNamePrivate;
typedef struct _JagBytNameClass          JagBytNameClass;


struct _JagBytName {
	GObject parent;
	JagBytNamePrivate *priv;
};

struct _JagBytNameClass {
	GObjectClass parent_class;
};


GType jag_byt_name_get_type();

JagBytName *jag_byt_name_new(CatStringWo *a_full_name, gboolean from_pool);



int jag_byt_name_count(JagBytName *name);

CatStringWo *jag_byt_name_get(JagBytName *name, int index);

CatStringWo *jag_byt_name_get_short_type_name(JagBytName *name);

JagBytName *jag_byt_name_create_package_name(JagBytName *name);

CatStringWo *jag_byt_name_create_fqn(JagBytName *name);

int jag_byt_name_hash_code(JagBytName *name);

int jag_byt_name_equal(JagBytName *name, JagBytName *other);

int jag_byt_name_compare(JagBytName *name, JagBytName *other);

gboolean jag_byt_name_starts_with(JagBytName *name, JagBytName *other);

G_END_DECLS

#endif /* JAGBYTNAME_H_ */
