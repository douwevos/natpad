/*
   File:    jagpjcswitch.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 13, 2017
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

#include "jagpjcswitch.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCSwitch"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCSwitch, jagp_jcswitch, JAGP_TYPE_JCSTATEMENT);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_class(JagPJCTree *tree) { return JAGP_TAG_SWITCH; }

static void jagp_jcswitch_class_init(JagPJCSwitchClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_class;
}

static void jagp_jcswitch_init(JagPJCSwitch *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCSwitch *instance = JAGP_JCSWITCH(object);
	cat_unref_ptr(instance->selector);
	cat_unref_ptr(instance->cases);
	G_OBJECT_CLASS(jagp_jcswitch_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcswitch_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCSwitch *jagp_jcswitch_new(JagPJCExpression *selector, CatArrayWo /*<JagPJCCase>*/ *cases) {
	JagPJCSwitch *result = g_object_new(JAGP_TYPE_JCSWITCH, NULL);
	cat_ref_anounce(result);
	result->selector = cat_ref_ptr(selector);
	result->cases = cat_ref_ptr(cases);
	return result;
}

