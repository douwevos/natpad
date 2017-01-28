/*
   File:    elkexplorer.h
   Project: elk
   Author:  Douwe Vos
   Date:    Nov 23, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef ELKEXPLORER_H_
#define ELKEXPLORER_H_

#include "../elkiservice.h"
#include <caterpillar.h>
#include <moose.h>
#include <mooseexplorer.h>

G_BEGIN_DECLS

#define ELK_TYPE_EXPLORER              (elk_explorer_get_type())
#define ELK_EXPLORER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_explorer_get_type(), ElkExplorer))
#define ELK_EXPLORER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_EXPLORER, ElkExplorerClass))
#define ELK_IS_EXPLORER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_EXPLORER))
#define ELK_IS_EXPLORER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_EXPLORER))
#define ELK_EXPLORER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_EXPLORER, ElkExplorerClass))
#define ELK_EXPLORER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ELK_TYPE_EXPLORER, ElkExplorerPrivate))


typedef struct _ElkExplorer               ElkExplorer;
typedef struct _ElkExplorerPrivate        ElkExplorerPrivate;
typedef struct _ElkExplorerClass          ElkExplorerClass;


struct _ElkExplorer {
	MooExplorer parent;
	ElkExplorerPrivate *priv;
};

struct _ElkExplorerClass {
	MooExplorerClass parent_class;
};


GType elk_explorer_get_type();

ElkExplorer *elk_explorer_new(ElkIService *elk_service, MooService *moo_service, MooNodeRenderRegistry *render_registry);

G_END_DECLS



#endif /* ELKEXPLORER_H_ */
