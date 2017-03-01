/*
   File:    xantoken.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef EDITOR_XANTOKEN_H_
#define EDITOR_XANTOKEN_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define XAN_TYPE_TOKEN              (xan_token_get_type())
#define XAN_TOKEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_token_get_type(), XanToken))
#define XAN_TOKEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_TOKEN, XanTokenClass))
#define XAN_IS_TOKEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_TOKEN))
#define XAN_IS_TOKEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_TOKEN))
#define XAN_TOKEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_TOKEN, XanTokenClass))


typedef struct _XanToken               XanToken;
typedef struct _XanTokenPrivate        XanTokenPrivate;
typedef struct _XanTokenClass          XanTokenClass;


struct _XanToken {
	GObject parent;
};

struct _XanTokenClass {
	GObjectClass parent_class;
};


GType xan_token_get_type();

XanToken *xan_token_new();

G_END_DECLS

#endif /* EDITOR_XANTOKEN_H_ */
