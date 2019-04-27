/*
   File:    moonodelayout.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 25, 2013
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

#ifndef MOONODELAYOUT_H_
#define MOONODELAYOUT_H_

#include "render/moonoderenderregistry.h"
#include <moose.h>
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MOO_TYPE_NODE_LAYOUT              (moo_node_layout_get_type())
#define MOO_NODE_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_node_layout_get_type(), MooNodeLayout))
#define MOO_NODE_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_NODE_LAYOUT, MooNodeLayoutClass))
#define MOO_IS_NODE_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_NODE_LAYOUT))
#define MOO_IS_NODE_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_NODE_LAYOUT))
#define MOO_NODE_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_NODE_LAYOUT, MooNodeLayoutClass))

typedef struct _MooNodeLayout               MooNodeLayout;
typedef struct _MooNodeLayoutPrivate        MooNodeLayoutPrivate;
typedef struct _MooNodeLayoutClass          MooNodeLayoutClass;

typedef enum _MooNodeLayoutZone             MooNodeLayoutZone;

enum _MooNodeLayoutZone {
	MOO_NODE_LAYOUT_ZONE_NONE,
	MOO_NODE_LAYOUT_ZONE_EXPANDER,
	MOO_NODE_LAYOUT_ZONE_ICON,
	MOO_NODE_LAYOUT_ZONE_NAME,
	MOO_NODE_LAYOUT_ZONE_EXTRA
};

struct _MooNodeLayout {
	GObject parent;
	GIcon *icon;
	cairo_surface_t *surface;
};

struct _MooNodeLayoutClass {
	GObjectClass parent_class;
};


GType moo_node_layout_get_type();

MooNodeLayout *moo_node_layout_new(MooNodeWo *node, int tree_level);

void moo_node_layout_set_node(MooNodeLayout *node_layout, MooNodeWo *node);

void moo_node_layout_refresh_renderer_list(MooNodeLayout *node_layout, MooNodeRenderRegistry *render_registry);

void moo_node_layout_update_layout(MooNodeLayout *node_layout, cairo_t *cairo, int ypos, int node_height);

void moo_node_layout_paint(MooNodeLayout *node_layout, PangoContext *pango_context, PangoLayout *pango_layout, double font_height, cairo_t *cairo, gboolean is_cursor);

gboolean moo_node_layout_is_a_map(MooNodeLayout *node_layout);

MooNodeLayoutZone moo_node_layout_get_zone(MooNodeLayout *node_layout, int xpos, int ypos);

int moo_node_layout_get_y(MooNodeLayout *node_layout);
int moo_node_layout_get_x(MooNodeLayout *node_layout);
int moo_node_layout_get_width(MooNodeLayout *node_layout);
int moo_node_layout_get_height(MooNodeLayout *node_layout);
void moo_node_layout_get_bounds(MooNodeLayout *node_layout, GdkRectangle *bounds);

MooNodeWo *moo_node_layout_get_node(MooNodeLayout *node_layout);

int moo_node_layout_get_font_height(MooNodeLayout *node_layout);

gboolean moo_node_layout_is_expanded(MooNodeLayout *node_layout);
gboolean moo_node_layout_toggle_expanded(MooNodeLayout *node_layout);

gboolean moo_node_layout_toggle_selected(MooNodeLayout *node_layout);
gboolean moo_node_layout_is_selected(MooNodeLayout *node_layout);
gboolean moo_node_layout_set_selected(MooNodeLayout *node_layout, gboolean selected);

CatLong *moo_node_layout_get_node_key(MooNodeLayout *node_layout);

void moo_node_layout_mark_for_repaint(MooNodeLayout *node_layout);

void moo_node_layout_set_children(MooNodeLayout *node_layout, CatArrayWo *new_children);

CatArrayWo *moo_node_layout_get_children(MooNodeLayout *node_layout);

int moo_node_layout_get_tree_level(MooNodeLayout *node_layout);

G_END_DECLS

#endif /* MOONODELAYOUT_H_ */
