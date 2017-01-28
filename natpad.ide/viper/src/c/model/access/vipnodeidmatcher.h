/*
   File:    vipnodeidmatcher.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 20, 2013
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


#ifndef VIPNODEIDMATCHER_H_
#define VIPNODEIDMATCHER_H_


#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_NODE_ID_MATCHER              (vip_node_id_matcher_get_type())
#define VIP_NODE_ID_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_node_id_matcher_get_type(), VipNodeIdMatcher))
#define VIP_NODE_ID_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_NODE_ID_MATCHER, VipNodeIdMatcherClass))
#define VIP_IS_NODE_ID_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_NODE_ID_MATCHER))
#define VIP_IS_NODE_ID_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_NODE_ID_MATCHER))
#define VIP_NODE_ID_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_NODE_ID_MATCHER, VipNodeIdMatcherClass))
#define VIP_NODE_ID_MATCHER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_NODE_ID_MATCHER, VipNodeIdMatcherPrivate))


typedef struct _VipNodeIdMatcher               VipNodeIdMatcher;
typedef struct _VipNodeIdMatcherPrivate        VipNodeIdMatcherPrivate;
typedef struct _VipNodeIdMatcherClass          VipNodeIdMatcherClass;


struct _VipNodeIdMatcher {
	GObject parent;
	VipNodeIdMatcherPrivate *priv;
};

struct _VipNodeIdMatcherClass {
	GObjectClass parent_class;
};


GType vip_node_id_matcher_get_type();

VipNodeIdMatcher *vip_node_id_matcher_new(long long viper_id);

void vip_node_id_matcher_set_id(VipNodeIdMatcher *matcher, long long viper_id);

G_END_DECLS

#endif /* VIPNODEIDMATCHER_H_ */
