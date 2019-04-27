/*
   File:    worworker.h
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

#ifndef WORWORKER_H_
#define WORWORKER_H_

#include "woriprovider.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define WOR_TYPE_WORKER              (wor_worker_get_type())
#define WOR_WORKER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_worker_get_type(), WorWorker))
#define WOR_WORKER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_WORKER, WorWorkerClass))
#define WOR_IS_WORKER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_WORKER))
#define WOR_IS_WORKER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_WORKER))
#define WOR_WORKER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_WORKER, WorWorkerClass))

typedef struct _WorWorker               WorWorker;
typedef struct _WorWorkerPrivate        WorWorkerPrivate;
typedef struct _WorWorkerClass          WorWorkerClass;


struct _WorWorker {
		GObject parent;
};

struct _WorWorkerClass {
		GObjectClass parent_class;
};


GType wor_worker_get_type();

WorWorker *wor_worker_new(WorIProvider *work_provider);

void wor_worker_dump_state(WorWorker *worker);

G_END_DECLS

#endif /* WORWORKER_H_ */
