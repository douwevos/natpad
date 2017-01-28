/*
   File:    vippathprivate.h
   Project: viper
   Author:  Douwe Vos
   Date:    Jun 7, 2012
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


#ifndef VIPPATHPRIVATE_H_
#define VIPPATHPRIVATE_H_

#include "vippath.h"

VipPath *vip_path_new_with(VipPathDriveType drive_type, CatStringWo *drive, CatArrayWo *entries, gboolean is_map);

VipPath *vip_path_new_from_entries(CatArrayWo *entries);

#endif /* VIPPATHPRIVATE_H_ */
