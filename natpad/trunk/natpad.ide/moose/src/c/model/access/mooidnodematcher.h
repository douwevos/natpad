/*
   File:    mooidnodematcher.h
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


#ifndef MOOIDNODEMATCHER_H_
#define MOOIDNODEMATCHER_H_


#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_ID_NODE_MATCHER              (moo_id_node_matcher_get_type())
#define MOO_ID_NODE_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_id_node_matcher_get_type(), MooIdNodeMatcher))
#define MOO_ID_NODE_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ID_NODE_MATCHER, MooIdNodeMatcherClass))
#define MOO_IS_ID_NODE_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ID_NODE_MATCHER))
#define MOO_IS_ID_NODE_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ID_NODE_MATCHER))
#define MOO_ID_NODE_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ID_NODE_MATCHER, MooIdNodeMatcherClass))
#define MOO_ID_NODE_MATCHER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_ID_NODE_MATCHER, MooIdNodeMatcherPrivate))


typedef struct _MooIdNodeMatcher               MooIdNodeMatcher;
typedef struct _MooIdNodeMatcherPrivate        MooIdNodeMatcherPrivate;
typedef struct _MooIdNodeMatcherClass          MooIdNodeMatcherClass;


struct _MooIdNodeMatcher {
	GObject parent;
	MooIdNodeMatcherPrivate *priv;
};

struct _MooIdNodeMatcherClass {
	GObjectClass parent_class;
};


GType moo_id_node_matcher_get_type();

MooIdNodeMatcher *moo_id_node_matcher_new(long long id);

void moo_id_node_matcher_set_id(MooIdNodeMatcher *node_matcher, long long id);


G_END_DECLS



#endif /* MOOIDNODEMATCHER_H_ */
