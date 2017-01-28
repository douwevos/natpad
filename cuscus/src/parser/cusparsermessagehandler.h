
/*
   File:    cusparsermessagehandler.h
   Project: cuscus
   Author:  Douwe Vos
   Date:    Jun 22, 2016
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

#ifndef PARSER_CUSPARSERMESSAGEHANDLER_H_
#define PARSER_CUSPARSERMESSAGEHANDLER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CUS_TYPE_PARSER_MESSAGE_HANDLER              (cur_parser_message_handler_get_type())
#define CUS_PARSER_MESSAGE_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cur_parser_message_handler_get_type(), CusParserMessageHandler))
#define CUS_PARSER_MESSAGE_HANDLER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_PARSER_MESSAGE_HANDLER, CusParserMessageHandlerClass))
#define CUS_IS_PARSER_MESSAGE_HANDLER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_PARSER_MESSAGE_HANDLER))
#define CUS_IS_PARSER_MESSAGE_HANDLER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_PARSER_MESSAGE_HANDLER))
#define CUS_PARSER_MESSAGE_HANDLER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_PARSER_MESSAGE_HANDLER, CusParserMessageHandlerClass))


typedef struct _CusParserMessageHandler               CusParserMessageHandler;
typedef struct _CusParserMessageHandlerPrivate        CusParserMessageHandlerPrivate;
typedef struct _CusParserMessageHandlerClass          CusParserMessageHandlerClass;


struct _CusParserMessageHandler {
	GObject parent;
};

struct _CusParserMessageHandlerClass {
	GObjectClass parent_class;
};


GType cur_parser_message_handler_get_type();

CusParserMessageHandler *cur_parser_message_handler_new();

G_END_DECLS

#endif /* PARSER_CUSPARSERMESSAGEHANDLER_H_ */
