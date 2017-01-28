/*
   File:    worqueueprivate.h
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

#ifndef WORQUEUEPRIVATE_H_
#define WORQUEUEPRIVATE_H_

#include "worqueue.h"
#include "worqueuerequestdelegate.h"

void wor_queue_forward_next(WorQueue *queue);

void wor_queue_repost(WorQueue *queue, WorQueueRequestDelegate *wrapped_request);

#endif /* WORQUEUEPRIVATE_H_ */
