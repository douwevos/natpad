/*
   File:    leakeynodecontext.h
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

#ifndef LEAKEYNODECONTEXT_H_
#define LEAKEYNODECONTEXT_H_

#include "leakeynode.h"
#include "../leakeycontext.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_KEY_NODE_CONTEXT              (lea_key_node_context_get_type())
#define LEA_KEY_NODE_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_key_node_context_get_type(), LeaKeyNodeContext))
#define LEA_KEY_NODE_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_KEY_NODE_CONTEXT, LeaKeyNodeContextClass))
#define LEA_IS_KEY_NODE_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_KEY_NODE_CONTEXT))
#define LEA_IS_KEY_NODE_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_KEY_NODE_CONTEXT))
#define LEA_KEY_NODE_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_KEY_NODE_CONTEXT, LeaKeyNodeContextClass))
#define LEA_KEY_NODE_CONTEXT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_KEY_NODE_CONTEXT, LeaKeyNodeContextPrivate))


typedef struct _LeaKeyNodeContext               LeaKeyNodeContext;
typedef struct _LeaKeyNodeContextPrivate        LeaKeyNodeContextPrivate;
typedef struct _LeaKeyNodeContextClass          LeaKeyNodeContextClass;


struct _LeaKeyNodeContext {
	LeaKeyNode parent;
	LeaKeyNodeContextPrivate *priv;
};

struct _LeaKeyNodeContextClass {
	LeaKeyNodeClass parent_class;
};


GType lea_key_node_context_get_type();

LeaKeyNodeContext *lea_key_node_context_new(LeaKeyContext *key_context);

G_END_DECLS

#endif /* LEAKEYNODECONTEXT_H_ */
