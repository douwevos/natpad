/*
   File:    catbalancedcounter.h
   Project: Caterpillar
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

#ifndef CATBALANCEDCOUNTER_H_
#define CATBALANCEDCOUNTER_H_

#include <glib-object.h>

G_BEGIN_DECLS


#define CAT_TYPE_BALANCED_COUNTER            (cat_balanced_counter_get_type())
#define CAT_BALANCED_COUNTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_balanced_counter_get_type(), CatBalancedCounter))
#define CAT_BALANCED_COUNTER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_BALANCED_COUNTER, CatBalancedCounterClass))
#define CAT_IS_BALANCED_COUNTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_BALANCED_COUNTER))
#define CAT_IS_BALANCED_COUNTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_BALANCED_COUNTER))
#define CAT_BALANCED_COUNTER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_BALANCED_COUNTER, CatBalancedCounterClass))


typedef struct _CatBalancedCounter        CatBalancedCounter;

typedef struct _CatBalancedCounterClass   CatBalancedCounterClass;


struct _CatBalancedCounter {
	GObject parent;
	int *digits;
	int digit_count;
	int sum;
	gboolean first;
};


struct _CatBalancedCounterClass {
	GObjectClass parent_class;
};


CatBalancedCounter *cat_balanced_counter_new(int digit_count, int sum);

gboolean cat_balanced_counter_next(CatBalancedCounter *th);

void cat_balanced_counter_dump(CatBalancedCounter *th);

G_END_DECLS



#endif /* CATBALANCEDCOUNTER_H_ */
