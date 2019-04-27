/*
   File:    leakeynodekey.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 30, 2013
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

#ifndef LEAKEYNODEKEY_H_
#define LEAKEYNODEKEY_H_

#include "leakeynode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_KEY_NODE_KEY              (lea_key_node_key_get_type())
#define LEA_KEY_NODE_KEY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_key_node_key_get_type(), LeaKeyNodeKey))
#define LEA_KEY_NODE_KEY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_KEY_NODE_KEY, LeaKeyNodeKeyClass))
#define LEA_IS_KEY_NODE_KEY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_KEY_NODE_KEY))
#define LEA_IS_KEY_NODE_KEY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_KEY_NODE_KEY))
#define LEA_KEY_NODE_KEY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_KEY_NODE_KEY, LeaKeyNodeKeyClass))

typedef struct _LeaKeyNodeKey               LeaKeyNodeKey;
typedef struct _LeaKeyNodeKeyPrivate        LeaKeyNodeKeyPrivate;
typedef struct _LeaKeyNodeKeyClass          LeaKeyNodeKeyClass;

struct _LeaKeyNodeKey {
	LeaKeyNode parent;
};

struct _LeaKeyNodeKeyClass {
	LeaKeyNodeClass parent_class;
};


GType lea_key_node_key_get_type();

LeaKeyNodeKey *lea_key_node_key_new(LeaKey *key);

G_END_DECLS

#endif /* LEAKEYNODEKEY_H_ */
