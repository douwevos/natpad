/*
   File:    shotemplateargument.h
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
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


#ifndef SHOTEMPLATEARGUMENT_H_
#define SHOTEMPLATEARGUMENT_H_

#include "shoivalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_TEMPLATE_ARGUMENT              (sho_template_argument_get_type())
#define SHO_TEMPLATE_ARGUMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_template_argument_get_type(), ShoTemplateArgument))
#define SHO_TEMPLATE_ARGUMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_TEMPLATE_ARGUMENT, ShoTemplateArgumentClass))
#define SHO_IS_TEMPLATE_ARGUMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_TEMPLATE_ARGUMENT))
#define SHO_IS_TEMPLATE_ARGUMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_TEMPLATE_ARGUMENT))
#define SHO_TEMPLATE_ARGUMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_TEMPLATE_ARGUMENT, ShoTemplateArgumentClass))
#define SHO_TEMPLATE_ARGUMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), SHO_TYPE_TEMPLATE_ARGUMENT, ShoTemplateArgumentPrivate))

typedef struct _ShoTemplateArgument               ShoTemplateArgument;
typedef struct _ShoTemplateArgumentPrivate        ShoTemplateArgumentPrivate;
typedef struct _ShoTemplateArgumentClass          ShoTemplateArgumentClass;


struct _ShoTemplateArgument {
	GObject parent;
	ShoTemplateArgumentPrivate *priv;
};

struct _ShoTemplateArgumentClass {
	GObjectClass parent_class;
};

GType sho_template_argument_get_type();

ShoTemplateArgument *sho_template_argument_new(CatStringWo *id);
ShoTemplateArgument *sho_template_argument_deep_copy(ShoTemplateArgument *source);

void sho_template_argument_set_default(ShoTemplateArgument *argument, ShoIValue *default_value);

G_END_DECLS

#endif /* SHOTEMPLATEARGUMENT_H_ */
