/*
   File:    worqueuerequestdelegate.h
   Project: worm
   Author:  Douwe Vos
   Date:    May 2, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef WORQUEUEREQUESTDELEGATE_H_
#define WORQUEUEREQUESTDELEGATE_H_

#include "../worrequest.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define WOR_TYPE_QUEUE_REQUEST_DELEGATE              (wor_queue_request_delegate_get_type())
#define WOR_QUEUE_REQUEST_DELEGATE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_queue_request_delegate_get_type(), WorQueueRequestDelegate))
#define WOR_QUEUE_REQUEST_DELEGATE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_QUEUE_REQUEST_DELEGATE, WorQueueRequestDelegateClass))
#define WOR_IS_QUEUE_REQUEST_DELEGATE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_QUEUE_REQUEST_DELEGATE))
#define WOR_IS_QUEUE_REQUEST_DELEGATE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_QUEUE_REQUEST_DELEGATE))
#define WOR_QUEUE_REQUEST_DELEGATE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_QUEUE_REQUEST_DELEGATE, WorQueueRequestDelegateClass))

typedef struct _WorQueueRequestDelegate               WorQueueRequestDelegate;
typedef struct _WorQueueRequestDelegatePrivate        WorQueueRequestDelegatePrivate;
typedef struct _WorQueueRequestDelegateClass          WorQueueRequestDelegateClass;

struct _WorQueue;
struct _WorQueueRequestDelegate {
	WorRequest parent;
};

struct _WorQueueRequestDelegateClass {
	WorRequestClass parent_class;
};

GType wor_queue_request_delegate_get_type();

WorQueueRequestDelegate *wor_queue_request_delegate_new(struct _WorQueue *queue, WorRequest *request, gboolean with_timeout);

G_END_DECLS

#endif /* WORQUEUEREQUESTDELEGATE_H_ */
