/*
   File:    jagpnumerictoken.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 6, 2017
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

#ifndef JAGPNUMERICTOKEN_H_
#define JAGPNUMERICTOKEN_H_

#include "jagptoken.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_NUMERIC_TOKEN              (jagp_numeric_token_get_type())
#define JAGP_NUMERIC_TOKEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_numeric_token_get_type(), JagPNumericToken))
#define JAGP_NUMERIC_TOKEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_NUMERIC_TOKEN, JagPNumericTokenClass))
#define JAGP_IS_NUMERIC_TOKEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_NUMERIC_TOKEN))
#define JAGP_IS_NUMERIC_TOKEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_NUMERIC_TOKEN))
#define JAGP_NUMERIC_TOKEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_NUMERIC_TOKEN, JagPNumericTokenClass))


typedef struct _JagPNumericToken               JagPNumericToken;
typedef struct _JagPNumericTokenPrivate        JagPNumericTokenPrivate;
typedef struct _JagPNumericTokenClass          JagPNumericTokenClass;


struct _JagPNumericToken {
	JagPToken parent;
};

struct _JagPNumericTokenClass {
	JagPTokenClass parent_class;
};


GType jagp_numeric_token_get_type();

JagPNumericToken *jagp_numeric_token_new();

int jagp_numeric_token_radix(JagPNumericToken *token);

G_END_DECLS

#endif /* JAGPNUMERICTOKEN_H_ */
