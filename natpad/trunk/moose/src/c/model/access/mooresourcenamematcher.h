/*
   File:    mooresourcenamematcher.h
   Project: moose
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


#ifndef MOORESOURCENAMEMATCHER_H_
#define MOORESOURCENAMEMATCHER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_RESOURCE_NAME_MATCHER              (moo_resource_name_matcher_get_type())
#define MOO_RESOURCE_NAME_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_resource_name_matcher_get_type(), MooResourceNameMatcher))
#define MOO_RESOURCE_NAME_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_RESOURCE_NAME_MATCHER, MooResourceNameMatcherClass))
#define MOO_IS_RESOURCE_NAME_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_RESOURCE_NAME_MATCHER))
#define MOO_IS_RESOURCE_NAME_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_RESOURCE_NAME_MATCHER))
#define MOO_RESOURCE_NAME_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_RESOURCE_NAME_MATCHER, MooResourceNameMatcherClass))


typedef struct _MooResourceNameMatcher               MooResourceNameMatcher;
typedef struct _MooResourceNameMatcherClass          MooResourceNameMatcherClass;


struct _MooResourceNameMatcher {
	GObject parent;
};

struct _MooResourceNameMatcherClass {
	GObjectClass parent_class;
};


GType moo_resource_name_matcher_get_type();

MooResourceNameMatcher *moo_resource_name_matcher_new();

G_END_DECLS

#endif /* MOORESOURCENAMEMATCHER_H_ */
