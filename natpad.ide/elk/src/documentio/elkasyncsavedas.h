/*
   File:    elkasyncsavedas.h
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 4, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef DOCUMENTIO_ELKASYNCSAVEDAS_H_
#define DOCUMENTIO_ELKASYNCSAVEDAS_H_

#include "elkdocumentbin.h"
#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_ASYNC_SAVED_AS              (elk_async_saved_as_get_type())
#define ELK_ASYNC_SAVED_AS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_async_saved_as_get_type(), ElkAsyncSavedAs))
#define ELK_ASYNC_SAVED_AS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ASYNC_SAVED_AS, ElkAsyncSavedAsClass))
#define ELK_IS_ASYNC_SAVED_AS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ASYNC_SAVED_AS))
#define ELK_IS_ASYNC_SAVED_AS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ASYNC_SAVED_AS))
#define ELK_ASYNC_SAVED_AS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ASYNC_SAVED_AS, ElkAsyncSavedAsClass))


typedef struct _ElkAsyncSavedAs               ElkAsyncSavedAs;
typedef struct _ElkAsyncSavedAsPrivate        ElkAsyncSavedAsPrivate;
typedef struct _ElkAsyncSavedAsClass          ElkAsyncSavedAsClass;


struct _ElkAsyncSavedAs {
	ChaIOAsync parent;
};

struct _ElkAsyncSavedAsClass {
	ChaIOAsyncClass parent_class;
};


GType elk_async_saved_as_get_type();

ElkAsyncSavedAs *elk_async_saved_as_new(ElkDocumentBin *document_bin, CatStringWo *path, ChaIOAsync *async);

G_END_DECLS

#endif /* DOCUMENTIO_ELKASYNCSAVEDAS_H_ */
