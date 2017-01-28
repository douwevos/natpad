/*
   File:    elkactionabout.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 31, 2012
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

#ifndef ELKACTIONABOUT_H_
#define ELKACTIONABOUT_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include <viper.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_ABOUT              (elk_action_about_get_type())
#define ELK_ACTION_ABOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_about_get_type(), ElkActionAbout))
#define ELK_ACTION_ABOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_ABOUT, ElkActionAboutClass))
#define ELK_IS_ACTION_ABOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_ABOUT))
#define ELK_IS_ACTION_ABOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_ABOUT))
#define ELK_ACTION_ABOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_ABOUT, ElkActionAboutClass))
#define ELK_ACTION_ABOUT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ELK_TYPE_ACTION_ABOUT, ElkActionAboutPrivate))

typedef struct _ElkActionAbout               ElkActionAbout;
typedef struct _ElkActionAboutPrivate        ElkActionAboutPrivate;
typedef struct _ElkActionAboutClass          ElkActionAboutClass;


struct _ElkActionAbout {
	LeaAction parent;
	ElkActionAboutPrivate *priv;
};

struct _ElkActionAboutClass {
	LeaActionClass parent_class;
};


GType elk_action_about_get_type();

ElkActionAbout *elk_action_about_new(LeaFrame *frame, VipService *vip_service);

G_END_DECLS


#endif /* ELKACTIONABOUT_H_ */
