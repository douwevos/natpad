/*
   File:    shomodel.h
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


#ifndef SHOMODEL_H_
#define SHOMODEL_H_

#include "shoreference.h"
#include "shoreferencelist.h"
#include "shotemplate.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_MODEL              (sho_model_get_type())
#define SHO_MODEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_model_get_type(), ShoModel))
#define SHO_MODEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_MODEL, ShoModelClass))
#define SHO_IS_MODEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_MODEL))
#define SHO_IS_MODEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_MODEL))
#define SHO_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_MODEL, ShoModelClass))


typedef struct _ShoModel               ShoModel;
typedef struct _ShoModelPrivate        ShoModelPrivate;
typedef struct _ShoModelClass          ShoModelClass;


struct _ShoModel {
	ShoReferenceList parent;
};

struct _ShoModelClass {
	ShoReferenceListClass parent_class;
};


GType sho_model_get_type();

ShoModel *sho_model_new();

ShoModel *sho_model_deep_copy(ShoModel *source);

void sho_model_add_template(ShoModel *model, ShoTemplate *stemplate);

void sho_model_dump(ShoModel *model);

G_END_DECLS

#endif /* SHOMODEL_H_ */
