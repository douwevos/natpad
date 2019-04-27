/*
   File:    worrequest.h
   Project: worm
   Author:  Douwe Vos
   Date:    Dec 12, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#ifndef WORREQUEST_H_
#define WORREQUEST_H_

#include <caterpillar.h>
#include "worcondition.h"
#include "worconditionstate.h"

G_BEGIN_DECLS

#define WOR_TYPE_REQUEST              (wor_request_get_type())
#define WOR_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_request_get_type(), WorRequest))
#define WOR_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_REQUEST, WorRequestClass))
#define WOR_IS_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_REQUEST))
#define WOR_IS_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_REQUEST))
#define WOR_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_REQUEST, WorRequestClass))

typedef struct _WorRequest               WorRequest;
typedef struct _WorRequestPrivate        WorRequestPrivate;
typedef struct _WorRequestClass          WorRequestClass;


struct _WorRequest {
		GObject parent;
};

struct _WorRequestClass {
		GObjectClass parent_class;
		void (*addNotify)(WorRequest *request, CatLock *serviceConsumerLock);
		void (*setNotified)(WorRequest *request);
		void (*runRequest)(WorRequest *request);
		gboolean (*tryCombine)(WorRequest *request, WorRequest *toCombine);
};


GType wor_request_get_type();

void wor_request_construct(WorRequest *request);

gboolean wor_request_is_delayed(WorRequest *request);

void wor_request_set_time_out(WorRequest *request, uint64_t time_out);
uint64_t wor_request_get_time_out(WorRequest *request);


void wor_request_add_notify_real(WorRequest *request, CatLock *serviceConsumerLock);
void wor_request_add_notify(WorRequest *request, CatLock *serviceConsumerLock);

void wor_request_set_notified_real(WorRequest *request);
void wor_request_set_notified(WorRequest *request);

gboolean wor_request_get_and_reset_notified(WorRequest *request);


void wor_request_add_condition(WorRequest *request, WorCondition *condition2add);
gboolean wor_request_remove_condition(WorRequest *request, WorCondition *condition2remove);


void wor_request_update_states(WorRequest *request);

G_END_DECLS

#endif /* WORREQUEST_H_ */
