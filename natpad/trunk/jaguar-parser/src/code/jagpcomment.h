/*
   File:    jagpcomment.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 14, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef CODE_JAGPCOMMENT_H_
#define CODE_JAGPCOMMENT_H_

#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum _JagPCommentStyle JagPCommentStyle;

enum _JagPCommentStyle {
    JAGP_COMMENT_STYLE_LINE,
	JAGP_COMMENT_STYLE_BLOCK,
	JAGP_COMMENT_STYLE_JAVADOC
};


#define JAGP_TYPE_COMMENT              (jagp_comment_get_type())
#define JAGP_COMMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_comment_get_type(), JagPComment))
#define JAGP_COMMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_COMMENT, JagPCommentClass))
#define JAGP_IS_COMMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_COMMENT))
#define JAGP_IS_COMMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_COMMENT))
#define JAGP_COMMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_COMMENT, JagPCommentClass))


typedef struct _JagPComment               JagPComment;
typedef struct _JagPCommentPrivate        JagPCommentPrivate;
typedef struct _JagPCommentClass          JagPCommentClass;


struct _JagPComment {
	GObject parent;
};

struct _JagPCommentClass {
	GObjectClass parent_class;
};


GType jagp_comment_get_type();

JagPComment *jagp_comment_new();

CatStringWo *jagp_comment_get_text(JagPComment *comment);
int jagp_comment_get_source_pos(JagPComment *comment, int index);
JagPCommentStyle jagp_comment_get_style(JagPComment *comment);
gboolean jagp_comment_is_deprecated(JagPComment *comment);

G_END_DECLS

#endif /* CODE_JAGPCOMMENT_H_ */
