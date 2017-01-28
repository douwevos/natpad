/*
   File:    jagbytinterfacelist.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 1, 2012
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


#ifndef JAGBYTINTERFACELIST_H_
#define JAGBYTINTERFACELIST_H_

#include "../ast/jagastdeclarationtype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_INTERFACE_LIST              (jag_byt_interface_list_get_type())
#define JAG_BYT_INTERFACE_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_interface_list_get_type(), JagBytInterfaceList))
#define JAG_BYT_INTERFACE_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_INTERFACE_LIST, JagBytInterfaceListClass))
#define JAG_BYT_IS_INTERFACE_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_INTERFACE_LIST))
#define JAG_BYT_IS_INTERFACE_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_INTERFACE_LIST))
#define JAG_BYT_INTERFACE_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_INTERFACE_LIST, JagBytInterfaceListClass))
#define JAG_BYT_INTERFACE_LIST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_INTERFACE_LIST, JagBytInterfaceListPrivate))

typedef struct _JagBytInterfaceList               JagBytInterfaceList;
typedef struct _JagBytInterfaceListPrivate        JagBytInterfaceListPrivate;
typedef struct _JagBytInterfaceListClass          JagBytInterfaceListClass;


struct _JagBytInterfaceList {
	GObject parent;
	JagBytInterfaceListPrivate *priv;
};

struct _JagBytInterfaceListClass {
	GObjectClass parent_class;
};


GType jag_byt_interface_list_get_type();

JagBytInterfaceList *jag_byt_interface_list_new();

void jag_byt_interface_list_add(JagBytInterfaceList *interface_list, JagAstDeclarationType *iface_type);

int jag_byt_interface_list_count(JagBytInterfaceList *interface_list);

JagAstDeclarationType *jag_byt_interface_list_get(JagBytInterfaceList *interface_list, int index);

G_END_DECLS

#endif /* JAGBYTINTERFACELIST_H_ */
