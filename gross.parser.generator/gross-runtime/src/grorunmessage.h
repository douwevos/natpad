/*
   File:    grorunmessage.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 15, 2016
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

#ifndef GRORUNMESSAGE_H_
#define GRORUNMESSAGE_H_

#include "grorunlocation.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_MESSAGE              (grorun_message_get_type())
#define GRORUN_MESSAGE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_message_get_type(), GroRunMessage))
#define GRORUN_MESSAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_MESSAGE, GroRunMessageClass))
#define GRORUN_IS_MESSAGE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_MESSAGE))
#define GRORUN_IS_MESSAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_MESSAGE))
#define GRORUN_MESSAGE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_MESSAGE, GroRunMessageClass))


typedef struct _GroRunMessage               GroRunMessage;
typedef struct _GroRunMessagePrivate        GroRunMessagePrivate;
typedef struct _GroRunMessageClass          GroRunMessageClass;


struct _GroRunMessage {
	GObject parent;
};

struct _GroRunMessageClass {
	GObjectClass parent_class;
};


GType grorun_message_get_type();

GroRunMessage *grorun_message_new(GroRunMessage *parent_message, CatStringWo *text, GroRunLocation *location);

GroRunMessage *grorun_message_get_parent_message(GroRunMessage *message);
CatStringWo *grorun_message_get_text(GroRunMessage *message);
GroRunLocation *grorun_message_get_location(GroRunMessage *message);

G_END_DECLS

#endif /* GRORUNMESSAGE_H_ */
