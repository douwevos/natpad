/*
   File:    jagbytcatch.h
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

#ifndef JAGBYTCATCH_H_
#define JAGBYTCATCH_H_

#include "../ast/jagastdeclarationtype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CATCH              (jag_byt_catch_get_type())
#define JAG_BYT_CATCH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_catch_get_type(), JagBytCatch))
#define JAG_BYT_CATCH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CATCH, JagBytCatchClass))
#define JAG_BYT_IS_CATCH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CATCH))
#define JAG_BYT_IS_CATCH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CATCH))
#define JAG_BYT_CATCH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CATCH, JagBytCatchClass))

typedef struct _JagBytCatch               JagBytCatch;
typedef struct _JagBytCatchPrivate        JagBytCatchPrivate;
typedef struct _JagBytCatchClass          JagBytCatchClass;


struct _JagBytCatch {
	GObject parent;
};

struct _JagBytCatchClass {
	GObjectClass parent_class;
};


GType jag_byt_catch_get_type();

JagBytCatch *jag_byt_catch_new(JagAstDeclarationType *exception_type, int handle_pc);

JagAstDeclarationType *jag_byt_catch_get_exception_type(JagBytCatch *catch);

G_END_DECLS

#endif /* JAGBYTCATCH_H_ */
