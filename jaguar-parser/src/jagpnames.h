/*
   File:    jagpnames.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Jul 15, 2017
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

#ifndef JAGPNAMES_H_
#define JAGPNAMES_H_

#include "code/jagpname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_NAMES              (jagp_names_get_type())
#define JAGP_NAMES(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_names_get_type(), JagPNames))
#define JAGP_NAMES_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_NAMES, JagPNamesClass))
#define JAGP_IS_NAMES(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_NAMES))
#define JAGP_IS_NAMES_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_NAMES))
#define JAGP_NAMES_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_NAMES, JagPNamesClass))


typedef struct _JagPNames               JagPNames;
typedef struct _JagPNamesPrivate        JagPNamesPrivate;
typedef struct _JagPNamesClass          JagPNamesClass;


struct _JagPNames {
	GObject parent;
};

struct _JagPNamesClass {
	GObjectClass parent_class;
};


GType jagp_names_get_type();

JagPNames *jagp_names_new();

JagPName *jagp_names_get(JagPNames *names, CatStringWo *text);
JagPName *jagp_names_by_chars(JagPNames *names, const char *txt);

G_END_DECLS

#endif /* JAGPNAMES_H_ */
