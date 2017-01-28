/*
   File:    vipnodenamematcher.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
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


#ifndef VIPNODENAMEMATCHER_H_
#define VIPNODENAMEMATCHER_H_

#include "vipinodematcher.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_NODE_NAME_MATCHER              (vip_node_name_matcher_get_type())
#define VIP_NODE_NAME_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_node_name_matcher_get_type(), VipNodeNameMatcher))
#define VIP_NODE_NAME_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_NODE_NAME_MATCHER, VipNodeNameMatcherClass))
#define VIP_IS_NODE_NAME_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_NODE_NAME_MATCHER))
#define VIP_IS_NODE_NAME_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_NODE_NAME_MATCHER))
#define VIP_NODE_NAME_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_NODE_NAME_MATCHER, VipNodeNameMatcherClass))
#define VIP_NODE_NAME_MATCHER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_NODE_NAME_MATCHER, VipNodeNameMatcherPrivate))


typedef struct _VipNodeNameMatcher               VipNodeNameMatcher;
typedef struct _VipNodeNameMatcherPrivate        VipNodeNameMatcherPrivate;
typedef struct _VipNodeNameMatcherClass          VipNodeNameMatcherClass;


struct _VipNodeNameMatcher {
	GObject parent;
	VipNodeNameMatcherPrivate *priv;
};

struct _VipNodeNameMatcherClass {
	GObjectClass parent_class;
};


GType vip_node_name_matcher_get_type();

VipNodeNameMatcher *vip_node_name_matcher_new(CatStringWo *name);

void vip_node_name_matcher_set_match_name(VipNodeNameMatcher *matcher, CatStringWo *name);

G_END_DECLS

#endif /* VIPNODENAMEMATCHER_H_ */
