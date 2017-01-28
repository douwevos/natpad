/*
   File:    vipifile.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 16, 2013
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


#ifndef VIPIFILE_H_
#define VIPIFILE_H_

#include "vipiresource.h"
#include <caterpillar.h>

#define VIP_TYPE_IFILE                 (vip_ifile_get_type())
#define VIP_IFILE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_IFILE, VipIFile))
#define VIP_IS_IFILE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_IFILE))
#define VIP_IFILE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_IFILE, VipIFileInterface))


typedef struct _VipIFile               VipIFile; /* dummy object */
typedef struct _VipIFileInterface      VipIFileInterface;


struct _VipResource;

struct _VipIFileInterface {
	VipIResourceInterface parent_iface;
	CatIInputStream *(*openInputStream)(VipIFile *self);
	CatIOutputStream *(*openOutputStream)(VipIFile *self);
	long long (*length)(VipIFile *self);

};

GType vip_ifile_get_type(void);


CatIInputStream *vip_ifile_open_input_stream(VipIFile *self);
CatIOutputStream *vip_ifile_open_output_stream(VipIFile *self);

long long vip_ifile_length(VipIFile *self);

#endif /* VIPIFILE_H_ */
