/*
   File:    shotemplate.h
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


#ifndef SHOTEMPLATE_H_
#define SHOTEMPLATE_H_

#include "shotemplateargument.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_TEMPLATE              (sho_template_get_type())
#define SHO_TEMPLATE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_template_get_type(), ShoTemplate))
#define SHO_TEMPLATE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_TEMPLATE, ShoTemplateClass))
#define SHO_IS_TEMPLATE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_TEMPLATE))
#define SHO_IS_TEMPLATE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_TEMPLATE))
#define SHO_TEMPLATE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_TEMPLATE, ShoTemplateClass))
#define SHO_TEMPLATE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), SHO_TYPE_TEMPLATE, ShoTemplatePrivate))

typedef struct _ShoTemplate               ShoTemplate;
typedef struct _ShoTemplatePrivate        ShoTemplatePrivate;
typedef struct _ShoTemplateClass          ShoTemplateClass;


struct _ShoTemplate {
	GObject parent;
	ShoTemplatePrivate *priv;
};

struct _ShoTemplateClass {
	GObjectClass parent_class;
};


GType sho_template_get_type();

ShoTemplate *sho_template_new(CatStringWo *name);

ShoTemplate *sho_template_deep_copy(ShoTemplate *source);

CatStringWo *sho_template_get_name(ShoTemplate *stemplate);

void sho_template_add_argument(ShoTemplate *stemplate, ShoTemplateArgument *arg);

G_END_DECLS

#endif /* SHOTEMPLATE_H_ */
