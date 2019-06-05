/*
   File:    chautf8.c
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

#include "chautf8.h"


gunichar cha_utf8_unichar_at(const char *data, const char *end) {
	gunichar result = 0;
	char fb = *data++;
	int len = 0;
	if (fb<0x80) {
		result = fb;
	} else if ((fb & 0xE0) == 0xC0) {
		result = (fb & 0x1F);
		len = 1;
	} else if ((fb & 0xF0) == 0xE0) {
		result = (fb & 0xF);
		len = 2;
	} else if ((fb & 0xF8) == 0xF0) {
		result = fb & 0x7;
		len = 3;
	} else if ((fb & 0xFC) == 0xF8) {
		result = (fb & 0x3);
		len = 4;
	} else if ((fb & 0xFE) == 0xFC) {
		result = (fb & 0x1);
		len = 5;
	} else if ((fb & 0xFF) == 0xFE) {
		len = 6;
	}
	while(len-->0) {
		result = result<<6;
		if (data<end) {
			result = result | (*data++ & 0x3F);
		}
	}
	return result;
}

int cha_utf8_prev_char(const char *chrs, int cur_x_next, gunichar *uch) {
	int result = cur_x_next;
	result--;
	while(result>=0) {
		char c = chrs[result];
		if ((c & 0xC0)==0x80) {
			result--;
		} else {
			*uch = cha_utf8_unichar_at(chrs+result, chrs+cur_x_next);
			break;
		}
	}
	return result;
}



int cha_utf8_next_char(const char *chrs, int cur_x_next, int maxlength, gunichar *uch) {
	int result = cur_x_next;
	char c = chrs[result];
	if ((c & 0x80)==0x0) {
		result++;
		*uch = c;
	} else if ((c & 0xC0)==0x80) {
		// TODO Invalid code-point
		result++;
		*uch = c;
	} else {
		if ((c & 0xE0) == 0xC0) {
			result += 2;
		} else if ((c & 0xF0) == 0xE0) {
			result += 3;
		} else if ((c & 0xF8) == 0xF0) {
			result += 4;
		} else if ((c & 0xFC) == 0xF8) {
			result += 5;
		} else if ((c & 0xFE) == 0xFC) {
			result += 6;
		} else if ((c & 0xFF) == 0xFE) {
			result += 7;
		}
		if (result>maxlength) {
			result=maxlength;
		}
		*uch = cha_utf8_unichar_at(chrs+cur_x_next, chrs+maxlength);
	}
	return result;
}

