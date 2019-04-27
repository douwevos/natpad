/*
   File:    jagclasseditorfactory.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Mar 19, 2014
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

#include "jagclasseditorfactory.h"
#include "jageditorpanel.h"
#include <jaguarbytecode.h>
#include <elk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagClassEditorFactory"
#include <logging/catlog.h>

struct _JagClassEditorFactoryPrivate {
	LeaIPanelOwner *panel_owner;
	JagEditorConnector *connector;
	ElkDocumentIO *document_io;
	VipIFile *document_file;
//	JagIndexer *indexer;
};

static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagClassEditorFactory, jag_class_editor_factory, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagClassEditorFactory)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_EDITOR_FACTORY, l_resource_factory_iface_init)
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_class_editor_factory_class_init(JagClassEditorFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_class_editor_factory_init(JagClassEditorFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagClassEditorFactory *instance = JAG_CLASS_EDITOR_FACTORY(object);
	JagClassEditorFactoryPrivate *priv = jag_class_editor_factory_get_instance_private(instance);
	cat_unref_ptr(priv->panel_owner);
	cat_unref_ptr(priv->connector);
	cat_unref_ptr(priv->document_io);
	cat_unref_ptr(priv->document_file);
//	cat_unref_ptr(priv->indexer);
	G_OBJECT_CLASS(jag_class_editor_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_class_editor_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagClassEditorFactory *jag_class_editor_factory_new(LeaIPanelOwner *panel_owner, JagEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file/*, JagIndexer *indexer*/) {
	JagClassEditorFactory *result = g_object_new(JAG_TYPE_CLASS_EDITOR_FACTORY, NULL);
	cat_ref_anounce(result);
	JagClassEditorFactoryPrivate *priv = jag_class_editor_factory_get_instance_private(result);
	priv->panel_owner = cat_ref_ptr(panel_owner);
	priv->connector = cat_ref_ptr(connector);
	priv->document_io = cat_ref_ptr(document_io);
	priv->document_file = cat_ref_ptr(document_file);
//	priv->indexer = cat_ref_ptr(indexer);
	return result;
}

/********************* start ElkIResourceEditorFactory implementation *********************/

static ChaDocument *l_create_document_for_class(ChaDocumentManager *document_manager, CatIInputStream *input_stream) {

	ChaDocument *document = NULL;

	JagBytecodeReader *bytecode_reader = jag_bytecode_reader_new(input_stream);
	JagBytClassfile *classfile = jag_bytecode_reader_parse_bytecode(bytecode_reader);

	if (classfile) {

		JagByteCodePrinter *printer = jag_byte_code_printer_new();

		char *text = jag_byte_code_printer_print(printer, classfile);

		CatStringWo *a_source = cat_string_wo_new_with(text);

//		JagAstWriter *ast_writer = jag_ast_writer_new();
//		jag_ast_tp_write(ast_type, ast_writer);
//		CatStringWo *a_source = jag_ast_writer_to_string(ast_writer);
//
//		cat_unref_ptr(decompiler);
//		cat_unref_ptr(ast_type);
//		cat_unref_ptr(ast_writer);


//		CatStringWo *e_source = jag_raw_classfile_as_source(classfile);
		document = cha_document_new(document_manager);
		ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
		CatArrayWo *e_lines_2_insert = cha_document_create_line_list(document, a_source);
		cat_log_debug("a_source=%o", a_source);

		ChaCursorMWo *e_cursor_null = cha_cursorm_wo_new(0,0);
		cha_revision_wo_insert_lines(e_revision, e_cursor_null, e_lines_2_insert, TRUE);


		cha_document_anchor_document_full(document, CHA_DOCUMENT_ANCHOR_MODE_ADD_HISTORY);
		ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);
		cha_document_set_saved_revision(document, a_rev);
		cha_document_set_read_only(document, TRUE);

		cat_unref_ptr(a_rev);
		cat_unref_ptr(a_source);
	}
//		printf("%s", cat_string_wo_getchars(e_source));

	cat_unref_ptr(classfile);
	cat_unref_ptr(bytecode_reader);

	return document;
}


static GtkWidget *l_create_editor(ElkIResourceEditorFactory *self, MooNodeWo *moo_node) {
	JagClassEditorFactory *instance = JAG_CLASS_EDITOR_FACTORY(self);
	JagClassEditorFactoryPrivate *priv = jag_class_editor_factory_get_instance_private(instance);
	cat_log_debug("node=%o", moo_node);

	ElkDocumentBin *document_bin = elk_document_io_open_document_for_file(priv->document_io, priv->document_file);
	ChaDocument *document = elk_document_bin_get_document(document_bin);
	if (document==NULL) {
		CatIInputStream *input_stream = vip_ifile_open_input_stream(priv->document_file);
		ChaDocumentManager *document_manager = elk_document_bin_get_document_manager(document_bin);
		document = l_create_document_for_class(document_manager, input_stream);
		cat_unref_ptr(input_stream);
		elk_document_bin_set_document(document_bin, document);
		cat_unref(document);
	}

	JagEditorPanel *result = jag_editor_panel_new(priv->panel_owner, document_bin, priv->connector, NULL);
	elk_editor_panel_set_moose_node((ElkEditorPanel *) result, moo_node);
	return (GtkWidget *) result;
}


static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface) {
	iface->createEditor = l_create_editor;
}

/********************* end ElkIResourceEditorFactory implementation *********************/
