/*
   File:    leaactionprivate.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 27, 2013
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

#ifndef LEAACTIONPRIVATE_H_
#define LEAACTIONPRIVATE_H_

#include "leaaction.h"

G_BEGIN_DECLS

void lea_action_set_sensitive_group(LeaAction *action, gboolean flag);

void lea_action_set_visible_group(LeaAction *action, gboolean flag);

G_END_DECLS

#endif /* LEAACTIONPRIVATE_H_ */
