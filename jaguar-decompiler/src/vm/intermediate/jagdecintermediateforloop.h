/*
   File:    jagdecintermediateforloop.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Dec 24, 2012
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


#ifndef JAGDECINTERMEDIATEFORLOOP_H_
#define JAGDECINTERMEDIATEFORLOOP_H_

#include "jagdeciintermediatenode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP              (jag_dec_intermediate_for_loop_get_type())
#define JAG_DEC_INTERMEDIATE_FOR_LOOP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_for_loop_get_type(), JagDecIntermediateForLoop))
#define JAG_DEC_INTERMEDIATE_FOR_LOOP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP, JagDecIntermediateForLoopClass))
#define JAG_DEC_IS_INTERMEDIATE_FOR_LOOP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP))
#define JAG_DEC_IS_INTERMEDIATE_FOR_LOOP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP))
#define JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP, JagDecIntermediateForLoopClass))
#define JAG_DEC_INTERMEDIATE_FOR_LOOP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_FOR_LOOP, JagDecIntermediateForLoopPrivate))

typedef struct _JagDecIntermediateForLoop               JagDecIntermediateForLoop;
typedef struct _JagDecIntermediateForLoopPrivate        JagDecIntermediateForLoopPrivate;
typedef struct _JagDecIntermediateForLoopClass          JagDecIntermediateForLoopClass;


struct _JagDecIntermediateForLoop {
	GObject parent;
	JagDecIntermediateForLoopPrivate *priv;
};

struct _JagDecIntermediateForLoopClass {
	GObjectClass parent_class;
};


GType jag_dec_intermediate_for_loop_get_type();

JagDecIntermediateForLoop *jag_dec_intermediate_for_loop_new(JagDecIIntermediateNode *initNode, JagDecIIntermediateNode *conditionNode, JagDecIIntermediateNode *incrementNode, JagDecIIntermediateNode *loopNode);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEFORLOOP_H_ */
