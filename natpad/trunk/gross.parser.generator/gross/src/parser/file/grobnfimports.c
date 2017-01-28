/*
   File:    grobnfimports.c
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

#include "grobnfimports.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfImports"
#include <logging/catlog.h>

struct _GroBnfImportsPrivate {
	GroBnfString *code_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfImports, gro_bnf_imports, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfImports)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_imports_class_init(GroBnfImportsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_imports_init(GroBnfImports *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfImports *instance = GRO_BNF_IMPORTS(object);
	GroBnfImportsPrivate *priv = gro_bnf_imports_get_instance_private(instance);
	cat_unref_ptr(priv->code_text);
	G_OBJECT_CLASS(gro_bnf_imports_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_imports_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroBnfImports *gro_bnf_imports_new(GroBnfString *code_text) {
	GroBnfImports *result = g_object_new(GRO_TYPE_BNF_IMPORTS, NULL);
	cat_ref_anounce(result);
	GroBnfImportsPrivate *priv = gro_bnf_imports_get_instance_private(result);
	priv->code_text = cat_ref_ptr(code_text);
	return result;
}

GroBnfString *gro_bnf_imports_get_code_text(GroBnfImports *imports) {
	GroBnfImportsPrivate *priv = gro_bnf_imports_get_instance_private(imports);
	return priv->code_text;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfImports *instance = GRO_BNF_IMPORTS(self);
	GroBnfImportsPrivate *priv = gro_bnf_imports_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
