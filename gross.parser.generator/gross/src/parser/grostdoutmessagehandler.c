
/*
   File:    grostdoutmessagehandler.c
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

#include "grostdoutmessagehandler.h"
#include "groimessagehandler.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroStdOutMessageHandler"
#include <logging/catlog.h>

struct _GroStdOutMessageHandlerPrivate {
	void *dummy;
};

static void l_message_handler_iface_init(GroIMessageHandlerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroStdOutMessageHandler, gro_std_out_message_handler, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroStdOutMessageHandler)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IMESSAGE_HANDLER, l_message_handler_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_std_out_message_handler_class_init(GroStdOutMessageHandlerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_std_out_message_handler_init(GroStdOutMessageHandler *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroStdOutMessageHandler *instance = GRO_STD_OUT_MESSAGE_HANDLER(object);
//	GroStdOutMessageHandlerPrivate *priv = gro_std_out_message_handler_get_instance_private(instance);
	G_OBJECT_CLASS(gro_std_out_message_handler_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_std_out_message_handler_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroStdOutMessageHandler *gro_std_out_message_handler_new() {
	GroStdOutMessageHandler *result = g_object_new(GRO_TYPE_STD_OUT_MESSAGE_HANDLER, NULL);
	cat_ref_anounce(result);
//	GroStdOutMessageHandlerPrivate *priv = gro_std_out_message_handler_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}

/********************* start GroIMessageHandler implementation *********************/

static void l_handler_message(GroIMessageHandler *handler, CatStringWo *message, GroLocation *start, GroLocation *end) {
//	GroStdOutMessageHandlerPrivate *priv = gro_std_out_message_handler_get_instance_private(GRO_STD_OUT_MESSAGE_HANDLER(handler));
	if (start) {
		fprintf(stderr, "at line %lld, column %d: %s\n", gro_location_get_row(start)+1, gro_location_get_column(start)+1, cat_string_wo_getchars(message));
	} else {
		fprintf(stderr, "%s\n", cat_string_wo_getchars(message));
	}
}


static void l_message_handler_iface_init(GroIMessageHandlerInterface *iface) {
	iface->message = l_handler_message;
}

/********************* end GroIMessageHandler implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroStdOutMessageHandler *instance = GRO_STD_OUT_MESSAGE_HANDLER(self);
//	GroStdOutMessageHandlerPrivate *priv = gro_std_out_message_handler_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
