/*
   File:    grorunfulltokenfactory.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 12, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef FULL_GRORUNFULLTOKENFACTORY_H_
#define FULL_GRORUNFULLTOKENFACTORY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_FULL_TOKEN_FACTORY              (grorun_full_token_factory_get_type())
#define GRORUN_FULL_TOKEN_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_full_token_factory_get_type(), GroRunFullTokenFactory))
#define GRORUN_FULL_TOKEN_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_FULL_TOKEN_FACTORY, GroRunFullTokenFactoryClass))
#define GRORUN_IS_FULL_TOKEN_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_FULL_TOKEN_FACTORY))
#define GRORUN_IS_FULL_TOKEN_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_FULL_TOKEN_FACTORY))
#define GRORUN_FULL_TOKEN_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_FULL_TOKEN_FACTORY, GroRunFullTokenFactoryClass))


typedef struct _GroRunFullTokenFactory               GroRunFullTokenFactory;
typedef struct _GroRunFullTokenFactoryPrivate        GroRunFullTokenFactoryPrivate;
typedef struct _GroRunFullTokenFactoryClass          GroRunFullTokenFactoryClass;


struct _GroRunFullTokenFactory {
	GObject parent;
};

struct _GroRunFullTokenFactoryClass {
	GObjectClass parent_class;
};


GType grorun_full_token_factory_get_type();

GroRunFullTokenFactory *grorun_full_token_factory_new();

G_END_DECLS

#endif /* FULL_GRORUNFULLTOKENFACTORY_H_ */
