/*
   File:    vipcreatepathrequestprivate.h
   Project: viper
   Author:  Douwe Vos
   Date:    Mar 6, 2014
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

#ifndef VIPCREATEPATHREQUESTPRIVATE_H_
#define VIPCREATEPATHREQUESTPRIVATE_H_

#include "vipcreatepathrequest.h"


VipNodePath *vip_create_path_request_for_path(VipCreatePathRequest *request, VipPath *full_path);


#endif /* VIPCREATEPATHREQUESTPRIVATE_H_ */
