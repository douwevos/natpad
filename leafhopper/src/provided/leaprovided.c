/*
   File:    leaprovided.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 7, 2013
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

#include "leaprovided.h"

CatS l_s_edit_group_name = CAT_S_DEF("lea.edit.group");
CatS l_s_navigate_group_name = CAT_S_DEF("lea.navigate.group");
CatS l_s_search_group_name = CAT_S_DEF("lea.search.group");
CatS l_s_window_group_name = CAT_S_DEF("lea.window.group");
CatS l_s_help_group_name = CAT_S_DEF("lea.help.group");
CatS l_s_file_group_name = CAT_S_DEF("lea.file.group");

CatStringWo *lea_prov_edit_group_name() {
	return (CatStringWo *) CAT_S(l_s_edit_group_name);
}

CatStringWo *lea_prov_navigate_group_name() {
	return (CatStringWo *) CAT_S(l_s_navigate_group_name);
}

CatStringWo *lea_prov_search_group_name() {
	return (CatStringWo *) CAT_S(l_s_search_group_name);
}

CatStringWo *lea_prov_window_group_name() {
	return (CatStringWo *) CAT_S(l_s_window_group_name);
}

CatStringWo *lea_prov_help_group_name() {
	return (CatStringWo *) CAT_S(l_s_help_group_name);
}

CatStringWo *lea_prov_file_group_name() {
	return (CatStringWo *) CAT_S(l_s_file_group_name);
}
