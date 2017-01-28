/*
   File:    Catbalancedcounter.c
   Project: Catterpillar
   Author:  Douwe Vos
   Date:    Jan 11, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

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

#include "catbalancedcounter.h"
#include "../catlib.h"
#include <stdio.h>

G_DEFINE_TYPE(CatBalancedCounter, cat_balanced_counter, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void cat_balanced_counter_class_init(CatBalancedCounterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void cat_balanced_counter_init(CatBalancedCounter *block_array) {
}

static void _dispose(GObject *object) {
	CatBalancedCounter *balanced_counter = CAT_BALANCED_COUNTER(object);
	cat_free_ptr(balanced_counter->digits);
}


CatBalancedCounter *cat_balanced_counter_new(int digit_count, int sum) {
	CatBalancedCounter *result = g_object_new(CAT_TYPE_BALANCED_COUNTER, NULL);
	cat_ref_anounce(result);
	result->digit_count = digit_count;
	result->sum = sum;
	result->first = TRUE;
#ifdef DEBUG
	printf("digit_count=%d, sum=%d\n", digit_count, sum);
#endif
	if (digit_count>0) {
		result->digits = (int *) g_malloc(sizeof(int)*digit_count);
		int idx;
		for(idx=1; idx<digit_count; idx++) {
			result->digits[idx] = 0;
		}
		result->digits[0] = sum;
	} else {
		result->digits = NULL;
	}
	return result;
}

gboolean cat_balanced_counter_next(CatBalancedCounter *this) {
	if (this->first) {
		this->first = FALSE;
		return this->digit_count==0;
	}
	if (this->digit_count<=1) {
		return FALSE;
	}
	if (this->digits[0]!=0) {
		this->digits[0]--;
		this->digits[1]++;
		return TRUE;
	}
	int idx;
	for(idx=1; idx<this->digit_count; idx++) {
		if (this->digits[idx]!=0) {
			this->digits[0] = this->digits[idx]-1;
			this->digits[idx] = 0;
			if (idx==this->digit_count-1) {
				return FALSE;
			}
			this->digits[idx+1]++;
			return TRUE;
		}
	}
	return FALSE;
}

void cat_balanced_counter_dump(CatBalancedCounter *this) {
	int idx;
	for(idx=0; idx<this->digit_count; idx++) {
		printf("%d ", this->digits[idx]);
	}
	printf("\n");
}

