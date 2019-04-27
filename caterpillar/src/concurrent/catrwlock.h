/*
   File:    catrwlock.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 7, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef CATRWLOCK_H_
#define CATRWLOCK_H_

#include <glib-object.h>
#include <stdint.h>

G_BEGIN_DECLS

#define CAT_TYPE_RW_LOCK              (cat_rw_lock_get_type())
#define CAT_RW_LOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_rw_lock_get_type(), CatRWLock))
#define CAT_RW_LOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_RW_LOCK, CatRWLockClass))
#define CAT_IS_RW_LOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_RW_LOCK))
#define CAT_IS_RW_LOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_RW_LOCK))
#define CAT_RW_LOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_RW_LOCK, CatRWLockClass))

typedef struct _CatRWLock               CatRWLock;
typedef struct _CatRWLockPrivate        CatRWLockPrivate;
typedef struct _CatRWLockClass          CatRWLockClass;


struct _CatRWLock {
	GObject parent;
};

struct _CatRWLockClass {
	GObjectClass parent_class;
};


GType cat_rw_lock_get_type();

CatRWLock *cat_rw_lock_new();


void cat_rw_lock_read_lock(CatRWLock *lock);

void cat_rw_lock_write_lock(CatRWLock *lock);

gboolean cat_rw_lock_read_trylock(CatRWLock *lock);

gboolean cat_rw_lock_write_trylock(CatRWLock *lock);

void cat_rw_lock_read_unlock(CatRWLock *lock);

void cat_rw_lock_write_unlock(CatRWLock *lock);


G_END_DECLS


#endif /* CATRWLOCK_H_ */
