/*
   File:    leaactionscientific.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 7, 2013
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


#include "leaactionscientific.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaActionScientific"
#include <logging/catlog.h>


G_DEFINE_TYPE(LeaActionScientific, lea_action_scientific, LEA_TYPE_ACTION) // @suppress("Unused static function")

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_action_scientific_class_init(LeaActionScientificClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_action_scientific_init(LeaActionScientific *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(lea_action_scientific_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_action_scientific_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaActionScientific *lea_action_scientific_new() {
	LeaActionScientific *result = g_object_new(LEA_TYPE_ACTION_SCIENTIFIC, NULL);
	cat_ref_anounce(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_data("digit.scientific"), cat_string_wo_new_with("S_cientific"), NULL);
	return result;
}
