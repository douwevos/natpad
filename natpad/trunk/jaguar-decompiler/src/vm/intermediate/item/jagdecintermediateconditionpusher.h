/*
   File:    jagdecintermediateconditionpusher.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef JAGDECINTERMEDIATECONDITIONPUSHER_H_
#define JAGDECINTERMEDIATECONDITIONPUSHER_H_

#include "../jagdeciintermediatenode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_CONDITION_PUSHER              (jag_dec_intermediate_condition_pusher_get_type())
#define JAG_DEC_INTERMEDIATE_CONDITION_PUSHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_condition_pusher_get_type(), JagDecIntermediateConditionPusher))
#define JAG_DEC_INTERMEDIATE_CONDITION_PUSHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_PUSHER, JagDecIntermediateConditionPusherClass))
#define JAG_DEC_IS_INTERMEDIATE_CONDITION_PUSHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_PUSHER))
#define JAG_DEC_IS_INTERMEDIATE_CONDITION_PUSHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_PUSHER))
#define JAG_DEC_INTERMEDIATE_CONDITION_PUSHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_PUSHER, JagDecIntermediateConditionPusherClass))
#define JAG_DEC_INTERMEDIATE_CONDITION_PUSHER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_PUSHER, JagDecIntermediateConditionPusherPrivate))

typedef struct _JagDecIntermediateConditionPusher               JagDecIntermediateConditionPusher;
typedef struct _JagDecIntermediateConditionPusherPrivate        JagDecIntermediateConditionPusherPrivate;
typedef struct _JagDecIntermediateConditionPusherClass          JagDecIntermediateConditionPusherClass;


struct _JagDecIntermediateConditionPusher {
	GObject parent;
	JagDecIntermediateConditionPusherPrivate *priv;
};

struct _JagDecIntermediateConditionPusherClass {
	GObjectClass parent_class;
};


GType jag_dec_intermediate_condition_pusher_get_type();

JagDecIntermediateConditionPusher *jag_dec_intermediate_condition_pusher_new(JagDecIIntermediateNode *condition_node, JagDecIIntermediateNode *then_node, JagDecIIntermediateNode *else_node, gboolean invert_condition);

G_END_DECLS


#endif /* JAGDECINTERMEDIATECONDITIONPUSHER_H_ */