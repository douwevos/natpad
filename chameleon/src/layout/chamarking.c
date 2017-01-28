/*
   File:    chamarking.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 18, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chamarking.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaMarking"
#include <logging/catlog.h>

struct _ChaMarkingPrivate {
	int sub_line_idx;
	int x_left;
	int x_right;
	gboolean is_top;
	gboolean is_bottom;
	double alpha;
	int color_index;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaMarking, cha_marking, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaMarking)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_marking_class_init(ChaMarkingClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_marking_init(ChaMarking *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaMarking *instance = CHA_MARKING(object);
//	ChaMarkingPrivate *priv = cha_marking_get_instance_private(instance);
	G_OBJECT_CLASS(cha_marking_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_marking_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaMarking *cha_marking_new(int sub_line_idx, int x_left, int x_right, double alpha, int color_index) {
	ChaMarking *result = g_object_new(CHA_TYPE_MARKING, NULL);
	cat_ref_anounce(result);
	ChaMarkingPrivate *priv = cha_marking_get_instance_private(result);
	priv->sub_line_idx = sub_line_idx;
	priv->x_left = x_left;
	priv->x_right = x_right;
	priv->is_top = FALSE;
	priv->is_bottom = FALSE;
	priv->alpha = alpha;
	priv->color_index = color_index;
	return result;
}

void cha_marking_set_mark_as_top(ChaMarking *marking) {
	ChaMarkingPrivate *priv = cha_marking_get_instance_private(marking);
	priv->is_top = TRUE;
}

void cha_marking_set_mark_as_bottom(ChaMarking *marking) {
	ChaMarkingPrivate *priv = cha_marking_get_instance_private(marking);
	priv->is_bottom = TRUE;
}


gboolean cha_marking_equal(ChaMarking *marking_a, ChaMarking *marking_b) {
	if (marking_a==marking_b) {
		return TRUE;
	} else if ((marking_a==NULL) || (marking_b==NULL)) {
		return FALSE;
	}

	ChaMarkingPrivate *priv_a = cha_marking_get_instance_private(marking_a);
	ChaMarkingPrivate *priv_b = cha_marking_get_instance_private(marking_b);

	return (priv_a->sub_line_idx==priv_b->sub_line_idx)
			&& (priv_a->x_left==priv_b->x_left)
			&& (priv_a->x_right==priv_b->x_right)
			&& (priv_a->is_top==priv_b->is_top)
			&& (priv_a->is_bottom==priv_b->is_bottom)
			&& (priv_a->color_index==priv_b->color_index)
			&& (priv_a->alpha==priv_b->alpha);
}

void cha_marking_paint(ChaMarking *marking, int sub_line_index, cairo_t *cairo, ChaPrefsColorMapWo *color_map, int height) {
	ChaMarkingPrivate *priv = cha_marking_get_instance_private(marking);
	if (priv->sub_line_idx==sub_line_index) {
		cat_log_debug("x=%d %d", priv->x_left, priv->x_right);
		ChaColorEntry cce = cha_prefs_color_map_wo_get_color(color_map, priv->color_index);
		cairo_set_source_rgba(cairo, cce.red, cce.green, cce.blue, priv->alpha);
		cairo_rectangle(cairo, priv->x_left, 0, priv->x_right-priv->x_left, height);
		cairo_fill(cairo);


		gdouble h;
		gdouble s;
		gdouble v;

		gtk_rgb_to_hsv(cce.red, cce.green, cce.blue, &h, &s, &v);
		v -= 0.4;
		if (v<0) {
			v = 0;
		}
		gtk_hsv_to_rgb(h, s, v, &cce.red, &cce.green, &cce.blue);

		double alpha = priv->alpha+0.3;
		if (alpha>1) {
			alpha = 1;
		}

		cairo_set_source_rgba(cairo, cce.red, cce.green, cce.blue, alpha);

		/* left line */
		cairo_move_to(cairo, priv->x_left+0.5, 0);
		cairo_line_to(cairo, priv->x_left+0.5, height);
		cairo_stroke(cairo);

		/* right line */
		cairo_move_to(cairo, priv->x_right-0.5, 0);
		cairo_line_to(cairo, priv->x_right-0.5, height);
		cairo_stroke(cairo);

		if (priv->is_top) {
			cairo_move_to(cairo, priv->x_left+1.5, 0.5);
			cairo_line_to(cairo, priv->x_right-0.5, 0.5);
			cairo_stroke(cairo);
		}

		if (priv->is_bottom) {
			cairo_move_to(cairo, priv->x_left+1.5, height-0.5);
			cairo_line_to(cairo, priv->x_right-0.5, height-0.5);
			cairo_stroke(cairo);
		}
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	ChaMarking *instance = CHA_MARKING(self);
	ChaMarkingPrivate *priv = cha_marking_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(e_append_to, "%s[%p left=%d, right=%d]", iname, self, priv->x_left, priv->x_right);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
