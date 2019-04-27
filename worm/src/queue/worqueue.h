/*
   File:    worqueue.h
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

#ifndef WORQUEUE_H_
#define WORQUEUE_H_

#include <caterpillar.h>
#include "../worrequest.h"
#include "../worservice.h"

G_BEGIN_DECLS

#define WOR_TYPE_QUEUE              (wor_queue_get_type())
#define WOR_QUEUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_queue_get_type(), WorQueue))
#define WOR_QUEUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_QUEUE, WorQueueClass))
#define WOR_IS_QUEUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_QUEUE))
#define WOR_IS_QUEUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_QUEUE))
#define WOR_QUEUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_QUEUE, WorQueueClass))

typedef struct _WorQueue               WorQueue;
typedef struct _WorQueuePrivate        WorQueuePrivate;
typedef struct _WorQueueClass          WorQueueClass;

struct _WorQueue {
	GObject parent;
};

struct _WorQueueClass {
	GObjectClass parent_class;
};


GType wor_queue_get_type();

WorQueue *wor_queue_new(WorService *wor_service);

void wor_queue_set_semaphore_size(WorQueue *queue, int count);


void wor_queue_post(WorQueue *queue, WorRequest *request);

void wor_queue_post_delayed(WorQueue *queue, WorRequest *request, uint64_t timeout);


G_END_DECLS

#endif /* WORQUEUE_H_ */
