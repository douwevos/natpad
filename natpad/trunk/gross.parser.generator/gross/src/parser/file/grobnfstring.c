
/*
   File:    grobnfstring.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 20, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "grobnfstring.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfString"
#include <logging/catlog.h>

struct _GroBnfStringPrivate {
	CatStringWo *text;
	GroLocation *start_location;
	GroLocation *end_location;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfString, gro_bnf_string, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfString)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_string_class_init(GroBnfStringClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_string_init(GroBnfString *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroBnfString *instance = GRO_BNF_STRING(object);
//	GroBnfStringPrivate *priv = gro_bnf_string_get_instance_private(instance);
	G_OBJECT_CLASS(gro_bnf_string_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_string_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroBnfString *gro_bnf_string_new(const CatStringWo *text, GroLocation *start_location, GroLocation *end_location) {
	GroBnfString *result = g_object_new(GRO_TYPE_BNF_STRING, NULL);
	cat_ref_anounce(result);
	GroBnfStringPrivate *priv = gro_bnf_string_get_instance_private(result);
	priv->text = cat_string_wo_clone(text, CAT_CLONE_DEPTH_AS_ANCHORED);
	priv->start_location = cat_ref_ptr(start_location);
	priv->end_location = cat_ref_ptr(end_location);
	return result;
}

CatStringWo *gro_bnf_string_get_text(GroBnfString *bnf_string) {
	GroBnfStringPrivate *priv = gro_bnf_string_get_instance_private(bnf_string);
	return priv->text;
}

GroLocation *gro_bnf_string_get_start(GroBnfString *bnf_string) {
	GroBnfStringPrivate *priv = gro_bnf_string_get_instance_private(bnf_string);
	return priv->start_location;
}

GroLocation *gro_bnf_string_get_end(GroBnfString *bnf_string) {
	GroBnfStringPrivate *priv = gro_bnf_string_get_instance_private(bnf_string);
	return priv->end_location;
}

int gro_bnf_string_hash(const GroBnfString *bnf_string) {
	GroBnfStringPrivate *priv = gro_bnf_string_get_instance_private((GroBnfString *) bnf_string);
	return cat_string_wo_hash(priv->text);
}

gboolean gro_bnf_string_equal(const GroBnfString *bnf_string_a, const GroBnfString *bnf_string_b) {
	if (bnf_string_a==bnf_string_b) {
		return TRUE;
	}
	if (bnf_string_a==NULL || bnf_string_b==NULL) {
		return FALSE;
	}
	GroBnfStringPrivate *priv_a = gro_bnf_string_get_instance_private((GroBnfString *) bnf_string_a);
	GroBnfStringPrivate *priv_b = gro_bnf_string_get_instance_private((GroBnfString *) bnf_string_b);
	return cat_string_wo_equal(priv_a->text, priv_b->text);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfString *instance = GRO_BNF_STRING(self);
	GroBnfStringPrivate *priv = gro_bnf_string_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%O]", iname, priv->text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
