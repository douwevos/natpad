/*
   File:    grorunmodel.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 10, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef GRORUNMODEL_H_
#define GRORUNMODEL_H_

#include "grorunstate.h"
#include "grorunsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

struct _GroRunSymbolDescription {
	gboolean is_non_terminal;
	const char *name;
	int id;

};


typedef struct _GroRunSymbolDescription GroRunSymbolDescription;

#define GRORUN_TYPE_MODEL              (grorun_model_get_type())
#define GRORUN_MODEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_model_get_type(), GroRunModel))
#define GRORUN_MODEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_MODEL, GroRunModelClass))
#define GRORUN_IS_MODEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_MODEL))
#define GRORUN_IS_MODEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_MODEL))
#define GRORUN_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_MODEL, GroRunModelClass))


typedef struct _GroRunModel               GroRunModel;
typedef struct _GroRunModelPrivate        GroRunModelPrivate;
typedef struct _GroRunModelClass          GroRunModelClass;


struct _GroRunModel {
	GObject parent;
};

struct _GroRunModelClass {
	GObjectClass parent_class;
};


GType grorun_model_get_type();

GroRunModel *grorun_model_new(const GroRunSymbolDescription *symbol_desc, int symbol_desc_count, const char *states_text, int states_count);

GroRunState *grorun_model_get_start_state(GroRunModel *model);

GroRunSymbol *grorun_model_get_error_terminal(GroRunModel *model);
GroRunSymbol *grorun_model_get_eof_terminal(GroRunModel *model);

G_END_DECLS

#endif /* GRORUNMODEL_H_ */
