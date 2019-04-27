/*
   File:    elkgroupfile.h
   Project: elk
   Author:  Douwe Vos
   Date:    Dec 23, 2013
   Web:     http://www.natpad.net/
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

#ifndef ELKGROUPFILE_H_
#define ELKGROUPFILE_H_

#include "../elkiservice.h"
#include <dragonfly.h>
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_GROUP_FILE              (elk_group_file_get_type())
#define ELK_GROUP_FILE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_group_file_get_type(), ElkGroupFile))
#define ELK_GROUP_FILE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_GROUP_FILE, ElkGroupFileClass))
#define ELK_IS_GROUP_FILE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_GROUP_FILE))
#define ELK_IS_GROUP_FILE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_GROUP_FILE))
#define ELK_GROUP_FILE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_GROUP_FILE, ElkGroupFileClass))

typedef struct _ElkGroupFile               ElkGroupFile;
typedef struct _ElkGroupFilePrivate        ElkGroupFilePrivate;
typedef struct _ElkGroupFileClass          ElkGroupFileClass;

struct _ElkGroupFile {
	LeaActionGroup parent;
};

struct _ElkGroupFileClass {
	LeaActionGroupClass parent_class;
};


GType elk_group_file_get_type();

ElkGroupFile *elk_group_file_new(ElkIService *service);

LeaAction *elk_group_file_get(ElkGroupFile *group, int index);

void elk_group_file_set_editor_panel(ElkGroupFile *group, DraEditorPanel *editor);

void elk_group_file_set_editor_list(ElkGroupFile *group, CatArrayWo *a_editor_list);

G_END_DECLS

#endif /* ELKGROUPFILE_H_ */
