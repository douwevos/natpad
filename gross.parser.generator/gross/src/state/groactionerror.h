
/*
   File:    groactionerror.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 6, 2016
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

#ifndef PARSER_STATE_GROACTIONERROR_H_
#define PARSER_STATE_GROACTIONERROR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_ACTION_ERROR              (gro_action_error_get_type())
#define GRO_ACTION_ERROR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_action_error_get_type(), GroActionError))
#define GRO_ACTION_ERROR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_ACTION_ERROR, GroActionErrorClass))
#define GRO_IS_ACTION_ERROR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_ACTION_ERROR))
#define GRO_IS_ACTION_ERROR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_ACTION_ERROR))
#define GRO_ACTION_ERROR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_ACTION_ERROR, GroActionErrorClass))


typedef struct _GroActionError               GroActionError;
typedef struct _GroActionErrorPrivate        GroActionErrorPrivate;
typedef struct _GroActionErrorClass          GroActionErrorClass;


struct _GroActionError {
	GObject parent;
};

struct _GroActionErrorClass {
	GObjectClass parent_class;
};


GType gro_action_error_get_type();

GroActionError *gro_action_error_new();

G_END_DECLS

#endif /* PARSER_STATE_GROACTIONERROR_H_ */
