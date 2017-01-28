/*
   File:    worimergeablerequest.h
   Project: worm
   Author:  Douwe Vos
   Date:    Jan 18, 2015
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

#ifndef WORIMERGEABLEREQUEST_H_
#define WORIMERGEABLEREQUEST_H_

#include <caterpillar.h>
#include "worrequest.h"

#define WOR_TYPE_IMERGEABLE_REQUEST                 (wor_imergeable_request_get_type())
#define WOR_IMERGEABLE_REQUEST(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), WOR_TYPE_IMERGEABLE_REQUEST, WorIMergeableRequest))
#define WOR_IS_IMERGEABLE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), WOR_TYPE_IMERGEABLE_REQUEST))
#define WOR_IMERGEABLE_REQUEST_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), WOR_TYPE_IMERGEABLE_REQUEST, WorIMergeableRequestInterface))

typedef enum {
	WOR_MERGE_FAILED,
	WOR_MERGE_KEEP_IN_QUEUE,
	WOR_MERGE_KEEP_NEW
} WorMergeResult;

typedef struct _WorIMergeableRequest               WorIMergeableRequest; /* dummy object */
typedef struct _WorIMergeableRequestInterface      WorIMergeableRequestInterface;

struct _WorIMergeableRequestInterface {
	GTypeInterface parent_iface;

	WorMergeResult (*tryMerge)(WorIMergeableRequest *new_request, WorIMergeableRequest *in_queue);
};

GType wor_imergeable_request_get_type(void);

WorMergeResult wor_imergeable_request_try_merge(WorIMergeableRequest *self, WorIMergeableRequest *post);

#endif /* WORIMERGEABLEREQUEST_H_ */
