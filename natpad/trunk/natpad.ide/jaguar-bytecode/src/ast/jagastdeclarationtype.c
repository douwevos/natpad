/*
   File:    jagastdeclarationtype.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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



#include "jagastdeclarationtype.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstDeclarationType"
#include <logging/catlog.h>

struct _JagAstDeclarationTypePrivate {
	JagBytType type;
	JagBytName *referenceTypeName;
	int dimCount;
};

G_DEFINE_TYPE (JagAstDeclarationType, jag_ast_declaration_type, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_declaration_type_class_init(JagAstDeclarationTypeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstDeclarationTypePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_declaration_type_init(JagAstDeclarationType *instance) {
	JagAstDeclarationTypePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_DECLARATION_TYPE, JagAstDeclarationTypePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstDeclarationType *instance = JAG_AST_DECLARATION_TYPE(object);
	JagAstDeclarationTypePrivate *priv = instance->priv;
	cat_unref_ptr(priv->referenceTypeName);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstDeclarationType *jag_ast_declaration_type_new(JagBytType primitive_type, int dimCount) {
	JagAstDeclarationType *result = g_object_new(JAG_AST_TYPE_DECLARATION_TYPE, NULL);
	cat_ref_anounce(result);
	JagAstDeclarationTypePrivate *priv = result->priv;
	priv->type = primitive_type;
	priv->dimCount = dimCount;
	priv->referenceTypeName = NULL;
	return result;
}

JagAstDeclarationType *jag_ast_declaration_type_new_reference(JagBytName *type_name, int dimCount) {
	JagAstDeclarationType *result = g_object_new(JAG_AST_TYPE_DECLARATION_TYPE, NULL);
	cat_ref_anounce(result);
	JagAstDeclarationTypePrivate *priv = result->priv;
	priv->type = JAG_BYT_TYPE_REFERENCE;
	priv->dimCount = dimCount;
	priv->referenceTypeName = cat_ref_ptr(type_name);
	return result;
}


JagAstDeclarationType *jag_ast_declaration_type_set_dim_count(JagAstDeclarationType *declaration_type, int newDimCount) {
	JagAstDeclarationTypePrivate *spriv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	if (spriv->dimCount==newDimCount) {
		return cat_ref_ptr(declaration_type);
	}
	JagAstDeclarationType *result = g_object_new(JAG_AST_TYPE_DECLARATION_TYPE, NULL);
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(result);
	cat_ref_anounce(result);
	priv->type = spriv->type;
	priv->dimCount = newDimCount;
	priv->referenceTypeName = cat_ref_ptr(spriv->referenceTypeName);
	return result;
}

int jag_ast_declaration_type_get_dim_count(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return priv->dimCount;
}

JagBytName *jag_ast_declaration_type_get_reference_type_name(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return priv->referenceTypeName;
}

JagAstDeclarationType *jag_ast_declaration_type_set_reference_type_name(JagAstDeclarationType *declaration_type, JagBytName *new_name) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	if (jag_byt_name_equal(new_name, priv->referenceTypeName)) {
		return cat_ref_ptr(declaration_type);
	}
	return jag_ast_declaration_type_new_reference(new_name, priv->dimCount);
}

JagBytType jag_ast_declaration_type_get_primitive_type(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return priv->type;
}


gboolean jag_ast_declaration_type_is_reference(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return priv->type==JAG_BYT_TYPE_REFERENCE;
}

gboolean jag_ast_declaration_type_is_void(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return priv->type==JAG_BYT_TYPE_VOID;
}

gboolean jag_ast_declaration_type_is_primitive(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return priv->type!=JAG_BYT_TYPE_VOID && priv->type!=JAG_BYT_TYPE_REFERENCE;
}


gboolean jag_ast_declaration_type_is_category2(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return priv->type == JAG_BYT_TYPE_LONG || priv->type == JAG_BYT_TYPE_DOUBLE;
}


int jag_ast_declaration_type_hash_code(JagAstDeclarationType *declaration_type) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	return 13*priv->type
				+((priv->referenceTypeName==NULL) ? 0 : jag_byt_name_hash_code(priv->referenceTypeName))
				+priv->dimCount*101;
}

gboolean jag_ast_declaration_type_equal(JagAstDeclarationType *declaration_type, JagAstDeclarationType *other) {
	if (other==declaration_type) {
		return TRUE;
	}
	if (other==NULL || declaration_type==NULL) {
		return TRUE;
	}
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	JagAstDeclarationTypePrivate *opriv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(other);
	return opriv->type==priv->type && opriv->dimCount==priv->dimCount
					&& jag_byt_name_equal(opriv->referenceTypeName, priv->referenceTypeName);
}



static CatS l_s_txt_void = CAT_S_DEF("void");
static CatS l_s_txt_boolean = CAT_S_DEF("boolean");
static CatS l_s_txt_byte = CAT_S_DEF("byte");
static CatS l_s_txt_char = CAT_S_DEF("char");
static CatS l_s_txt_double = CAT_S_DEF("double");
static CatS l_s_txt_float = CAT_S_DEF("float");
static CatS l_s_txt_int = CAT_S_DEF("int");
static CatS l_s_txt_long = CAT_S_DEF("long");
static CatS l_s_txt_short= CAT_S_DEF("short");
static CatS l_s_txt_dims = CAT_S_DEF("[]");

void jag_ast_declaration_type_write(JagAstDeclarationType *declaration_type, JagAstWriter *out) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);

	switch(priv->type) {
		case JAG_BYT_TYPE_VOID : { jag_ast_writer_print(out, CAT_S(l_s_txt_void)); } break;
		case JAG_BYT_TYPE_BOOLEAN : { jag_ast_writer_print(out, CAT_S(l_s_txt_boolean)); } break;
		case JAG_BYT_TYPE_BYTE : { jag_ast_writer_print(out, CAT_S(l_s_txt_byte)); } break;
		case JAG_BYT_TYPE_CHAR : { jag_ast_writer_print(out, CAT_S(l_s_txt_char)); } break;
		case JAG_BYT_TYPE_DOUBLE : { jag_ast_writer_print(out, CAT_S(l_s_txt_double)); } break;
		case JAG_BYT_TYPE_FLOAT : { jag_ast_writer_print(out, CAT_S(l_s_txt_float)); } break;
		case JAG_BYT_TYPE_INT : { jag_ast_writer_print(out, CAT_S(l_s_txt_int)); } break;
		case JAG_BYT_TYPE_LONG : { jag_ast_writer_print(out, CAT_S(l_s_txt_long)); } break;
		case JAG_BYT_TYPE_SHORT : { jag_ast_writer_print(out, CAT_S(l_s_txt_short)); } break;
		case JAG_BYT_TYPE_REFERENCE : {
			jag_ast_writer_print(out, jag_byt_name_create_fqn(priv->referenceTypeName));
		} break;
	}

	int idx;
	for(idx=0; idx<priv->dimCount; idx++) {
		jag_ast_writer_print(out, CAT_S(l_s_txt_dims));
	}
}

CatStringWo *jag_ast_declaration_type_as_text(JagAstDeclarationType *declaration_type, CatStringWo *e_dump_buffer) {
	JagAstDeclarationTypePrivate *priv = JAG_AST_DECLARATION_TYPE_GET_PRIVATE(declaration_type);
	if (e_dump_buffer==NULL) {
		e_dump_buffer = cat_string_wo_new();
	}
	cat_string_wo_append_chars(e_dump_buffer, "JagAstDeclarationType[");
	gboolean print_dims = TRUE;
	switch(priv->type) {
		case JAG_BYT_TYPE_REFERENCE : {
			CatStringWo *a_fqn = jag_byt_name_create_fqn(priv->referenceTypeName);
			cat_string_wo_append(e_dump_buffer, a_fqn);
			cat_unref_ptr(a_fqn);
		} break;
		case  JAG_BYT_TYPE_VOID : {
			print_dims = FALSE;
			cat_string_wo_append_chars(e_dump_buffer, "void");
		} break;
		case JAG_BYT_TYPE_BOOLEAN : cat_string_wo_append_chars(e_dump_buffer, "boolean"); break;
		case JAG_BYT_TYPE_BYTE : cat_string_wo_append_chars(e_dump_buffer, "byte"); break;
		case JAG_BYT_TYPE_CHAR : cat_string_wo_append_chars(e_dump_buffer, "char"); break;
		case JAG_BYT_TYPE_SHORT : cat_string_wo_append_chars(e_dump_buffer, "short"); break;
		case JAG_BYT_TYPE_INT : cat_string_wo_append_chars(e_dump_buffer, "int"); break;
		case JAG_BYT_TYPE_LONG : cat_string_wo_append_chars(e_dump_buffer, "long"); break;
		case JAG_BYT_TYPE_FLOAT : cat_string_wo_append_chars(e_dump_buffer, "float"); break;
		case JAG_BYT_TYPE_DOUBLE : cat_string_wo_append_chars(e_dump_buffer, "double"); break;
	}
	if (print_dims) {
		if (priv->dimCount>0) {
			cat_string_wo_append_char(e_dump_buffer, '|');
			cat_string_wo_append_decimal(e_dump_buffer, priv->dimCount);
		}
	}
	cat_string_wo_append_char(e_dump_buffer, ']');
	return e_dump_buffer;
}


//	@Override
//	public String toString() {
//		StringBuilder buf = new StringBuilder("DeclarationType[");
//		if (type==JAG_BYT_TYPE_REFERENCE) {
//			buf.append(referenceTypeName.createFQN());
//			if (dimCount>0) {
//				buf.append("|"+dimCount);
//			}
//		} else if (type==Type.VOID) {
//			buf.append("void");
//		} else {
//			buf.append(type);
//			if (dimCount>0) {
//				buf.append("|"+dimCount);
//			}
//		}
//		buf.append("]");
//		return buf.toString();
//	}





