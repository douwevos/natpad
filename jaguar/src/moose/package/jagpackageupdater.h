/*
   File:    jagpackageupdater.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 23, 2013
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

#ifndef JAGPACKAGEUPDATER_H_
#define JAGPACKAGEUPDATER_H_

#include <caterpillar.h>
#include <viper.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_PACKAGE_UPDATER              (jag_package_updater_get_type())
#define JAG_PACKAGE_UPDATER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_package_updater_get_type(), JagPackageUpdater))
#define JAG_PACKAGE_UPDATER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PACKAGE_UPDATER, JagPackageUpdaterClass))
#define JAG_IS_PACKAGE_UPDATER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PACKAGE_UPDATER))
#define JAG_IS_PACKAGE_UPDATER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PACKAGE_UPDATER))
#define JAG_PACKAGE_UPDATER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PACKAGE_UPDATER, JagPackageUpdaterClass))

typedef struct _JagPackageUpdater               JagPackageUpdater;
typedef struct _JagPackageUpdaterPrivate        JagPackageUpdaterPrivate;
typedef struct _JagPackageUpdaterClass          JagPackageUpdaterClass;

struct _JagPackageUpdater {
	GObject parent;
};

struct _JagPackageUpdaterClass {
	GObjectClass parent_class;
};


GType jag_package_updater_get_type();

JagPackageUpdater *jag_package_updater_new(VipISequence *moo_id_sequence, MooNodeWo *editable_src_folder, MooNodeWo *res_node, MooNodeWo *editable_package, MooIdPath *package_path);

void jag_package_updater_run(JagPackageUpdater *updater);

G_END_DECLS

#endif /* JAGPACKAGEUPDATER_H_ */
