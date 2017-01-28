/*
   File:    draactionclipboard.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Sep 4, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef ACTION_DRAACTIONCLIPBOARD_H_
#define ACTION_DRAACTIONCLIPBOARD_H_

#include <caterpillar.h>

typedef void (*DraClipboardNotifyCB)(gpointer user_data);

void dra_clipboard_refresh_has_text(gpointer clipboard_handler);

#endif /* ACTION_DRAACTIONCLIPBOARD_H_ */
