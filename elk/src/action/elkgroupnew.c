/*
   File:    elkgroupnew.c
   Project: elk
   Author:  Douwe Vos
   Date:    9 Aug 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#include "elkgroupnew.h"
#include "elkactionnew.h"
#include "../elkservice.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkGroupNew"
#include <logging/catlog.h>

struct _ElkGroupNewPrivate {
	ElkIService *service;
};

G_DEFINE_TYPE_WITH_CODE(ElkGroupNew, elk_group_new, LEA_TYPE_ACTION_GROUP,  // @suppress("Unused static function")
	G_ADD_PRIVATE(ElkGroupNew)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_group_new_class_init(ElkGroupNewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_group_new_init(ElkGroupNew *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkGroupNew *instance = ELK_GROUP_NEW(object);
	ElkGroupNewPrivate *priv = elk_group_new_get_instance_private(instance);
	cat_unref_ptr(priv->service);
	G_OBJECT_CLASS(elk_group_new_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_group_new_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkGroupNew *elk_group_new_new(ElkIService *service) {
	ElkGroupNew *result = g_object_new(ELK_TYPE_GROUP_NEW, NULL);
	cat_ref_anounce(result);
	ElkGroupNewPrivate *priv = elk_group_new_get_instance_private(result);
	priv->service = cat_ref_ptr(service);
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("New Editor"), cat_string_wo_new_with("New Editor"));

	return result;
}

void elk_group_new_resource_handlers_updated(ElkGroupNew *group_new) {
	ElkGroupNewPrivate *priv = elk_group_new_get_instance_private(group_new);
	CatHashMapWo *map = elk_service_enlist_empty_editor_factories((ElkService *) priv->service);
	CatArrayWo *names = cat_hash_map_wo_enlist_keys(map, NULL);
	cat_array_wo_sort(names, cat_string_wo_compare);
	CatIIterator *iter = cat_array_wo_iterator(names);
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *name = (CatStringWo *) cat_iiterator_next(iter);
		cat_log_debug("editor name:%O", name);
		ElkIResourceEditorFactory *factory = (ElkIResourceEditorFactory *) cat_hash_map_wo_get(map, name);
		LeaAction *action = elk_action_new_for_new(priv->service, name, factory);
		lea_action_group_add((LeaActionGroup *) group_new, action);

	}
	cat_unref_ptr(iter);

}
