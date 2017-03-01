/*
   File:    turlineview.c
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 14, 2017
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

#include "turlineview.h"
#include "../model/turinode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "TurLineView"
#include <logging/catlog.h>

struct _TurLineViewPrivate {
	TurLine *line;
	int line_height;
	int line_width;
	TurSurfaceWrapper *surface_wrapper;
	CatStringWo *text;
	long long int y_pos;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TurLineView, tur_line_view, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TurLineView)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void tur_line_view_class_init(TurLineViewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void tur_line_view_init(TurLineView *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	TurLineView *instance = TUR_LINE_VIEW(object);
//	TurLineViewPrivate *priv = tur_line_view_get_instance_private(instance);
	G_OBJECT_CLASS(tur_line_view_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(tur_line_view_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TurLineView *tur_line_view_new(TurLine *line) {
	TurLineView *result = g_object_new(TUR_TYPE_LINE_VIEW, NULL);
	cat_ref_anounce(result);
	TurLineViewPrivate *priv = tur_line_view_get_instance_private(result);
	priv->line = cat_ref_ptr(line);
	priv->line_height = 0;
	priv->surface_wrapper = NULL;
	return result;
}

gboolean tur_line_view_defined_by(TurLineView *line_view, TurLine *line) {
	TurLineViewPrivate *priv = tur_line_view_get_instance_private(line_view);
	return priv->line == line;
}

gboolean tur_line_view_update(TurLineView *line_view, const TurLineViewUpdate update) {
	TurLineViewPrivate *priv = tur_line_view_get_instance_private(line_view);
	gboolean do_update = FALSE;
	if (priv->line_height!=update.line_height) {
		priv->line_height = update.line_height;
		do_update = TRUE;
	}
	if (priv->line_width!=update.view_width) {
		priv->line_width = update.view_width;
		do_update = TRUE;
	}

	TurINode *node = tur_line_get_node(priv->line);
	TurINodeInterface *iface = TUR_INODE_GET_INTERFACE(node);
	CatStringWo *name = iface->name(node);
	if (!cat_string_wo_equal(priv->text, name)) {
		cat_ref_swap(priv->text, name);
		do_update = TRUE;
	}

	if (do_update) {
		if (priv->surface_wrapper) {
			tur_surface_pool_release(update.surface_pool, priv->surface_wrapper);
		}
		priv->surface_wrapper = tur_surface_pool_obtain(update.surface_pool, update.window);

		cairo_t *cr = tur_surface_wrapper_get_context(priv->surface_wrapper);

		pango_layout_set_text(update.pango_layout, cat_string_wo_getchars(priv->text), cat_string_wo_length(priv->text));
		cairo_move_to(cr, priv->line_height, 0);
		cairo_set_source_rgb(cr, 1, 1, 0);
		pango_cairo_show_layout(cr, update.pango_layout);
	}

	if (priv->y_pos!=update.y_pos) {
		priv->y_pos = update.y_pos;
		do_update = TRUE;
	}

	return do_update;
}

void tur_line_view_draw(TurLineView *line_view, cairo_t *cr) {
	TurLineViewPrivate *priv = tur_line_view_get_instance_private(line_view);
	cairo_save(cr);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, 0,0);
	cairo_line_to(cr, 1500, 400);
	cairo_stroke(cr);
	cairo_surface_t *surface = cha_cairo_surface_wrapper_get(priv->surface_wrapper);
	cat_log_debug("line_width=%d, line_height=%d, ypos=%d, text=%O", priv->line_width, priv->line_height, (int) priv->y_pos, priv->text);
	if (surface) {
		cat_log_debug("hit");
		cairo_set_source_surface(cr, surface, 0, (double) priv->y_pos);
		cairo_rectangle(cr, 0, (double) priv->y_pos, priv->line_width, priv->line_height);
		cairo_fill(cr);
	}
	cairo_restore(cr);
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





/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TurLineView *instance = TUR_LINE_VIEW(self);
	TurLineViewPrivate *priv = tur_line_view_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
