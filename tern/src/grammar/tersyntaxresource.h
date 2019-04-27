/*
   File:    tersyntaxresource.h
   Project: tern
   Author:  Douwe Vos
   Date:    Aug 6, 2012
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

#ifndef TERSYNTAXRESOURCE_H_
#define TERSYNTAXRESOURCE_H_

#include "scanner/tersyntax.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define TER_TYPE_SYNTAX_RESOURCE              (ter_syntax_resource_get_type())
#define TER_SYNTAX_RESOURCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_syntax_resource_get_type(), TerSyntaxResource))
#define TER_SYNTAX_RESOURCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_SYNTAX_RESOURCE, TerSyntaxResourceClass))
#define TER_IS_SYNTAX_RESOURCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_SYNTAX_RESOURCE))
#define TER_IS_SYNTAX_RESOURCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_SYNTAX_RESOURCE))
#define TER_SYNTAX_RESOURCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_SYNTAX_RESOURCE, TerSyntaxResourceClass))

typedef struct _TerSyntaxResource               TerSyntaxResource;
typedef struct _TerSyntaxResourcePrivate        TerSyntaxResourcePrivate;
typedef struct _TerSyntaxResourceClass          TerSyntaxResourceClass;

struct _TerSyntaxResource {
	GObject parent;
};

struct _TerSyntaxResourceClass {
	GObjectClass parent_class;
};

GType ter_syntax_resource_get_type();

TerSyntaxResource *ter_syntax_resource_new(CatTreeNode *vip_tree_node);

gboolean ter_syntax_resource_reload_syntax(TerSyntaxResource *syntax_resource);

TerSyntax *ter_syntax_resource_get_or_load_syntax(TerSyntaxResource *syntax_resource);

G_END_DECLS

#endif /* TERSYNTAXRESOURCE_H_ */
