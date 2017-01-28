/*
   File:    woriconditionobserver.h
   Project: worm
   Author:  Douwe Vos
   Date:    Dec 29, 2011
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

#ifndef WORICONDITIONOBSERVER_H_
#define WORICONDITIONOBSERVER_H_

#include <caterpillar.h>

#define WOR_TYPE_ICONDITION_OBSERVER                 (wor_icondition_observer_get_type())
#define WOR_ICONDITION_OBSERVER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), WOR_TYPE_ICONDITION_OBSERVER, WorIConditionObserver))
#define WOR_IS_ICONDITION_OBSERVER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), WOR_TYPE_ICONDITION_OBSERVER))
#define WOR_ICONDITION_OBSERVER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), WOR_TYPE_ICONDITION_OBSERVER, WorIConditionObserverInterface))


typedef struct _WorIConditionObserver               WorIConditionObserver; /* dummy object */
typedef struct _WorIConditionObserverInterface      WorIConditionObserverInterface;

struct _WorIConditionObserverInterface {
	GTypeInterface parent_iface;

	void (*condition_reporting)(WorIConditionObserver *self);
};

GType wor_icondition_observer_get_type(void);


void wor_icondition_observer_condition_reporting(WorIConditionObserver *self);

#endif /* WORICONDITIONOBSERVER_H_ */
