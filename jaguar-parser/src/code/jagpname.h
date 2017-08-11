/*
   File:    jagpname.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 3, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef CODE_JAGPNAME_H_
#define CODE_JAGPNAME_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_NAME              (jagp_name_get_type())
#define JAGP_NAME(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_name_get_type(), JagPName))
#define JAGP_NAME_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_NAME, JagPNameClass))
#define JAGP_IS_NAME(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_NAME))
#define JAGP_IS_NAME_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_NAME))
#define JAGP_NAME_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_NAME, JagPNameClass))


typedef struct _JagPName               JagPName;
typedef struct _JagPNamePrivate        JagPNamePrivate;
typedef struct _JagPNameClass          JagPNameClass;


struct _JagPName {
	GObject parent;
};

struct _JagPNameClass {
	GObjectClass parent_class;
};


GType jagp_name_get_type();

JagPName *jagp_name_new(CatStringWo *text);

CatStringWo *jagp_name_get_string(JagPName *name);

G_END_DECLS

#endif /* CODE_JAGPNAME_H_ */
