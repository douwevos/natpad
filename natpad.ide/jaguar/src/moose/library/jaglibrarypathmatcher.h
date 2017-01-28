/*
   File:    jaglibrarypathmatcher.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 20, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef JAGLIBRARYPATHMATCHER_H_
#define JAGLIBRARYPATHMATCHER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_LIBRARY_PATH_MATCHER              (jag_library_path_matcher_get_type())
#define JAG_LIBRARY_PATH_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_library_path_matcher_get_type(), JagLibraryPathMatcher))
#define JAG_LIBRARY_PATH_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_LIBRARY_PATH_MATCHER, JagLibraryPathMatcherClass))
#define JAG_IS_LIBRARY_PATH_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_LIBRARY_PATH_MATCHER))
#define JAG_IS_LIBRARY_PATH_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_LIBRARY_PATH_MATCHER))
#define JAG_LIBRARY_PATH_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_LIBRARY_PATH_MATCHER, JagLibraryPathMatcherClass))


typedef struct _JagLibraryPathMatcher               JagLibraryPathMatcher;
typedef struct _JagLibraryPathMatcherPrivate        JagLibraryPathMatcherPrivate;
typedef struct _JagLibraryPathMatcherClass          JagLibraryPathMatcherClass;


struct _JagLibraryPathMatcher {
	GObject parent;
};

struct _JagLibraryPathMatcherClass {
	GObjectClass parent_class;
};


GType jag_library_path_matcher_get_type();

JagLibraryPathMatcher *jag_library_path_matcher_new();

G_END_DECLS

#endif /* JAGLIBRARYPATHMATCHER_H_ */
