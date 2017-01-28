/*
   File:    leadroptotabproposal.c
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

#include "leadroptotabproposal.h"
#include "leaidropproposal.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDropToTabProposal"
#include <logging/catlog.h>

struct _LeaDropToTabProposalPrivate {
	LeaSurface *surface;
	int panel_idx;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_drop_proposal_iface_init(LeaIDropProposalInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaDropToTabProposal, lea_drop_to_tab_proposal, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaDropToTabProposal)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IDROP_PROPOSAL, l_drop_proposal_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_drop_to_tab_proposal_class_init(LeaDropToTabProposalClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_drop_to_tab_proposal_init(LeaDropToTabProposal *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	LeaDropToTabProposal *instance = LEA_DROP_TO_TAB_PROPOSAL(object);
//	LeaDropToTabProposalPrivate *priv = lea_drop_to_tab_proposal_get_instance_private(instance);
	G_OBJECT_CLASS(lea_drop_to_tab_proposal_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_drop_to_tab_proposal_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaDropToTabProposal *lea_drop_to_tab_proposal_new(LeaSurface *surface, int panel_idx) {
	LeaDropToTabProposal *result = g_object_new(LEA_TYPE_DROP_TO_TAB_PROPOSAL, NULL);
	cat_ref_anounce(result);
	LeaDropToTabProposalPrivate *priv = lea_drop_to_tab_proposal_get_instance_private(result);
	priv->surface = cat_ref_ptr(surface);
	priv->panel_idx = panel_idx;
	return result;
}

LeaSurface *lea_drop_to_tab_proposal_get_surface(LeaDropToTabProposal *drop_proposal) {
	LeaDropToTabProposalPrivate *priv = lea_drop_to_tab_proposal_get_instance_private(drop_proposal);
	return priv->surface;
}

int lea_drop_to_tab_proposal_get_tab_index(LeaDropToTabProposal *drop_proposal) {
	LeaDropToTabProposalPrivate *priv = lea_drop_to_tab_proposal_get_instance_private(drop_proposal);
	return priv->panel_idx;
}


/********************* start LeaIDropProposal implementation *********************/



static gboolean l_get_rect(LeaDropToTabProposal *proposal, GdkRectangle *rect, gboolean shift) {
	LeaDropToTabProposalPrivate *priv = lea_drop_to_tab_proposal_get_instance_private(proposal);
	LeaSurfaceTabBar *tab_bar = lea_surface_get_tab_bar(priv->surface);

	gboolean result = lea_surface_tab_bar_get_tab_view_rect(tab_bar, priv->panel_idx, rect);

	GtkAllocation alloc;
	gtk_widget_get_allocation((GtkWidget *) tab_bar, &alloc);
	rect->x += alloc.x;
	rect->y += alloc.y;

	if (shift) {
		GtkWidget *container = gtk_widget_get_parent((GtkWidget *) tab_bar);
		int x = 0;
		int y = 0;
		GtkAllocation allocation;
		/* translate coordinates. Ugly business, that. */
		if (!gtk_widget_get_has_window((container))) {
			gtk_widget_get_allocation(GTK_WIDGET (container), &allocation);
			x = -allocation.x;
			y = -allocation.y;
		}

		if (!gtk_widget_get_has_window(GTK_WIDGET(tab_bar))) {
			gtk_widget_get_allocation(GTK_WIDGET(tab_bar), &allocation);
			x += allocation.x;
			y += allocation.y;
		}
		cat_log_debug("shifting:x/y=%d/%d", x, y);
		rect->x += x;
		rect->y += y;
	}

	cat_log_debug("rect[%d,%d,%d,%d]", rect->x, rect->y, rect->width, rect->height);

	return result;
}




static void l_drop_proposal_paint(LeaIDropProposal *self, cairo_t *cairo) {
	LeaDropToTabProposal *proposal = LEA_DROP_TO_TAB_PROPOSAL(self);
	GdkRectangle rect;
	if (l_get_rect(proposal, &rect, FALSE)) {

		double dashes[] = {3.0,  /* ink */
		                   3.0,  /* skip */ };
		cairo_set_dash(cairo, dashes, 2, 0);
		cairo_set_source_rgb(cairo, 1,0, 0);
		cairo_set_line_width(cairo, 1);
		cairo_rectangle(cairo, rect.x+0.5, rect.y+0.5, rect.width-1, rect.height-1);
		cairo_stroke(cairo);
		cairo_rectangle(cairo, rect.x+2.5, rect.y+2.5, rect.width-5, rect.height-5);
		cairo_stroke(cairo);
	}
}


static void l_drop_proposal_invalidate(LeaIDropProposal *self, GdkWindow *window) {
	LeaDropToTabProposal *proposal = LEA_DROP_TO_TAB_PROPOSAL(self);
	GdkRectangle rect;
	if (l_get_rect(proposal, &rect, FALSE)) {
		rect.x--;
		rect.y--;
		rect.width += 2;
		rect.height += 2;
		gdk_window_invalidate_rect(window, &rect, TRUE);
	}
}

static gboolean l_drop_proposal_equal(LeaIDropProposal *self, LeaIDropProposal *other) {
	if (self==other) {
		return TRUE;
	}
	if ((self==NULL) || (other==NULL) || !LEA_IS_DROP_TO_TAB_PROPOSAL(other)) {
		return FALSE;
	}
	LeaDropToTabProposalPrivate *priv_a = lea_drop_to_tab_proposal_get_instance_private(LEA_DROP_TO_TAB_PROPOSAL(self));
	LeaDropToTabProposalPrivate *priv_b = lea_drop_to_tab_proposal_get_instance_private(LEA_DROP_TO_TAB_PROPOSAL(other));
	return (priv_a->panel_idx == priv_b->panel_idx) && (priv_a->surface == priv_b->surface);
}

static void l_drop_proposal_iface_init(LeaIDropProposalInterface *iface) {
	iface->paint = l_drop_proposal_paint;
	iface->invalidate = l_drop_proposal_invalidate;
	iface->equal = l_drop_proposal_equal;
}

/********************* end LeaIDropProposal implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaDropToTabProposal *instance = LEA_DROP_TO_TAB_PROPOSAL(self);
	LeaDropToTabProposalPrivate *priv = lea_drop_to_tab_proposal_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, panel_idx=%d, surface=%o]", iname, self, priv->panel_idx, priv->surface);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

