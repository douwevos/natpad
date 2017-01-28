/*
   File:    vipisequence.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef VIPISEQUENCE_H_
#define VIPISEQUENCE_H_

#include <caterpillar.h>

#define VIP_TYPE_ISEQUENCE                 (vip_isequence_get_type())
#define VIP_ISEQUENCE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_ISEQUENCE, VipISequence))
#define VIP_IS_ISEQUENCE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_ISEQUENCE))
#define VIP_ISEQUENCE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_ISEQUENCE, VipISequenceInterface))


typedef struct _VipISequence               VipISequence; /* dummy object */
typedef struct _VipISequenceInterface      VipISequenceInterface;



struct _VipISequenceInterface {
	GTypeInterface parent_iface;
	long long (*next)(VipISequence *self);
};

GType vip_isequence_get_type(void);

long long vip_isequence_next(VipISequence *self);

#endif /* VIPISEQUENCE_H_ */
