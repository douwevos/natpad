
/*
   File:    grostdoutmessagehandler.h
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

#ifndef PARSER_GROSTDOUTMESSAGEHANDLER_H_
#define PARSER_GROSTDOUTMESSAGEHANDLER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_STD_OUT_MESSAGE_HANDLER              (gro_std_out_message_handler_get_type())
#define GRO_STD_OUT_MESSAGE_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_std_out_message_handler_get_type(), GroStdOutMessageHandler))
#define GRO_STD_OUT_MESSAGE_HANDLER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_STD_OUT_MESSAGE_HANDLER, GroStdOutMessageHandlerClass))
#define GRO_IS_STD_OUT_MESSAGE_HANDLER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_STD_OUT_MESSAGE_HANDLER))
#define GRO_IS_STD_OUT_MESSAGE_HANDLER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_STD_OUT_MESSAGE_HANDLER))
#define GRO_STD_OUT_MESSAGE_HANDLER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_STD_OUT_MESSAGE_HANDLER, GroStdOutMessageHandlerClass))


typedef struct _GroStdOutMessageHandler               GroStdOutMessageHandler;
typedef struct _GroStdOutMessageHandlerPrivate        GroStdOutMessageHandlerPrivate;
typedef struct _GroStdOutMessageHandlerClass          GroStdOutMessageHandlerClass;


struct _GroStdOutMessageHandler {
	GObject parent;
};

struct _GroStdOutMessageHandlerClass {
	GObjectClass parent_class;
};


GType gro_std_out_message_handler_get_type();

GroStdOutMessageHandler *gro_std_out_message_handler_new();

G_END_DECLS

#endif /* PARSER_GROSTDOUTMESSAGEHANDLER_H_ */
