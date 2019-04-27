/*
   File:    worirequestpostbox.h
   Project: worm
   Author:  Douwe Vos
   Date:    Feb 28, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef WORIREQUESTPOSTBOX_H_
#define WORIREQUESTPOSTBOX_H_

#include <caterpillar.h>
#include "worrequest.h"

#define WOR_TYPE_IREQUEST_POST_BOX                 (wor_irequest_post_box_get_type())
#define WOR_IREQUEST_POST_BOX(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), WOR_TYPE_IREQUEST_POST_BOX, WorIRequestPostBox))
#define WOR_IS_IREQUEST_POST_BOX(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), WOR_TYPE_IREQUEST_POST_BOX))
#define WOR_IREQUEST_POST_BOX_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), WOR_TYPE_IREQUEST_POST_BOX, WorIRequestPostBoxInterface))


typedef struct _WorIRequestPostBox               WorIRequestPostBox; /* dummy object */
typedef struct _WorIRequestPostBoxInterface      WorIRequestPostBoxInterface;

struct _WorIRequestPostBoxInterface {
	GTypeInterface parent_iface;

	void (*postRequest)(WorIRequestPostBox *self, WorRequest *post);
};

GType wor_irequest_post_box_get_type(void);

void wor_irequest_post_box_post_request(WorIRequestPostBox *self, WorRequest *post);

#endif /* WORIREQUESTPOSTBOX_H_ */
