/*
   File:    leaoutputactionclear.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 15, 2014
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

#include "leaoutputactionclear.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaOutputActionClear"
#include <logging/catlog.h>

struct _LeaOutputActionClearPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaOutputActionClear, lea_output_action_clear, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(LeaOutputActionClear)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_output_action_clear_class_init(LeaOutputActionClearClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_output_action_clear_init(LeaOutputActionClear *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	LeaOutputActionClear *instance = LEA_OUTPUT_ACTION_CLEAR(object);
//	LeaOutputActionClearPrivate *priv = lea_output_action_clear_get_instance_private(instance);
	G_OBJECT_CLASS(lea_output_action_clear_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_output_action_clear_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaOutputActionClear *lea_output_action_clear_new() {
	LeaOutputActionClear *result = g_object_new(LEA_TYPE_OUTPUT_ACTION_CLEAR, NULL);
	cat_ref_anounce(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("do clear") , cat_string_wo_new_with("Do a clear"), NULL);
	const CatStringWo *key_seq = cat_string_wo_new_data("Ctrl+c");
	LeaKeySequence *seq = lea_key_sequence_from_string(key_seq);
	lea_action_set_default_key_sequence((LeaAction *) result, seq);
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


