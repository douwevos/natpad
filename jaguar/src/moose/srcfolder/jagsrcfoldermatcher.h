/*
   File:    jagsrcfoldermatcher.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 6, 2013
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

#ifndef JAGSRCFOLDERMATCHER_H_
#define JAGSRCFOLDERMATCHER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_SRC_FOLDER_MATCHER              (jag_src_folder_matcher_get_type())
#define JAG_SRC_FOLDER_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_src_folder_matcher_get_type(), JagSrcFolderMatcher))
#define JAG_SRC_FOLDER_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRC_FOLDER_MATCHER, JagSrcFolderMatcherClass))
#define JAG_IS_SRC_FOLDER_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRC_FOLDER_MATCHER))
#define JAG_IS_SRC_FOLDER_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRC_FOLDER_MATCHER))
#define JAG_SRC_FOLDER_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRC_FOLDER_MATCHER, JagSrcFolderMatcherClass))

typedef struct _JagSrcFolderMatcher               JagSrcFolderMatcher;
typedef struct _JagSrcFolderMatcherClass          JagSrcFolderMatcherClass;

struct _JagSrcFolderMatcher {
	GObject parent;
};

struct _JagSrcFolderMatcherClass {
	GObjectClass parent_class;
};

GType jag_src_folder_matcher_get_type();

JagSrcFolderMatcher *jag_src_folder_matcher_new();

G_END_DECLS

#endif /* JAGSRCFOLDERMATCHER_H_ */
