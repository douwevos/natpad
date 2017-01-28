/*
   File:    drasublinecache.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Mar 28, 2015
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

#ifndef DRASUBLINECACHE_H_
#define DRASUBLINECACHE_H_

#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_SUB_LINE_CACHE              (dra_sub_line_cache_get_type())
#define DRA_SUB_LINE_CACHE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_sub_line_cache_get_type(), DraSubLineCache))
#define DRA_SUB_LINE_CACHE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_SUB_LINE_CACHE, DraSubLineCacheClass))
#define DRA_IS_SUB_LINE_CACHE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_SUB_LINE_CACHE))
#define DRA_IS_SUB_LINE_CACHE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_SUB_LINE_CACHE))
#define DRA_SUB_LINE_CACHE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_SUB_LINE_CACHE, DraSubLineCacheClass))


typedef struct _DraSubLineCache               DraSubLineCache;
typedef struct _DraSubLineCachePrivate        DraSubLineCachePrivate;
typedef struct _DraSubLineCacheClass          DraSubLineCacheClass;


struct _DraSubLineCache {
	ChaSubLineCache parent;
};

struct _DraSubLineCacheClass {
	ChaSubLineCacheClass parent_class;
};


GType dra_sub_line_cache_get_type();

DraSubLineCache *dra_sub_line_cache_new();

G_END_DECLS

#endif /* DRASUBLINECACHE_H_ */
