
/*
   File:    groimessagehandler.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 26, 2016
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

#ifndef PARSER_GROIMESSAGEHANDLER_H_
#define PARSER_GROIMESSAGEHANDLER_H_

#include "grolocation.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_IMESSAGE_HANDLER                 (gro_imessage_handler_get_type())
#define GRO_IMESSAGE_HANDLER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRO_TYPE_IMESSAGE_HANDLER, GroIMessageHandler))
#define GRO_IS_IMESSAGE_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRO_TYPE_IMESSAGE_HANDLER))
#define GRO_IMESSAGE_HANDLER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRO_TYPE_IMESSAGE_HANDLER, GroIMessageHandlerInterface))


typedef struct _GroIMessageHandler               GroIMessageHandler; /* dummy object */
typedef struct _GroIMessageHandlerInterface      GroIMessageHandlerInterface;


struct _GroIMessageHandlerInterface {
	GTypeInterface parent_iface;
	void (*message)(GroIMessageHandler *self, CatStringWo *message, GroLocation *start, GroLocation *end);
};

GType gro_imessage_handler_get_type(void);

void gro_imessage_handler_message(GroIMessageHandler *ab_a, CatStringWo *message, GroLocation *start, GroLocation *end);

G_END_DECLS

#endif /* PARSER_GROIMESSAGEHANDLER_H_ */
