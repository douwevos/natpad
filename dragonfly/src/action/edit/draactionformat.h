/*
   File:    draactionformat.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Nov 20, 2016
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

#ifndef ACTION_EDIT_DRAACTIONFORMAT_H_
#define ACTION_EDIT_DRAACTIONFORMAT_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_FORMAT              (dra_action_format_get_type())
#define DRA_ACTION_FORMAT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_format_get_type(), DraActionFormat))
#define DRA_ACTION_FORMAT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_FORMAT, DraActionFormatClass))
#define DRA_IS_ACTION_FORMAT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_FORMAT))
#define DRA_IS_ACTION_FORMAT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_FORMAT))
#define DRA_ACTION_FORMAT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_FORMAT, DraActionFormatClass))


typedef struct _DraActionFormat               DraActionFormat;
typedef struct _DraActionFormatPrivate        DraActionFormatPrivate;
typedef struct _DraActionFormatClass          DraActionFormatClass;


struct _DraActionFormat {
	LeaAction parent;
};

struct _DraActionFormatClass {
	LeaActionClass parent_class;
};


GType dra_action_format_get_type();

DraActionFormat *dra_action_format_new();

void dra_action_format_set_editor_panel(DraActionFormat *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONFORMAT_H_ */
