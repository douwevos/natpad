/*
   File:    elkasyncbulkresult.h
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 28, 2015
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

#ifndef DOCUMENTIO_ELKASYNCBULKRESULT_H_
#define DOCUMENTIO_ELKASYNCBULKRESULT_H_

#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_ASYNC_BULK_RESULT              (elk_async_bulk_result_get_type())
#define ELK_ASYNC_BULK_RESULT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_async_bulk_result_get_type(), ElkAsyncBulkResult))
#define ELK_ASYNC_BULK_RESULT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ASYNC_BULK_RESULT, ElkAsyncBulkResultClass))
#define ELK_IS_ASYNC_BULK_RESULT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ASYNC_BULK_RESULT))
#define ELK_IS_ASYNC_BULK_RESULT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ASYNC_BULK_RESULT))
#define ELK_ASYNC_BULK_RESULT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ASYNC_BULK_RESULT, ElkAsyncBulkResultClass))


typedef struct _ElkAsyncBulkResult               ElkAsyncBulkResult;
typedef struct _ElkAsyncBulkResultPrivate        ElkAsyncBulkResultPrivate;
typedef struct _ElkAsyncBulkResultClass          ElkAsyncBulkResultClass;


struct _ElkAsyncBulkResult {
	ChaIOAsync parent;
};

struct _ElkAsyncBulkResultClass {
	ChaIOAsyncClass parent_class;
};


GType elk_async_bulk_result_get_type();

ElkAsyncBulkResult *elk_async_bulk_result_new(int expect_count);

gboolean elk_async_bulk_result_wait(ElkAsyncBulkResult *bulk_result);

G_END_DECLS

#endif /* DOCUMENTIO_ELKASYNCBULKRESULT_H_ */
