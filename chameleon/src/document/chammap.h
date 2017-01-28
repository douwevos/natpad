/*
   File:    chammap.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Apr 17, 2015
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

#ifndef CHAMMAP_H_
#define CHAMMAP_H_

#include <gio/gio.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_MMAP              (cha_mmap_get_type())
#define CHA_MMAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_mmap_get_type(), ChaMMap))
#define CHA_MMAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_MMAP, ChaMMapClass))
#define CHA_IS_MMAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_MMAP))
#define CHA_IS_MMAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_MMAP))
#define CHA_MMAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_MMAP, ChaMMapClass))


typedef struct _ChaMMap               ChaMMap;
typedef struct _ChaMMapPrivate        ChaMMapPrivate;
typedef struct _ChaMMapClass          ChaMMapClass;


struct _ChaMMap {
	GObject parent;
};

struct _ChaMMapClass {
	GObjectClass parent_class;
};


GType cha_mmap_get_type();

ChaMMap *cha_mmap_new(GFile *file);

void *cha_mmap_get_data(ChaMMap *map);

uint64_t cha_mmap_get_length(ChaMMap *map);

void cha_mmap_call_dontneed(ChaMMap *map);

G_END_DECLS

#endif /* CHAMMAP_H_ */
