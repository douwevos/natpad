/*
   File:    moogroupmodule.h
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 24, 2013
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

#ifndef MOOGROUPMODULE_H_
#define MOOGROUPMODULE_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_GROUP_MODULE              (moo_group_module_get_type())
#define MOO_GROUP_MODULE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_group_module_get_type(), MooGroupModule))
#define MOO_GROUP_MODULE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_GROUP_MODULE, MooGroupModuleClass))
#define MOO_IS_GROUP_MODULE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_GROUP_MODULE))
#define MOO_IS_GROUP_MODULE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_GROUP_MODULE))
#define MOO_GROUP_MODULE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_GROUP_MODULE, MooGroupModuleClass))
#define MOO_GROUP_MODULE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_GROUP_MODULE, MooGroupModulePrivate))


typedef struct _MooGroupModule               MooGroupModule;
typedef struct _MooGroupModulePrivate        MooGroupModulePrivate;
typedef struct _MooGroupModuleClass          MooGroupModuleClass;


struct _MooGroupModule {
	LeaActionGroup parent;
	MooGroupModulePrivate *priv;
};

struct _MooGroupModuleClass {
	LeaActionGroupClass parent_class;
};


GType moo_group_module_get_type();

void moo_group_module_construct(MooGroupModule *group, MooService *moo_service, LeaFrame *frame);

MooGroupModule *moo_group_module_new(MooService *moo_service, LeaFrame *frame);

LeaAction *moo_group_module_get_action_add_project(MooGroupModule *group_module);

G_END_DECLS

#endif /* MOOGROUPMODULE_H_ */
