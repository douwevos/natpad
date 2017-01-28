/*
   File:    grorunfulltoken.h
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

#ifndef FULL_GRORUNFULLTOKEN_H_
#define FULL_GRORUNFULLTOKEN_H_

#include "../grorunsymbol.h"
#include "../grorunlocation.h"
#include "../grorunitoken.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_FULL_TOKEN              (grorun_full_token_get_type())
#define GRORUN_FULL_TOKEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_full_token_get_type(), GroRunFullToken))
#define GRORUN_FULL_TOKEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_FULL_TOKEN, GroRunFullTokenClass))
#define GRORUN_IS_FULL_TOKEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_FULL_TOKEN))
#define GRORUN_IS_FULL_TOKEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_FULL_TOKEN))
#define GRORUN_FULL_TOKEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_FULL_TOKEN, GroRunFullTokenClass))


typedef struct _GroRunFullToken               GroRunFullToken;
typedef struct _GroRunFullTokenPrivate        GroRunFullTokenPrivate;
typedef struct _GroRunFullTokenClass          GroRunFullTokenClass;


struct _GroRunFullToken {
	GObject parent;
};

struct _GroRunFullTokenClass {
	GObjectClass parent_class;
};


GType grorun_full_token_get_type();

GroRunFullToken *grorun_full_token_new(GroRunLeftRight indexes, GroRunSymbol *symbol, GObject *value, GroRunLocation *location, CatStringWo *symbol_description);

GroRunLocation *grorun_full_token_get_location(GroRunFullToken *token);

void grorun_full_token_set_user_index(GroRunFullToken *token, int index);
int grorun_full_token_get_user_index(GroRunFullToken *token);


G_END_DECLS

#endif /* FULL_GRORUNFULLTOKEN_H_ */
