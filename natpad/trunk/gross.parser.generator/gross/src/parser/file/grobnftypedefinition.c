/*
   File:    grobnftypedefinition.c
   Project: gross
   Author:  Douwe Vos
   Date:    Nov 17, 2016
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

#include "grobnftypedefinition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfTypeDefinition"
#include <logging/catlog.h>

struct _GroBnfTypeDefinitionPrivate {
	GroBnfString *sym_type;
	GroBnfIdList *id_list;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfTypeDefinition, gro_bnf_type_definition, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfTypeDefinition)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_type_definition_class_init(GroBnfTypeDefinitionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_type_definition_init(GroBnfTypeDefinition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfTypeDefinition *instance = GRO_BNF_TYPE_DEFINITION(object);
	GroBnfTypeDefinitionPrivate *priv = gro_bnf_type_definition_get_instance_private(instance);
	cat_unref_ptr(priv->id_list);
	cat_unref_ptr(priv->sym_type);
	G_OBJECT_CLASS(gro_bnf_type_definition_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_type_definition_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroBnfTypeDefinition *gro_bnf_type_definition_new(GroBnfString *sym_type, GroBnfIdList *id_list) {
	GroBnfTypeDefinition *result = g_object_new(GRO_TYPE_BNF_TYPE_DEFINITION, NULL);
	cat_ref_anounce(result);
	GroBnfTypeDefinitionPrivate *priv = gro_bnf_type_definition_get_instance_private(result);
	priv->sym_type = cat_ref_ptr(sym_type);
	priv->id_list = cat_ref_ptr(id_list);
	return result;
}

GroBnfString *gro_bnf_type_definition_get_symbol_type(GroBnfTypeDefinition *type_def) {
	GroBnfTypeDefinitionPrivate *priv = gro_bnf_type_definition_get_instance_private(type_def);
	return priv->sym_type;
}

GroBnfIdList *gro_bnf_type_definition_get_id_list(GroBnfTypeDefinition *type_def) {
	GroBnfTypeDefinitionPrivate *priv = gro_bnf_type_definition_get_instance_private(type_def);
	return priv->id_list;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfTypeDefinition *instance = GRO_BNF_TYPE_DEFINITION(self);
	GroBnfTypeDefinitionPrivate *priv = gro_bnf_type_definition_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
