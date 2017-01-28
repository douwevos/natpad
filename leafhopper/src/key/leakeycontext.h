/*
   File:    leakeycontext.h
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

#ifndef LEAKEYCONTEXT_H_
#define LEAKEYCONTEXT_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_KEY_CONTEXT              (lea_key_context_get_type())
#define LEA_KEY_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_key_context_get_type(), LeaKeyContext))
#define LEA_KEY_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_KEY_CONTEXT, LeaKeyContextClass))
#define LEA_IS_KEY_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_KEY_CONTEXT))
#define LEA_IS_KEY_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_KEY_CONTEXT))
#define LEA_KEY_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_KEY_CONTEXT, LeaKeyContextClass))
#define LEA_KEY_CONTEXT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_KEY_CONTEXT, LeaKeyContextPrivate))


typedef struct _LeaKeyContext               LeaKeyContext;
typedef struct _LeaKeyContextPrivate        LeaKeyContextPrivate;
typedef struct _LeaKeyContextClass          LeaKeyContextClass;


struct _LeaKeyContext {
	GObject parent;
	LeaKeyContextPrivate *priv;
};

struct _LeaKeyContextClass {
	GObjectClass parent_class;
};


GType lea_key_context_get_type();

LeaKeyContext *lea_key_context_new(CatStringWo *ca_context_name);

CatStringWo *lea_key_context_get_name(LeaKeyContext *key_context);

int lea_key_context_hash(LeaKeyContext *key_context);

gboolean lea_key_context_equal(LeaKeyContext *key_context_a, LeaKeyContext *key_context_b);


G_END_DECLS

#endif /* LEAKEYCONTEXT_H_ */
