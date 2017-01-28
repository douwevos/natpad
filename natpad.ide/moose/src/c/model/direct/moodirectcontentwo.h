/*
   File:    moodirectcontentwo.h
   Project: moose
   Author:  Douwe Vos
   Date:    Oct 4, 2015
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

#ifndef MODEL_DIRECT_MOODIRECTCONTENTWO_H_
#define MODEL_DIRECT_MOODIRECTCONTENTWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_DIRECT_CONTENT_WO              (moo_direct_content_wo_get_type())
#define MOO_DIRECT_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_direct_content_wo_get_type(), MooDirectContentWo))
#define MOO_DIRECT_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_DIRECT_CONTENT_WO, MooDirectContentWoClass))
#define MOO_IS_DIRECT_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_DIRECT_CONTENT_WO))
#define MOO_IS_DIRECT_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_DIRECT_CONTENT_WO))
#define MOO_DIRECT_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_DIRECT_CONTENT_WO, MooDirectContentWoClass))


typedef struct _MooDirectContentWo               MooDirectContentWo;
typedef struct _MooDirectContentWoPrivate        MooDirectContentWoPrivate;
typedef struct _MooDirectContentWoClass          MooDirectContentWoClass;


struct _MooDirectContentWo {
	CatWo parent;
};

struct _MooDirectContentWoClass {
	CatWoClass parent_class;
};


GType moo_direct_content_wo_get_type();

MooDirectContentWo *moo_direct_content_wo_new();

CAT_WO_BASE_H(MooDirectContentWo,moo_direct_content_wo);

CatStringWo *moo_direct_content_wo_key();


G_END_DECLS

#endif /* MODEL_DIRECT_MOODIRECTCONTENTWO_H_ */
