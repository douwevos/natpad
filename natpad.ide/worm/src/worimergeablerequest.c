/*
   File:    worimergeablerequest.c
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

#include "worimergeablerequest.h"

G_DEFINE_INTERFACE(WorIMergeableRequest, wor_imergeable_request, G_TYPE_OBJECT);

static void wor_imergeable_request_default_init (WorIMergeableRequestInterface *klass) {
}

WorMergeResult wor_imergeable_request_try_merge(WorIMergeableRequest *new_request, WorIMergeableRequest *in_queue) {
	g_return_val_if_fail(WOR_IS_IMERGEABLE_REQUEST(new_request), WOR_MERGE_FAILED);
	return WOR_IMERGEABLE_REQUEST_GET_INTERFACE(new_request)->tryMerge(new_request, in_queue);
}
