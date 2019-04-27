/*
   File:    shotemplatecall.h
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

#ifndef SHOTEMPLATECALL_H_
#define SHOTEMPLATECALL_H_

#include "sholistvalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_TEMPLATE_CALL              (sho_template_call_get_type())
#define SHO_TEMPLATE_CALL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_template_call_get_type(), ShoTemplateCall))
#define SHO_TEMPLATE_CALL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_TEMPLATE_CALL, ShoTemplateCallClass))
#define SHO_IS_TEMPLATE_CALL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_TEMPLATE_CALL))
#define SHO_IS_TEMPLATE_CALL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_TEMPLATE_CALL))
#define SHO_TEMPLATE_CALL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_TEMPLATE_CALL, ShoTemplateCallClass))

typedef struct _ShoTemplateCall               ShoTemplateCall;
typedef struct _ShoTemplateCallPrivate        ShoTemplateCallPrivate;
typedef struct _ShoTemplateCallClass          ShoTemplateCallClass;


struct _ShoTemplateCall {
	GObject parent;
};

struct _ShoTemplateCallClass {
	GObjectClass parent_class;
};

GType sho_template_call_get_type();

ShoTemplateCall *sho_template_call_new(CatStringWo *template_name);
ShoTemplateCall *sho_template_call_deep_copy(ShoTemplateCall *source);

void sho_template_call_set_argument_list(ShoTemplateCall *template_call, ShoListValue *argument_list);

G_END_DECLS

#endif /* SHOTEMPLATECALL_H_ */
