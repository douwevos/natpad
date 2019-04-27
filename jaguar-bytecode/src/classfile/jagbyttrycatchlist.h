/*
   File:    jagbyttrycatchlist.h
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

#ifndef JAGBYTTRYCATCHLIST_H_
#define JAGBYTTRYCATCHLIST_H_

#include "jagbyttrycatch.h"
#include "constant/jagbyticonstantprovider.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_TRY_CATCH_LIST              (jag_byt_try_catch_list_get_type())
#define JAG_BYT_TRY_CATCH_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_try_catch_list_get_type(), JagBytTryCatchList))
#define JAG_BYT_TRY_CATCH_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_TRY_CATCH_LIST, JagBytTryCatchListClass))
#define JAG_BYT_IS_TRY_CATCH_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_TRY_CATCH_LIST))
#define JAG_BYT_IS_TRY_CATCH_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_TRY_CATCH_LIST))
#define JAG_BYT_TRY_CATCH_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_TRY_CATCH_LIST, JagBytTryCatchListClass))

typedef struct _JagBytTryCatchList               JagBytTryCatchList;
typedef struct _JagBytTryCatchListPrivate        JagBytTryCatchListPrivate;
typedef struct _JagBytTryCatchListClass          JagBytTryCatchListClass;


struct _JagBytTryCatchList {
	GObject parent;
};

struct _JagBytTryCatchListClass {
	GObjectClass parent_class;
};


GType jag_byt_try_catch_list_get_type();

JagBytTryCatchList *jag_byt_try_catch_list_new(CatArrayWo *e_attr_code_exceptions, JagBytIConstantProvider *constant_provider);

int jag_byt_try_catch_list_count(JagBytTryCatchList *try_catch_list);
JagBytTryCatch *jag_byt_try_catch_list_get(JagBytTryCatchList *try_catch_list, int index);

G_END_DECLS

#endif /* JAGBYTTRYCATCHLIST_H_ */
