/*
   File:    jagbytfinally.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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


#ifndef JAGBYTFINALLY_H_
#define JAGBYTFINALLY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_FINALLY              (jag_byt_finally_get_type())
#define JAG_BYT_FINALLY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_finally_get_type(), JagBytFinally))
#define JAG_BYT_FINALLY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_FINALLY, JagBytFinallyClass))
#define JAG_BYT_IS_FINALLY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_FINALLY))
#define JAG_BYT_IS_FINALLY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_FINALLY))
#define JAG_BYT_FINALLY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_FINALLY, JagBytFinallyClass))
#define JAG_BYT_FINALLY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_FINALLY, JagBytFinallyPrivate))

typedef struct _JagBytFinally               JagBytFinally;
typedef struct _JagBytFinallyPrivate        JagBytFinallyPrivate;
typedef struct _JagBytFinallyClass          JagBytFinallyClass;


struct _JagBytFinally {
	GObject parent;
	JagBytFinallyPrivate *priv;
};

struct _JagBytFinallyClass {
	GObjectClass parent_class;
};


GType jag_byt_finally_get_type();

JagBytFinally *jag_byt_finally_new(int handle_pc);

G_END_DECLS


#endif /* JAGBYTFINALLY_H_ */
