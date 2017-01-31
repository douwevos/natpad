/*
   File:    chaioasync.h
   Project: chameleon
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

#ifndef IO_CHAIOASYNC_H_
#define IO_CHAIOASYNC_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_IO_ASYNC              (cha_io_async_get_type())
#define CHA_IO_ASYNC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_io_async_get_type(), ChaIOAsync))
#define CHA_IO_ASYNC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_IO_ASYNC, ChaIOAsyncClass))
#define CHA_IS_IO_ASYNC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_IO_ASYNC))
#define CHA_IS_IO_ASYNC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_IO_ASYNC))
#define CHA_IO_ASYNC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_IO_ASYNC, ChaIOAsyncClass))


typedef struct _ChaIOAsync               ChaIOAsync;
typedef struct _ChaIOAsyncClass          ChaIOAsyncClass;


struct _ChaIOAsync {
	GObject parent;
};

struct _ChaIOAsyncClass {
	GObjectClass parent_class;
	void (*finished)(ChaIOAsync *async, gboolean success, GError *error);
};


GType cha_io_async_get_type();

void cha_io_async_construct(ChaIOAsync *async);

ChaIOAsync *cha_io_async_new();

void cha_io_async_finished(ChaIOAsync *async, gboolean success, GError *error);

G_END_DECLS

#endif /* IO_CHAIOASYNC_H_ */
