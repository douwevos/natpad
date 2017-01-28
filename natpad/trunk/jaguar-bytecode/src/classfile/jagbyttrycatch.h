/*
   File:    jagbyttrycatch.h
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


#ifndef JAGBYTTRYCATCH_H_
#define JAGBYTTRYCATCH_H_

#include "jagbyticatchorfinally.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_TRY_CATCH              (jag_byt_try_catch_get_type())
#define JAG_BYT_TRY_CATCH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_try_catch_get_type(), JagBytTryCatch))
#define JAG_BYT_TRY_CATCH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_TRY_CATCH, JagBytTryCatchClass))
#define JAG_BYT_IS_TRY_CATCH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_TRY_CATCH))
#define JAG_BYT_IS_TRY_CATCH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_TRY_CATCH))
#define JAG_BYT_TRY_CATCH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_TRY_CATCH, JagBytTryCatchClass))
#define JAG_BYT_TRY_CATCH_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_TRY_CATCH, JagBytTryCatchPrivate))

typedef struct _JagBytTryCatch               JagBytTryCatch;
typedef struct _JagBytTryCatchPrivate        JagBytTryCatchPrivate;
typedef struct _JagBytTryCatchClass          JagBytTryCatchClass;


struct _JagBytTryCatch {
	GObject parent;
	JagBytTryCatchPrivate *priv;
};

struct _JagBytTryCatchClass {
	GObjectClass parent_class;
};


GType jag_byt_try_catch_get_type();

JagBytTryCatch *jag_byt_try_catch_new(int start_pc, int end_pc, CatArrayWo *e_catch_list);

int jag_byt_try_catch_get_start_pc(JagBytTryCatch *try_catch);
int jag_byt_try_catch_get_end_pc(JagBytTryCatch *try_catch);

int jag_byt_try_catch_get_catch_count(JagBytTryCatch *try_catch);
JagBytICatchOrFinally *jag_byt_try_catch_get_catch_at(JagBytTryCatch *try_catch, int index);
CatIIterator *jag_byt_try_catch_iterator(JagBytTryCatch *try_catch);


G_END_DECLS


#endif /* JAGBYTTRYCATCH_H_ */
