/*
   File:    jagdecimportrepository.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "jagdecimportrepository.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecImportRepository"
#include <logging/catlog.h>

struct _JagDecImportRepositoryPrivate {
	CatHashMapWo *e_map;	// Map<String, ImportNameEntry> map = new HashMap<String, ImportNameEntry>();
	JagBytName *main_type_name;
};

G_DEFINE_TYPE (JagDecImportRepository, jag_dec_import_repository, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_import_repository_class_init(JagDecImportRepositoryClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecImportRepositoryPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_import_repository_init(JagDecImportRepository *instance) {
	JagDecImportRepositoryPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_IMPORT_REPOSITORY, JagDecImportRepositoryPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecImportRepository *instance = JAG_DEC_IMPORT_REPOSITORY(object);
	JagDecImportRepositoryPrivate *priv = instance->priv;
	cat_unref_ptr(priv->main_type_name);
	cat_unref_ptr(priv->e_map);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecImportRepository *jag_dec_import_repository_new(JagBytName *main_type_name) {
	JagDecImportRepository *result = g_object_new(JAG_DEC_TYPE_IMPORT_REPOSITORY, NULL);
	cat_ref_anounce(result);
	JagDecImportRepositoryPrivate *priv = result->priv;
	priv->main_type_name = cat_ref_ptr(main_type_name);
	priv->e_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	return result;
}


JagBytName *jag_dec_import_repository_get_main_type_name(JagDecImportRepository *import_repository) {
	return JAG_DEC_IMPORT_REPOSITORY_GET_PRIVATE(import_repository)->main_type_name;
}

JagBytName *jag_dec_import_repository_create_resolved_typename(JagDecImportRepository *import_repository, JagBytName *referenceTypeName) {
	JagDecImportRepositoryPrivate *priv = JAG_DEC_IMPORT_REPOSITORY_GET_PRIVATE(import_repository);
	JagBytName *result = NULL;
	CatStringWo *a_shortTypeName = jag_byt_name_get_short_type_name(referenceTypeName);
	JagDecImportNameEntry *importNameEntry = (JagDecImportNameEntry *) cat_hash_map_wo_get(priv->e_map, a_shortTypeName);
	if (importNameEntry!=NULL) {
		if (jag_dec_import_name_entry_allow_short_version(importNameEntry)) {
			result = jag_byt_name_new(a_shortTypeName, FALSE);
		}
	}
	if (result == NULL) {
		result = cat_ref_ptr(referenceTypeName);
	}
	return result;
}


JagAstDeclarationType *jag_dec_import_repository_create_final_declaration_type(JagDecImportRepository *import_repository, JagAstDeclarationType *full_type) {
	JagAstDeclarationType *result = NULL;
	if (jag_ast_declaration_type_is_reference(full_type)) {
		JagBytName *full_name = jag_ast_declaration_type_get_reference_type_name(full_type);
		JagBytName *possible_short_name = jag_dec_import_repository_create_resolved_typename(import_repository, full_name);
		if (!jag_byt_name_equal(full_name, possible_short_name)) {
			result = jag_ast_declaration_type_set_reference_type_name(full_type, possible_short_name);
		}
		cat_unref_ptr(possible_short_name);
	}
	if (result == NULL) {
		result = cat_ref_ptr(full_type);
	}
	return result;

}


JagBytName *jag_dec_import_repository_add_name(JagDecImportRepository *import_repository, JagBytName *fullName) {
	JagDecImportRepositoryPrivate *priv = JAG_DEC_IMPORT_REPOSITORY_GET_PRIVATE(import_repository);
	CatStringWo *a_shortTypeName = jag_byt_name_get_short_type_name(fullName);
	JagDecImportNameEntry *importNameEntry = (JagDecImportNameEntry *) cat_hash_map_wo_get(priv->e_map, a_shortTypeName);
	cat_log_debug("shortTypeName=%s", cat_string_wo_getchars(a_shortTypeName));
	if (importNameEntry==NULL) {
		importNameEntry = jag_dec_import_name_entry_new(a_shortTypeName);
		cat_hash_map_wo_put(priv->e_map, (GObject *) a_shortTypeName, (GObject *) importNameEntry);
		cat_unref(importNameEntry);
	}
	return jag_dec_import_name_entry_get_or_add(importNameEntry, fullName);
}

static gint l_import_statment_comparator(const JagAstImportStatement *a, const JagAstImportStatement *b) {
	if (a==b) {
		return 0;
	}

	JagBytName *name_a = jag_ast_import_statement_get_name((JagAstImportStatement *) (a));
	JagBytName *name_b = jag_ast_import_statement_get_name((JagAstImportStatement *) (b));
	return jag_byt_name_compare(name_a, name_b);
}


CatArrayWo *jag_dec_import_repository_create_statements(JagDecImportRepository *import_repository) {
	JagDecImportRepositoryPrivate *priv = JAG_DEC_IMPORT_REPOSITORY_GET_PRIVATE(import_repository);
	JagBytName *javaLangPackage = jag_byt_name_new(cat_string_wo_new_with("java.lang"), FALSE);
	CatArrayWo *e_result = cat_array_wo_new();
	CatArrayWo *e_keys = cat_hash_map_wo_enlist_keys(priv->e_map, NULL);

	if (e_keys) {
		CatIIterator *iter = cat_array_wo_iterator(e_keys);
		CatStringWo *a_main_type_name = jag_byt_name_create_fqn(priv->main_type_name);
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *a_key = (CatStringWo *) cat_iiterator_next(iter);
			if (cat_string_wo_equal(a_main_type_name, a_key)) {
				continue;
			}
			JagDecImportNameEntry *importNameEntry = (JagDecImportNameEntry *) cat_hash_map_wo_get(priv->e_map, a_key);
			if (jag_dec_import_name_entry_allow_short_version(importNameEntry)) {
				JagBytName *importName = jag_dec_import_name_entry_get_first_name(importNameEntry);
				if (jag_byt_name_starts_with(importName, javaLangPackage)) {
					continue;
				}
				JagAstImportStatement *import_statement = jag_ast_import_statement_new(importName);
				cat_array_wo_append(e_result, (GObject *) import_statement);
				cat_unref(import_statement);
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(e_keys);
		cat_unref_ptr(a_main_type_name);
		cat_array_wo_sort(e_result, (GCompareFunc) l_import_statment_comparator);
	}
	cat_unref_ptr(javaLangPackage);

	return e_result;
}


void jag_dec_import_repository_attach_method(JagDecImportRepository *import_repository, JagBytMethod *method) {
	JagBytMethodHeader *method_header = jag_byt_method_get_method_header(method);
	jag_dec_import_repository_attach_method_header(import_repository, method_header);
}


void jag_dec_import_repository_attach_method_header(JagDecImportRepository *import_repository, JagBytMethodHeader *method_header) {
	JagAstDeclarationType *return_type = jag_byt_method_header_get_return_type(method_header);
	if (return_type!=NULL) {
		jag_dec_import_repository_attach_declaration_type(import_repository, return_type, TRUE);
	}

	JagAstArgumentList *argument_list = jag_byt_method_header_get_argument_list(method_header);
	if (argument_list && jag_ast_argument_list_count(argument_list)>0) {
		int arg_idx;
		for(arg_idx=jag_ast_argument_list_count(argument_list)-1; arg_idx>=0; arg_idx--) {
			JagAstArgument *argument = jag_ast_argument_list_get_at(argument_list, arg_idx);
			JagAstDeclarationType *argument_type = jag_ast_argument_get_declaration_type(argument);
			jag_dec_import_repository_attach_declaration_type(import_repository, argument_type, TRUE);
		}
	}
	JagBytExceptions *exceptions = jag_byt_method_header_get_exceptions(method_header);
	if (exceptions!=NULL) {
		jag_dec_import_repository_attach_exceptions(import_repository, exceptions);
	}

}

void jag_dec_import_repository_attach_exceptions(JagDecImportRepository *import_repository, JagBytExceptions *excpetions) {
	int count = jag_byt_exceptions_count(excpetions);
	int idx;
	for(idx=0; idx<count; idx++) {
		JagBytName *name = jag_byt_exceptions_get(excpetions, idx);
		jag_dec_import_repository_add_name(import_repository, name);
	}
}

JagAstDeclarationType *jag_dec_import_repository_attach_declaration_type(JagDecImportRepository *import_repository, JagAstDeclarationType *declaration_type, gboolean quit) {
	if (jag_ast_declaration_type_is_reference(declaration_type)) {
		JagBytName *importedName = jag_dec_import_repository_add_name(import_repository, jag_ast_declaration_type_get_reference_type_name(declaration_type));
		if (!quit) {
			declaration_type = jag_ast_declaration_type_new_reference(importedName, jag_ast_declaration_type_get_dim_count(declaration_type));
		}
	} else if (!quit) {
		cat_ref_ptr(declaration_type);
	}
	return declaration_type;
}






