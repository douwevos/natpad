/*
   File:    chaasyncbasic.h
   Project: chameleon
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

#ifndef IO_CHAASYNCBASIC_H_
#define IO_CHAASYNCBASIC_H_

#include "chaioasync.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_ASYNC_BASIC              (cha_async_basic_get_type())
#define CHA_ASYNC_BASIC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_async_basic_get_type(), ChaAsyncBasic))
#define CHA_ASYNC_BASIC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_ASYNC_BASIC, ChaAsyncBasicClass))
#define CHA_IS_ASYNC_BASIC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_ASYNC_BASIC))
#define CHA_IS_ASYNC_BASIC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_ASYNC_BASIC))
#define CHA_ASYNC_BASIC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_ASYNC_BASIC, ChaAsyncBasicClass))


typedef struct _ChaAsyncBasic               ChaAsyncBasic;
typedef struct _ChaAsyncBasicPrivate        ChaAsyncBasicPrivate;
typedef struct _ChaAsyncBasicClass          ChaAsyncBasicClass;


struct _ChaAsyncBasic {
	ChaIOAsync parent;
};

struct _ChaAsyncBasicClass {
	ChaIOAsyncClass parent_class;
};


GType cha_async_basic_get_type();

ChaAsyncBasic *cha_async_basic_new();

gboolean cha_async_basic_wait(ChaAsyncBasic *async_basic, long time_to_wait_ms);

G_END_DECLS

#endif /* IO_CHAASYNCBASIC_H_ */
