/*
   File:    cowcontainerpriv.h
   Project: cow
   Author:  Douwe Vos
   Date:    Sep 28, 2014
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

#ifndef COWCONTAINERPRIV_H_
#define COWCONTAINERPRIV_H_

#include "cowcontainer.h"

void cow_container_set_configuration_direct(CowContainer *container, gpointer new_config);
void cow_container_set_configuration_real(CowContainer *container, gpointer new_config);

#endif /* COWCONTAINERPRIV_H_ */
