/*
   File:    leaprovgroupmain.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 25, 2013
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

#ifndef LEAPROVGROUPMAIN_H_
#define LEAPROVGROUPMAIN_H_

#include "../action/leaactiongroup.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_PROV_GROUP_MAIN              (lea_prov_group_main_get_type())
#define LEA_PROV_GROUP_MAIN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_prov_group_main_get_type(), LeaProvGroupMain))
#define LEA_PROV_GROUP_MAIN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_PROV_GROUP_MAIN, LeaProvGroupMainClass))
#define LEA_IS_PROV_GROUP_MAIN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_PROV_GROUP_MAIN))
#define LEA_IS_PROV_GROUP_MAIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_PROV_GROUP_MAIN))
#define LEA_PROV_GROUP_MAIN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_PROV_GROUP_MAIN, LeaProvGroupMainClass))


typedef struct _LeaProvGroupMain               LeaProvGroupMain;
typedef struct _LeaProvGroupMainClass          LeaProvGroupMainClass;


struct _LeaProvGroupMain {
	LeaActionGroup parent;
};

struct _LeaProvGroupMainClass {
	LeaActionGroupClass parent_class;
};


GType lea_prov_group_main_get_type();

LeaProvGroupMain *lea_prov_group_main_new();

G_END_DECLS

#endif /* LEAPROVGROUPMAIN_H_ */
