/*
   File:    dragrouplineendings.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 13, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef ACTION_EDIT_DRAGROUPLINEENDINGS_H_
#define ACTION_EDIT_DRAGROUPLINEENDINGS_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_GROUP_LINE_ENDINGS              (dra_group_line_endings_get_type())
#define DRA_GROUP_LINE_ENDINGS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_group_line_endings_get_type(), DraGroupLineEndings))
#define DRA_GROUP_LINE_ENDINGS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_GROUP_LINE_ENDINGS, DraGroupLineEndingsClass))
#define DRA_IS_GROUP_LINE_ENDINGS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_GROUP_LINE_ENDINGS))
#define DRA_IS_GROUP_LINE_ENDINGS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_GROUP_LINE_ENDINGS))
#define DRA_GROUP_LINE_ENDINGS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_GROUP_LINE_ENDINGS, DraGroupLineEndingsClass))

struct _DraGroupLineEndings {
	LeaActionGroup parent;
};

struct _DraGroupLineEndingsClass {
	LeaActionGroupClass parent_class;
};

typedef struct _DraGroupLineEndings               DraGroupLineEndings;
typedef struct _DraGroupLineEndingsPrivate        DraGroupLineEndingsPrivate;
typedef struct _DraGroupLineEndingsClass          DraGroupLineEndingsClass;


GType dra_group_line_endings_get_type();

DraGroupLineEndings *dra_group_line_endings_new(LeaKeyContext *key_context);

void dra_group_line_endings_set_editor_panel(DraGroupLineEndings *group_line_endings, DraEditorPanel *editor_panel);

void dra_group_line_endings_set_line_ends(DraGroupLineEndings *group, ChaLineEnd line_ends, gboolean line_ends_are_mixed);

G_END_DECLS

#endif /* ACTION_EDIT_DRAGROUPLINEENDINGS_H_ */
