/*
   File:    jagpjcskip.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 3, 2017
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

#include "jagpjcskip.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCSkip"
#include <logging/catlog.h>

struct _JagPJCSkipPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPJCSkip, jagp_jcskip, JAGP_TYPE_JCSTATEMENT,
		G_ADD_PRIVATE(JagPJCSkip)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_class(JagPJCTree *tree) { return JAGP_TAG_SKIP; }

static void jagp_jcskip_class_init(JagPJCSkipClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_class;
}

static void jagp_jcskip_init(JagPJCSkip *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPJCSkip *instance = JAGP_JCSKIP(object);
//	JagPJCSkipPrivate *priv = jagp_jcskip_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_jcskip_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcskip_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCSkip *jagp_jcskip_new() {
	JagPJCSkip *result = g_object_new(JAGP_TYPE_JCSKIP, NULL);
	cat_ref_anounce(result);
	JagPJCSkipPrivate *priv = jagp_jcskip_get_instance_private(result);
//	JAGP_JCSTATEMENT_construct((JagPJCStatement *) result);
	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPJCSkip *instance = JAGP_JCSKIP(self);
	JagPJCSkipPrivate *priv = jagp_jcskip_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
