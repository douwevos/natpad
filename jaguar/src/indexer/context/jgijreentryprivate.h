/*
   File:    jgijreentryprivate.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#ifndef JGIJREENTRYPRIVATE_H_
#define JGIJREENTRYPRIVATE_H_

#include "jgijreentry.h"

void jgi_jre_entry_refresh_jar_entry(JgiJreEntry *jre_entry, JgiJarEntry *jar_entry, int last_jar_entry_index);

void jgi_jre_entry_update_moose_node(JgiJreEntry *jre_entry);


#endif /* JGIJREENTRYPRIVATE_H_ */
