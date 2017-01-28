/*
   File:    drauowtogglecomment.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 16, 2015
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

#ifndef UOW_DRAUOWTOGGLECOMMENT_H_
#define UOW_DRAUOWTOGGLECOMMENT_H_

#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_UOW_TOGGLE_COMMENT              (dra_uow_toggle_comment_get_type())
#define DRA_UOW_TOGGLE_COMMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_uow_toggle_comment_get_type(), DraUowToggleComment))
#define DRA_UOW_TOGGLE_COMMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_UOW_TOGGLE_COMMENT, DraUowToggleCommentClass))
#define DRA_IS_UOW_TOGGLE_COMMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_UOW_TOGGLE_COMMENT))
#define DRA_IS_UOW_TOGGLE_COMMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_UOW_TOGGLE_COMMENT))
#define DRA_UOW_TOGGLE_COMMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_UOW_TOGGLE_COMMENT, DraUowToggleCommentClass))


typedef struct _DraUowToggleComment               DraUowToggleComment;
typedef struct _DraUowToggleCommentPrivate        DraUowToggleCommentPrivate;
typedef struct _DraUowToggleCommentClass          DraUowToggleCommentClass;


struct _DraUowToggleComment {
	ChaUow parent;
};

struct _DraUowToggleCommentClass {
	ChaUowClass parent_class;
};


GType dra_uow_toggle_comment_get_type();

DraUowToggleComment *dra_uow_toggle_comment_new();

G_END_DECLS

#endif /* UOW_DRAUOWTOGGLECOMMENT_H_ */
