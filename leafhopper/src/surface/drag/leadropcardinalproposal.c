/*
   File:    leadropcardinalproposal.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 23, 2014
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

#include "leadropcardinalproposal.h"
#include "leaidropproposal.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDropCardinalProposal"
#include <logging/catlog.h>

struct _LeaDropCardinalProposalPrivate {
	LeaIRegion *region;
	int level;
	LeaCardinalPlacement placement;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_drop_proposal_iface_init(LeaIDropProposalInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaDropCardinalProposal, lea_drop_cardinal_proposal, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaDropCardinalProposal)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IDROP_PROPOSAL, l_drop_proposal_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_drop_cardinal_proposal_class_init(LeaDropCardinalProposalClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_drop_cardinal_proposal_init(LeaDropCardinalProposal *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaDropCardinalProposal *instance = LEA_DROP_CARDINAL_PROPOSAL(object);
	LeaDropCardinalProposalPrivate *priv = lea_drop_cardinal_proposal_get_instance_private(instance);
	cat_unref_ptr(priv->region);
	G_OBJECT_CLASS(lea_drop_cardinal_proposal_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_drop_cardinal_proposal_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaDropCardinalProposal *lea_drop_cardinal_proposal_new(LeaIRegion *region, int level, LeaCardinalPlacement placement) {
	LeaDropCardinalProposal *result = g_object_new(LEA_TYPE_DROP_CARDINAL_PROPOSAL, NULL);
	cat_ref_anounce(result);
	LeaDropCardinalProposalPrivate *priv = lea_drop_cardinal_proposal_get_instance_private(result);
	priv->region = cat_ref_ptr(region);
	priv->level = level;
	priv->placement = placement;
	return result;
}

LeaCardinalPlacement lea_drop_cardinal_proposal_get_placement(LeaDropCardinalProposal *proposal) {
	LeaDropCardinalProposalPrivate *priv = lea_drop_cardinal_proposal_get_instance_private(proposal);
	return priv->placement;
}

LeaIRegion *lea_drop_cardinal_proposal_get_region(LeaDropCardinalProposal *proposal) {
	LeaDropCardinalProposalPrivate *priv = lea_drop_cardinal_proposal_get_instance_private(proposal);
	return priv->region;
}



/********************* start LeaIDropProposal implementation *********************/

static void l_get_rectangle(LeaDropCardinalProposal *proposal, GdkRectangle *rect) {
	LeaDropCardinalProposalPrivate *priv = lea_drop_cardinal_proposal_get_instance_private(proposal);
	const GtkAllocation node_alloc = lea_iregion_get_allocation(priv->region);
	int sx = node_alloc.x;
	int sy = node_alloc.y;
	int swidth = node_alloc.width;
	int sheight = node_alloc.height;
	switch(priv->placement) {
		case LEA_CP_SOUTH : {
			sheight = sheight/2;
			sy += sheight;
		} break;
		case LEA_CP_NORTH : {
			sheight = sheight/2;
		} break;
		case LEA_CP_EAST : {
			swidth = swidth/2;
			sx += swidth;
		} break;
		case LEA_CP_WEST : {
			swidth = swidth/2;
		} break;
		case LEA_CP_NONE : break;
		case LEA_CP_COVER : break;
	}
	rect->x = sx;
	rect->y = sy;
	rect->width = swidth;
	rect->height = sheight;

	cat_log_debug("rect[%d,%d,%d,%d]", sx,sy, swidth, sheight);
}


static void l_drop_proposal_paint(LeaIDropProposal *self, cairo_t *cairo) {
	LeaDropCardinalProposal *proposal = LEA_DROP_CARDINAL_PROPOSAL(self);
	GdkRectangle rect;
	l_get_rectangle(proposal, &rect);

	cat_log_debug("drawing");
	double dashes[] = {3.0,  /* ink */
	                   3.0,  /* skip */ };
	cairo_set_dash(cairo, dashes, 2, 0);

	cairo_set_line_width(cairo, 1);
	cairo_set_source_rgb(cairo, 1,0, 0);
	cairo_rectangle(cairo,rect.x+0.5,rect.y+0.5,rect.width-1,rect.height-1);
	cairo_rectangle(cairo,rect.x+2.5,rect.y+2.5,rect.width-5,rect.height-5);
	cairo_stroke(cairo);
}



static void l_drop_proposal_invalidate(LeaIDropProposal *self, GdkWindow *window) {
	LeaDropCardinalProposal *proposal = LEA_DROP_CARDINAL_PROPOSAL(self);
	GdkRectangle rect;
	l_get_rectangle(proposal, &rect);
	gdk_window_invalidate_rect(window, &rect, TRUE);
}

static gboolean l_drop_proposal_equal(LeaIDropProposal *self, LeaIDropProposal *other) {
	if (self==other) {
		return TRUE;
	}
	if ((self==NULL) || (other==NULL) || !LEA_IS_DROP_CARDINAL_PROPOSAL(other)) {
		return FALSE;
	}
	LeaDropCardinalProposalPrivate *priv_a = lea_drop_cardinal_proposal_get_instance_private(LEA_DROP_CARDINAL_PROPOSAL(self));
	LeaDropCardinalProposalPrivate *priv_b = lea_drop_cardinal_proposal_get_instance_private(LEA_DROP_CARDINAL_PROPOSAL(other));
	return priv_a->level==priv_b->level
		&& priv_a->placement==priv_b->placement
		&& priv_a->region == priv_b->region;
}


static void l_drop_proposal_iface_init(LeaIDropProposalInterface *iface) {
	iface->paint = l_drop_proposal_paint;
	iface->invalidate = l_drop_proposal_invalidate;
	iface->equal = l_drop_proposal_equal;
}

/********************* end LeaIDropProposal implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaDropCardinalProposal *instance = LEA_DROP_CARDINAL_PROPOSAL(self);
	LeaDropCardinalProposalPrivate *priv = lea_drop_cardinal_proposal_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p placement=%d, level=%d, region=%o]", iname, self, priv->placement, priv->level, priv->region);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

