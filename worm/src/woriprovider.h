/*
   File:    woriprovider.h
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

#ifndef WORIPROVIDER_H_
#define WORIPROVIDER_H_

#include <caterpillar.h>
#include "worrequest.h"

#define WOR_TYPE_IPROVIDER                 (wor_iprovider_get_type())
#define WOR_IPROVIDER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), WOR_TYPE_IPROVIDER, WorIProvider))
#define WOR_IS_IPROVIDER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), WOR_TYPE_IPROVIDER))
#define WOR_IPROVIDER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), WOR_TYPE_IPROVIDER, WorIProviderInterface))


typedef struct _WorIProvider               WorIProvider; /* dummy object */
typedef struct _WorIProviderInterface      WorIProviderInterface;

struct _WorIProviderInterface {
	GTypeInterface parent_iface;

	WorRequest *(*next_request)(WorIProvider *self, int64_t *runtime);

	void (*increaseUsage)(WorIProvider *self);
	void (*decreaseUsage)(WorIProvider *self);
	gboolean (*isActive)(WorIProvider *self);

};

GType wor_iprovider_get_type(void);

#endif /* WORIPROVIDER_H_ */
