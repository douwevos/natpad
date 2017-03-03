/*
   File:    dralinelayout.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 4, 2015
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

#include "dralinelayout.h"
#include "attr/draiattributebuilder.h"
#include "attr/draforeground.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraLineLayout"
#include <logging/catlog.h>

struct _DraLineLayoutPrivate {
	DraLineInfoWo *a_line_info;
	CatArrayWo *a_occurrences;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraLineLayout, dra_line_layout, CHA_TYPE_LINE_LAYOUT,
		G_ADD_PRIVATE(DraLineLayout)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_apply_extra_attributes(ChaLineLayout *line_layout, PangoAttrList *pango_attr_list, ChaPrefsColorMapWo *color_map);


static void dra_line_layout_class_init(DraLineLayoutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaLineLayoutClass *ll_class = CHA_LINE_LAYOUT_CLASS(clazz);
	ll_class->applyExtraAttributes = l_apply_extra_attributes;
}

static void dra_line_layout_init(DraLineLayout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraLineLayout *instance = DRA_LINE_LAYOUT(object);
	DraLineLayoutPrivate *priv = dra_line_layout_get_instance_private(instance);
	cat_unref_ptr(priv->a_line_info);
	cat_unref_ptr(priv->a_occurrences);
	G_OBJECT_CLASS(dra_line_layout_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_line_layout_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraLineLayout *dra_line_layout_new() {
	DraLineLayout *result = g_object_new(DRA_TYPE_LINE_LAYOUT, NULL);
	cat_ref_anounce(result);
	DraLineLayoutPrivate *priv = dra_line_layout_get_instance_private(result);
	cha_line_layout_construct((ChaLineLayout *) result);
	priv->a_line_info = NULL;
	return result;
}

void dra_line_layout_set_line_info(DraLineLayout *line_layout, DraLineInfoWo *a_line_info) {
	DraLineLayoutPrivate *priv = dra_line_layout_get_instance_private(line_layout);
	cat_log_debug("line_layout=%o, a_line_info=%o", line_layout, a_line_info);
	if (priv->a_line_info == a_line_info) {
		return;
	}
	cat_ref_swap(priv->a_line_info, a_line_info);
	cha_line_layout_mark_for_repaint((ChaLineLayout *) line_layout);
	cha_line_layout_mark_attributes_dirty((ChaLineLayout *) line_layout);
}

void dra_line_layout_set_occurrences(DraLineLayout *line_layout, CatArrayWo *a_occurrences) {
	DraLineLayoutPrivate *priv = dra_line_layout_get_instance_private(line_layout);
//	cat_log_debug("line_layout=%o, a_line_info=%o", line_layout, a_line_info);
	if (cat_array_wo_equal(a_occurrences, priv->a_occurrences, NULL)) {
		return;
	}
	cat_ref_swap(priv->a_occurrences, a_occurrences);
	cha_line_layout_mark_for_repaint((ChaLineLayout *) line_layout);
	cha_line_layout_mark_attributes_dirty((ChaLineLayout *) line_layout);

}


static void l_apply_extra_attributes(ChaLineLayout *line_layout, PangoAttrList *pango_attr_list, ChaPrefsColorMapWo *color_map) {
	DraLineLayoutPrivate *priv = dra_line_layout_get_instance_private((DraLineLayout *) line_layout);

	if (priv->a_line_info) {
		CatArrayWo *ab_list = dra_line_info_wo_get_ab_list(priv->a_line_info);
		cat_log_detail("ab_list=%p", ab_list);
		if (ab_list) {
			CatIIterator *iter = cat_array_wo_iterator(ab_list);
			while(cat_iiterator_has_next(iter)) {
				DraIAttributeBuilder *ab = (DraIAttributeBuilder *) cat_iiterator_next(iter);
				cat_log_trace("ab=%o", ab);
				DraIAttributeBuilderInterface *ab_iface = DRA_IATTRIBUTE_BUILDER_GET_INTERFACE(ab);
				PangoAttribute *attr = ab_iface->build(ab, color_map);
				if (attr->end_index<=attr->start_index) {
					if (attr->end_index<attr->start_index) {
						cat_log_fatal("start_index:%d, end_index:%d", attr->start_index, attr->end_index);
					}
					continue;
				}
				pango_attr_list_change(pango_attr_list, attr);
			}
			cat_unref_ptr(iter);
		}
		cat_log_detail("ab_list=%p done", ab_list);
	}

	if (priv->a_occurrences) {
		int fidx, lidx;
		cat_log_detail("priv->a_occurrences=%p", priv->a_occurrences);
		CatIIterator *iter = cat_array_wo_iterator(priv->a_occurrences);
		while(cat_iiterator_has_next(iter)) {
			ChaSearchOccurrence *occurrence = (ChaSearchOccurrence *) cat_iiterator_next(iter);
			cat_log_detail("occurrence=%O", occurrence);
			cha_search_occurence_get_column_start_and_end(occurrence, &fidx, &lidx);
			PangoAttribute *ptr = pango_attr_background_new(0xE8E8, 0xE8E8, 0xF8F8);
			ptr->start_index = fidx;
			ptr->end_index = lidx;
			pango_attr_list_change(pango_attr_list, ptr);
		}
		cat_unref_ptr(iter);
	}

}

CatArrayWo *dra_line_layout_get_line_tags(DraLineLayout *line_layout) {
	DraLineLayoutPrivate *priv = dra_line_layout_get_instance_private(line_layout);
	CatArrayWo *result = NULL;
	if (priv->a_line_info) {
		result = dra_line_info_wo_get_line_tags(priv->a_line_info);
	}
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	DraLineLayout *instance = DRA_LINE_LAYOUT(self);
//	DraLineLayoutPrivate *priv = dra_line_layout_get_instance_private(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
