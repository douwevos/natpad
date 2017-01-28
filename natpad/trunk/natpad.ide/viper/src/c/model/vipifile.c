/*
   File:    vipifile.c
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





#include "vipifile.h"
#include "vipiresource.h"

G_DEFINE_INTERFACE(VipIFile, vip_ifile, VIP_TYPE_IRESOURCE);

static void vip_ifile_default_init (VipIFileInterface *klass) {
	klass->openOutputStream = NULL;
}

CatIInputStream *vip_ifile_open_input_stream(VipIFile *self) {
	g_return_val_if_fail(VIP_IS_IFILE(self), NULL);
	return VIP_IFILE_GET_INTERFACE(self)->openInputStream(self);
}


CatIOutputStream *vip_ifile_open_output_stream(VipIFile *self) {
	g_return_val_if_fail(VIP_IS_IFILE(self), NULL);
	VipIFileInterface *iface = VIP_IFILE_GET_INTERFACE(self);
	if (iface->openOutputStream) {
		return VIP_IFILE_GET_INTERFACE(self)->openOutputStream(self);
	}
	return NULL;
}


long long vip_ifile_length(VipIFile *self) {
	g_return_val_if_fail(VIP_IS_IFILE(self), 0);
	return VIP_IFILE_GET_INTERFACE(self)->length(self);
}
