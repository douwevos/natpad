/*
   File:    chautf8.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 12, 2015
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

#ifndef UOW_CHAUTF8_H_
#define UOW_CHAUTF8_H_

#include <glib.h>

gunichar cha_utf8_unichar_at(const char *data, const char *end);

int cha_utf8_prev_char(const char *chrs, int cur_x_next, gunichar *uch);
int cha_utf8_next_char(const char *chrs, int cur_x_next, int maxlength, gunichar *uch);


#endif /* UOW_CHAUTF8_H_ */
