/*
   File:    grobnfterminaldeclarationlist.c
   Project: gross
   Author:  Douwe Vos
   Date:    Nov 16, 2016
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

#include "../file/grobnfterminaldeclarationlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfTerminalDeclarationList"
#include <logging/catlog.h>

struct _GroBnfTerminalDeclarationListPrivate {
	GroBnfTerminalDeclarationList *linked;
	GroBnfTerminalDeclaration *term_decl;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfTerminalDeclarationList, gro_bnf_terminal_declaration_list, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfTerminalDeclarationList)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_terminal_declaration_list_class_init(GroBnfTerminalDeclarationListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_terminal_declaration_list_init(GroBnfTerminalDeclarationList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfTerminalDeclarationList *instance = GRO_BNF_TERMINAL_DECLARATION_LIST(object);
	GroBnfTerminalDeclarationListPrivate *priv = gro_bnf_terminal_declaration_list_get_instance_private(instance);
	cat_unref_ptr(priv->linked);
	cat_unref_ptr(priv->term_decl);
	G_OBJECT_CLASS(gro_bnf_terminal_declaration_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_terminal_declaration_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroBnfTerminalDeclarationList *gro_bnf_terminal_declaration_list_new(GroBnfTerminalDeclarationList *linked, GroBnfTerminalDeclaration *term_decl) {
	GroBnfTerminalDeclarationList *result = g_object_new(GRO_TYPE_BNF_TERMINAL_DECLARATION_LIST, NULL);
	cat_ref_anounce(result);
	GroBnfTerminalDeclarationListPrivate *priv = gro_bnf_terminal_declaration_list_get_instance_private(result);
	priv->linked = cat_ref_ptr(linked);
	priv->term_decl = cat_ref_ptr(term_decl);
	return result;
}

CatArrayWo *gro_bnf_terminal_declaration_list_enlist(GroBnfTerminalDeclarationList *list) {
	CatArrayWo *result = cat_array_wo_new();
	while(list!=NULL) {
		GroBnfTerminalDeclarationListPrivate *priv = gro_bnf_terminal_declaration_list_get_instance_private(list);
		cat_array_wo_append(result, (GObject *) priv->term_decl);
		list = priv->linked;
	}
	cat_array_wo_reverse(result);
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfTerminalDeclarationList *instance = GRO_BNF_TERMINAL_DECLARATION_LIST(self);
	GroBnfTerminalDeclarationListPrivate *priv = gro_bnf_terminal_declaration_list_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

