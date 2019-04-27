/*
   File:    catlock.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jul 1, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#ifndef CATLOCK_H_
#define CATLOCK_H_

#include <glib-object.h>
#include <stdint.h>

G_BEGIN_DECLS

#define CAT_TYPE_LOCK            (cat_lock_get_type())
#define CAT_LOCK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_lock_get_type(), CatLock))
#define CAT_LOCK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_LOCK, CatLockClass))
#define CAT_IS_LOCK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_LOCK))
#define CAT_IS_LOCK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_LOCK))
#define CAT_LOCK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_LOCK, CatLockClass))


typedef struct _CatLock	   	     CatLock;
typedef struct _CatLockPrivate	 CatLockPrivate;
typedef struct _CatLockClass     CatLockClass;


struct _CatLock {
	GObject parent;
};

struct _CatLockClass {
	GObjectClass parent_class;
};


GType cat_lock_get_type();

CatLock *cat_lock_new();


void cat_lock_lock(CatLock *lock);
void cat_lock_unlock(CatLock *lock);

void cat_lock_wait(CatLock *lock);
gboolean cat_lock_wait_timed(CatLock *lock, int64_t ms);

void cat_lock_notify(CatLock *lock);
void cat_lock_notify_all(CatLock *lock);

G_END_DECLS

#endif /* CATLOCK_H_ */
