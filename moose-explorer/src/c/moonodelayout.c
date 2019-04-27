/*
   File:    moonodelayout.c
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

#include "moonodelayout.h"
#include "render/mooinoderenderer.h"
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooNodeLayout"
#include <logging/catlog.h>

struct _MooNodeLayoutPrivate {
	MooNodeWo *node;
	CatLong *key;
	gboolean paint_as_map;
	int tree_level;
	int node_height;
	gboolean is_expanded;
	gboolean is_selected;
	gboolean needs_repaint;
	gboolean boundaries_invalid;
	GdkRectangle boundaries;
	CatArrayWo *e_renderers;
	CatStringWo *a_cached_name;
	int name_width;
	int error_width;
	CatArrayWo *e_children;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooNodeLayout, moo_node_layout, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_node_layout_class_init(MooNodeLayoutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_node_layout_init(MooNodeLayout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooNodeLayout *instance = MOO_NODE_LAYOUT(object);
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(instance);
	cat_unref_ptr(priv->e_children);
	cat_unref_ptr(priv->node);
	cat_unref_ptr(priv->key);
	cat_unref_ptr(priv->e_renderers);
	cat_unref_ptr(priv->a_cached_name);
	G_OBJECT_CLASS(moo_node_layout_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_node_layout_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooNodeLayout *moo_node_layout_new(MooNodeWo *node, int tree_level) {
	MooNodeLayout *result = g_object_new(MOO_TYPE_NODE_LAYOUT, NULL);
	result->icon = NULL;
	cat_ref_anounce(result);
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(result);
	priv->node = cat_ref_ptr(node);
	if (node) {
		long long id = moo_node_wo_get_unique_id(node);
		priv->key = cat_long_new(id);
	} else {
		priv->key = NULL;
	}
	priv->tree_level = tree_level;
	priv->node_height = 0;
	priv->is_expanded = FALSE;
	priv->is_selected = FALSE;
	priv->needs_repaint = TRUE;
	priv->boundaries_invalid = TRUE;
	priv->boundaries.x = 0;
	priv->boundaries.y = 0;
	priv->boundaries.width = 0;
	priv->boundaries.height = 0;
	priv->e_renderers = NULL;
	priv->a_cached_name = cat_string_wo_anchor(cat_string_wo_new(), 0);
	priv->name_width = 0;
	priv->error_width = 0;
	priv->e_children = NULL;
	return result;
}

void moo_node_layout_set_node(MooNodeLayout *node_layout, MooNodeWo *node) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	if (priv->node!=node) {
		cat_ref_swap(priv->node, node);
		priv->boundaries_invalid = TRUE;
		if (node) {
			long long id = moo_node_wo_get_unique_id(node);
			cat_unref_ptr(priv->key);
			priv->key = cat_long_new(id);
		} else {
			priv->key = NULL;
		}
	}
}

void moo_node_layout_refresh_renderer_list(MooNodeLayout *node_layout, MooNodeRenderRegistry *render_registry) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	if (priv->e_renderers==NULL) {
		priv->e_renderers = moo_node_render_registry_create_renderers_for_node(render_registry, priv->node);
	}
}

void moo_node_layout_update_layout(MooNodeLayout *node_layout, cairo_t *cairo, int ypos, int node_height) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	GdkRectangle oldBound = priv->boundaries;

	priv->boundaries.y = ypos;
	priv->boundaries.x = priv->tree_level*node_height;
	priv->boundaries.height = node_height;
	priv->node_height = node_height;


	priv->paint_as_map = FALSE;
	if ((moo_node_wo_child_count(priv->node)>0) || moo_node_wo_get_expander(priv->node)!=NULL) {
		priv->paint_as_map = TRUE;
	} else {
		MooResourceContentWo *resource = (MooResourceContentWo *) moo_node_wo_get_content(priv->node, moo_resource_content_wo_key());
		if (resource!=NULL) {
			VipNode *viperNode = moo_resource_content_wo_get_viper_node(resource);
			if (viperNode) {
				VipIResource *vip_resource = vip_node_get_content(viperNode);
				if (VIP_IS_IMAP(vip_resource)) {
					priv->paint_as_map = TRUE;
				}
			}
		}
	}

	CatStringWo *node_name = moo_node_wo_get_name(priv->node);
	if (!cat_string_wo_equal(node_name, priv->a_cached_name) || priv->boundaries.height!=oldBound.height) {
		cat_ref_swap(priv->a_cached_name, node_name);
		PangoLayout *layout = pango_cairo_create_layout(cairo);
		pango_layout_set_text(layout, cat_string_wo_getchars(node_name), cat_string_wo_length(node_name));
		PangoRectangle extents;
		pango_layout_get_extents(layout, NULL, &extents);
		priv->needs_repaint = TRUE;
		priv->name_width = extents.width/PANGO_SCALE;
	}
	if (priv->e_renderers) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_renderers);
		while(cat_iiterator_has_next(iter)) {
			MooINodeRenderer *renderer = cat_iiterator_next(iter);
			moo_inode_renderer_update_layout(renderer, node_layout);
		}
		cat_unref_ptr(iter);
	}
	priv->boundaries.width = priv->name_width;
	priv->boundaries.width += node_height*2;

//	int errorCount = node.getErrorCount();
//	if (errorCount>0) {
//
//		Font of = gfx.getFont();
//		Font f2 = new Font(of.getName(), Font.PLAIN, of.getSize()-3);
//		gfx.setFont(f2);
//		String txt = ""+errorCount;
//		Rectangle2D stringBounds = gfx.getFontMetrics().getStringBounds(txt, gfx);
//		errorWidth = 10 + (int) Math.ceil(stringBounds.getWidth());
//		boundaries.width += errorWidth+4;
//		gfx.setFont(of);
//
//
//	} else {
//		errorWidth = 0;
//	}

	if (!priv->needs_repaint) {
		if ((oldBound.x!=priv->boundaries.x) ||
				(oldBound.y!=priv->boundaries.y) ||
				(oldBound.width!=priv->boundaries.width) ||
				(oldBound.height!=priv->boundaries.height)) {
			priv->needs_repaint = TRUE;
		}
	}
}



static void l_draw_expander(cairo_t *cr, gint x, gint y, double expander_size, gboolean is_expanded) {
	gint line_width;
	double vertical_overshoot;
	int diameter;
	double radius;
	double interp; /* interpolation factor for center position */
	double x_double_horz, y_double_horz;
	double x_double_vert, y_double_vert;
	double x_double, y_double;
	gint degrees = 0;

	cairo_save(cr);


	line_width = MAX (1, expander_size/9);

	if (is_expanded) {
		degrees = 90;
		interp = 1.0;
	} else {
		degrees = 0;
		interp = 0.0;
	}

	/* Compute distance that the stroke extends beyonds the end
	 * of the triangle we draw.
	 */
	vertical_overshoot = line_width / 2.0 * (1. / tan(G_PI / 8));

	/* For odd line widths, we end the vertical line of the triangle
	 * at a half pixel, so we round differently.
	 */
	if (line_width % 2 == 1)
		vertical_overshoot = ceil(0.5 + vertical_overshoot) - 0.5;
	else
		vertical_overshoot = ceil(vertical_overshoot);

	/* Adjust the size of the triangle we draw so that the entire stroke fits
	 */
	diameter = MAX (3, expander_size - 2 * vertical_overshoot);

	/* If the line width is odd, we want the diameter to be even,
	 * and vice versa, so force the sum to be odd. This relationship
	 * makes the point of the triangle look right.
	 */
	diameter -= (1 - (diameter + line_width) % 2);

	radius = diameter / 2.;

	/* Adjust the center so that the stroke is properly aligned with
	 * the pixel grid. The center adjustment is different for the
	 * horizontal and vertical orientations. For intermediate positions
	 * we interpolate between the two.
	 */
	x_double_vert = floor(x - (radius + line_width) / 2.)
			+ (radius + line_width) / 2.;
	y_double_vert = y - 0.5;

	x_double_horz = x - 0.5;
	y_double_horz = floor(y - (radius + line_width) / 2.)
			+ (radius + line_width) / 2.;

	x_double = x_double_vert * (1 - interp) + x_double_horz * interp;
	y_double = y_double_vert * (1 - interp) + y_double_horz * interp;

	cairo_translate(cr, x_double, y_double);
	cairo_rotate(cr, degrees * G_PI / 180);

	cairo_move_to(cr, -radius / 2., -radius);
	cairo_line_to(cr, radius / 2., 0);
	cairo_line_to(cr, -radius / 2., radius);
	cairo_close_path(cr);

	cairo_set_line_width(cr, line_width);

	cairo_set_source_rgb(cr, 0.1875, 0.1875, 0.1875);

	cairo_fill(cr);

//	gdk_cairo_set_source_color(cr, &style->fg[state_type]);
//	cairo_stroke(cr);
	cairo_restore(cr);
}

void moo_node_layout_paint(MooNodeLayout *node_layout, PangoContext *pango_context, PangoLayout *pango_layout, double font_height, cairo_t *cairo, gboolean is_cursor) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);

	double clip_x, clip_y, clip_width, clip_height;
	cairo_clip_extents(cairo, &clip_x, &clip_y, &clip_width, &clip_height);

	int old_y = priv->boundaries.y;
	priv->boundaries.y =0;

//		g.setClip(null);
	if (priv->is_selected) {
		cairo_set_source_rgb(cairo, 0.6745, 0.803, 0.5411);
	} else {
		cairo_set_source_rgb(cairo, 1.0, 1.0, 1.0);
	}
	cairo_rectangle(cairo, clip_x, (double) priv->boundaries.y, clip_width, (double) priv->boundaries.height);
	cairo_fill(cairo);

	cairo_set_source_rgb(cairo, 0.75, 0.75, 0.75);

	if (is_cursor) {
		cairo_rectangle(cairo, clip_x, (double) priv->boundaries.y, clip_width-1, (double) priv->boundaries.height-1);
		cairo_stroke(cairo);
	}

	if (priv->paint_as_map > 0)  {
		double exp_size = priv->node_height;
		double exp_off = (priv->node_height) * 0.5;
//			gtk_render_expander(sctx, cairo_back, trx+exp_off-font_height,+exp_off, exp_size, exp_size);

		cairo_set_source_rgb(cairo, 0.25, 0.25, 0.25);

		l_draw_expander(cairo, priv->boundaries.x+exp_off,priv->boundaries.y+exp_off, exp_size, priv->is_expanded);
	}

	cairo_set_source_rgb(cairo, 0.0, 0.0, 0.0);
//		g.drawRect(boundaries.x, boundaries.y, boundaries.width, boundaries.height);

	if (priv->e_renderers) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_renderers);
		while(cat_iiterator_has_next(iter)) {
			MooINodeRenderer *renderer = cat_iiterator_next(iter);
			moo_inode_renderer_paint(renderer, cairo, node_layout);
		}
		cat_unref_ptr(iter);
	}

	cairo_set_source_rgb(cairo, 0.0, 0.0, 0.0);

//	PangoLayout *pango_layout = pango_cairo_create_layout(cairo);

	pango_layout_set_text(pango_layout, cat_string_wo_getchars(priv->a_cached_name), cat_string_wo_length(priv->a_cached_name));


	double top_margin = (priv->node_height-font_height)/2.0;

	cairo_move_to(cairo, priv->node_height*2+priv->boundaries.x, top_margin+priv->boundaries.y);
	pango_cairo_show_layout(cairo, pango_layout);

//	int errorCount = node.getErrorCount();
//	if (errorCount>0) {
//		Graphics2D gl = (Graphics2D) g.create();
//		String txt = ""+errorCount;
//		gl.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
//		if (node.getRecursiveErrorCount().localCount>0) {
//			gl.setColor(new Color(255,0,0));
//		} else {
//			gl.setColor(new Color(255, 128, 128));
//		}
//		int xpo = fontHeight*2+boundaries.x+nameWidth+4;
//		gl.fillRoundRect(xpo, boundaries.y, errorWidth-2, fontHeight-1, 10, 10);
//		gl.setColor(new Color(255,255,255));
//		Font of = gl.getFont();
//		Font f2 = new Font(of.getName(), Font.PLAIN, of.getSize()-3);
//		gl.setFont(f2);
//		gl.drawString(txt, xpo+4, boundaries.y+ascent-2);
//	}

	priv->boundaries.y = old_y;
}

gboolean moo_node_layout_is_a_map(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->paint_as_map;
}

MooNodeLayoutZone moo_node_layout_get_zone(MooNodeLayout *node_layout, int xpos, int ypos) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	xpos -= priv->boundaries.x;
	if (xpos<0 || xpos>=priv->boundaries.width) {
		return MOO_NODE_LAYOUT_ZONE_NONE;
	}
	if (xpos<=priv->node_height) {
		return MOO_NODE_LAYOUT_ZONE_EXPANDER;
	}
	if (xpos<=2*priv->node_height) {
		return MOO_NODE_LAYOUT_ZONE_ICON;
	}
	if (xpos<=2*priv->node_height+priv->name_width) {
		return MOO_NODE_LAYOUT_ZONE_ICON;
	}
	return MOO_NODE_LAYOUT_ZONE_EXTRA;
}

int moo_node_layout_get_y(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->boundaries.y;
}

int moo_node_layout_get_x(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->boundaries.x;
}

int moo_node_layout_get_width(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->boundaries.width;
}

int moo_node_layout_get_height(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->boundaries.height;
}

void moo_node_layout_get_bounds(MooNodeLayout *node_layout, GdkRectangle *bounds) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	*bounds = priv->boundaries;
}

MooNodeWo *moo_node_layout_get_node(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->node;
}

int moo_node_layout_get_font_height(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->node_height;
}

gboolean moo_node_layout_is_expanded(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->is_expanded;
}

gboolean moo_node_layout_toggle_expanded(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	priv->is_expanded = !priv->is_expanded;
	priv->needs_repaint = TRUE;
	return priv->is_expanded;
}

gboolean moo_node_layout_toggle_selected(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	priv->is_selected = !priv->is_selected;
	priv->needs_repaint = TRUE;
	return priv->is_selected;
}

gboolean moo_node_layout_is_selected(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->is_selected;
}

gboolean moo_node_layout_set_selected(MooNodeLayout *node_layout, gboolean selected) {
	gboolean result = FALSE;
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	if (priv->is_selected != selected) {
		result = TRUE;
		priv->is_selected = selected;
		priv->needs_repaint = TRUE;
	}
	return result;
}

CatLong *moo_node_layout_get_node_key(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->key;
}

void moo_node_layout_mark_for_repaint(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	priv->needs_repaint = TRUE;
}


void moo_node_layout_set_children(MooNodeLayout *node_layout, CatArrayWo *new_children) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	cat_ref_swap(priv->e_children, new_children);
}

CatArrayWo *moo_node_layout_get_children(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->e_children;
}

int moo_node_layout_get_tree_level(MooNodeLayout *node_layout) {
	MooNodeLayoutPrivate *priv = moo_node_layout_get_instance_private(node_layout);
	return priv->tree_level;
}
