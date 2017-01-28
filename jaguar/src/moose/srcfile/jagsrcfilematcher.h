/*
   File:    jagsrcfilematcher.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 23, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef JAGCLASSFILEMATCHER_H_
#define JAGCLASSFILEMATCHER_H_


#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_SRCFILE_MATCHER              (jag_srcfile_matcher_get_type())
#define JAG_SRCFILE_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_srcfile_matcher_get_type(), JagSrcfileMatcher))
#define JAG_SRCFILE_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRCFILE_MATCHER, JagSrcfileMatcherClass))
#define JAG_IS_SRCFILE_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRCFILE_MATCHER))
#define JAG_IS_SRCFILE_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRCFILE_MATCHER))
#define JAG_SRCFILE_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRCFILE_MATCHER, JagSrcfileMatcherClass))
#define JAG_SRCFILE_MATCHER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_SRCFILE_MATCHER, JagSrcfileMatcherPrivate))


typedef struct _JagSrcfileMatcher               JagSrcfileMatcher;
typedef struct _JagSrcfileMatcherPrivate        JagSrcfileMatcherPrivate;
typedef struct _JagSrcfileMatcherClass          JagSrcfileMatcherClass;


struct _JagSrcfileMatcher {
	GObject parent;
	JagSrcfileMatcherPrivate *priv;
};

struct _JagSrcfileMatcherClass {
	GObjectClass parent_class;
};


GType jag_srcfile_matcher_get_type();

JagSrcfileMatcher *jag_srcfile_matcher_new();

void jag_srcfile_matcher_set_match_name(JagSrcfileMatcher *classfile_matcher, CatStringWo *a_newname);

G_END_DECLS

#endif /* JAGCLASSFILEMATCHER_H_ */
