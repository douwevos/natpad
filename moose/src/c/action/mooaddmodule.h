/*
   File:    mooaddmodule.h
   Project: moose
   Author:  Douwe Vos
   Date:    Feb 14, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef MOOADDMODULE_H_
#define MOOADDMODULE_H_

#include "../mooservice.h"
#include <caterpillar.h>
#include <worm.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_ADD_MODULE              (moo_add_module_get_type())
#define MOO_ADD_MODULE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_add_module_get_type(), MooAddModule))
#define MOO_ADD_MODULE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ADD_MODULE, MooAddModuleClass))
#define MOO_IS_ADD_MODULE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ADD_MODULE))
#define MOO_IS_ADD_MODULE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ADD_MODULE))
#define MOO_ADD_MODULE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ADD_MODULE, MooAddModuleClass))

typedef struct _MooAddModule               MooAddModule;
typedef struct _MooAddModulePrivate        MooAddModulePrivate;
typedef struct _MooAddModuleClass          MooAddModuleClass;


struct _MooAddModule {
	WorRequest parent;
};

struct _MooAddModuleClass {
	WorRequestClass parent_class;
};


GType moo_add_module_get_type();

MooAddModule *moo_add_module_new(MooService *moo_service, VipNodePath *node_path);


void moo_add_module_from_path(MooService *moo_service, VipPath *path);

void moo_add_module_wait_for_done(MooAddModule *add_module);

G_END_DECLS

#endif /* MOOADDMODULE_H_ */
