/*
   File:    moomodulework.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 21, 2013
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


#ifndef MOOMODULEWORK_H_
#define MOOMODULEWORK_H_

#include "../model/moonodewo.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_MODULE_WORK              (moo_module_work_get_type())
#define MOO_MODULE_WORK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_module_work_get_type(), MooModuleWork))
#define MOO_MODULE_WORK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_MODULE_WORK, MooModuleWorkClass))
#define MOO_IS_MODULE_WORK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_MODULE_WORK))
#define MOO_IS_MODULE_WORK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_MODULE_WORK))
#define MOO_MODULE_WORK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_MODULE_WORK, MooModuleWorkClass))
#define MOO_MODULE_WORK_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_MODULE_WORK, MooModuleWorkPrivate))


typedef struct _MooModuleWork               MooModuleWork;
typedef struct _MooModuleWorkPrivate        MooModuleWorkPrivate;
typedef struct _MooModuleWorkClass          MooModuleWorkClass;


struct _MooModuleWork {
	GObject parent;
	MooModuleWorkPrivate *priv;
};

struct _MooModuleWorkClass {
	GObjectClass parent_class;
};


GType moo_module_work_get_type();

MooModuleWork *moo_module_work_new(VipISequence *moose_sequence, CatReadableTreeNode *new_node, MooNodeWo *editableNode);

G_END_DECLS

#endif /* MOOMODULEWORK_H_ */
