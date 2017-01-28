/*
   File:    jagbytmnemonicblock.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#include "jagbytmnemonicblock.h"
#include "mnemonic/jagbytoparraylength.h"
#include "mnemonic/jagbytopcheckcast.h"
#include "mnemonic/jagbytopcompareint.h"
#include "mnemonic/jagbytopcomparereference.h"
#include "mnemonic/jagbytopduplicate.h"
#include "mnemonic/jagbytopget.h"
#include "mnemonic/jagbytopgoto.h"
#include "mnemonic/jagbytopincrement.h"
#include "mnemonic/jagbytopinvoke.h"
#include "mnemonic/jagbytoploadarray.h"
#include "mnemonic/jagbytoploadconstant.h"
#include "mnemonic/jagbytoploadfastindex.h"
#include "mnemonic/jagbytoploadindex.h"
#include "mnemonic/jagbytoploadnull.h"
#include "mnemonic/jagbytopmathbasic.h"
#include "mnemonic/jagbytopmathlogic.h"
#include "mnemonic/jagbytopmonitor.h"
#include "mnemonic/jagbytopmultianewarray.h"
#include "mnemonic/jagbytopnew.h"
#include "mnemonic/jagbytopnewarray.h"
#include "mnemonic/jagbytoppush.h"
#include "mnemonic/jagbytoppushindex.h"
#include "mnemonic/jagbytopput.h"
#include "mnemonic/jagbytopreturn.h"
#include "mnemonic/jagbytopstorearray.h"
#include "mnemonic/jagbytopstorefastindex.h"
#include "mnemonic/jagbytopstoreindex.h"
#include "mnemonic/jagbytoptableswitch.h"
#include "mnemonic/jagbytopthrow.h"
#include "mnemonic/jagbytopvalueconvert.h"

#include "jagbytimnemonicblock.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytMnemonicBlock"
#include <logging/catlog.h>

struct _JagBytMnemonicBlockPrivate {
	CatStringWo *e_bytecode;
	CatArrayWo *e_opperationList;
	JagBytLabelRepository *label_repository;
};

static void l_mnemonic_block_iface_init(JagBytIMnemonicBlockInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytMnemonicBlock, jag_byt_mnemonic_block, G_TYPE_OBJECT ,{
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC_BLOCK, l_mnemonic_block_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_mnemonic_block_class_init(JagBytMnemonicBlockClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytMnemonicBlockPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_mnemonic_block_init(JagBytMnemonicBlock *instance) {
	JagBytMnemonicBlockPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_MNEMONIC_BLOCK, JagBytMnemonicBlockPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytMnemonicBlock *instance = JAG_BYT_MNEMONIC_BLOCK(object);
	JagBytMnemonicBlockPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_bytecode);
	cat_unref_ptr(priv->label_repository);
	cat_unref_ptr(priv->e_opperationList);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_run(JagBytMnemonicBlock *block);
static void l_resolve_labels(JagBytMnemonicBlock *block);

JagBytMnemonicBlock *jag_byt_mnemonic_block_new(CatStringWo *e_bytecode) {
	JagBytMnemonicBlock *result = g_object_new(JAG_BYT_TYPE_MNEMONIC_BLOCK, NULL);
	cat_ref_anounce(result);
	JagBytMnemonicBlockPrivate *priv = result->priv;
	priv->e_bytecode = cat_ref_ptr(e_bytecode);
	priv->e_opperationList = cat_array_wo_new();
	l_run(result);
	l_resolve_labels(result);
	return result;
}


JagBytLabelRepository *jag_byt_mnemonic_block_get_label_repository(JagBytMnemonicBlock *mnemonic_block) {
	JagBytMnemonicBlockPrivate *priv = JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(mnemonic_block);
	return priv->label_repository;
}

int jag_byt_mnemonic_block_find_by_bytecode_offset(JagBytMnemonicBlock *mnemonic_block, int bytecode_offset) {
	JagBytMnemonicBlockPrivate *priv = JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(mnemonic_block);
	int index;
	int op_count = cat_array_wo_size(priv->e_opperationList);
	for(index=0; index<op_count; index++) {
		JagBytIMnemonic *opp = (JagBytIMnemonic *) cat_array_wo_get(priv->e_opperationList, index);
		int op_offset = jag_byt_imnemonic_get_offset(opp);
		if (bytecode_offset>=op_offset && bytecode_offset<op_offset+jag_byt_imnemonic_get_length(opp)) {
			return index;
		}
	}
	return op_count;
}

int jag_byt_mnemonic_block_count(JagBytMnemonicBlock *mnemonic_block) {
	JagBytMnemonicBlockPrivate *priv = JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(mnemonic_block);
	return cat_array_wo_size(priv->e_opperationList);
}

JagBytIMnemonic *jag_byt_mnemonic_block_get(JagBytMnemonicBlock *mnemonic_block, int index) {
	JagBytMnemonicBlockPrivate *priv = JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(mnemonic_block);
	return (JagBytIMnemonic *) cat_array_wo_get(priv->e_opperationList, index);
}


JagBytLabel *jag_byt_mnemonic_block_find_or_create_label_at(JagBytMnemonicBlock *mnemonic_block, int mnemonic_index) {
	JagBytMnemonicBlockPrivate *priv = JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(mnemonic_block);
	JagBytLabel *label = jag_byt_label_repository_get(priv->label_repository, mnemonic_index);
	if (label==NULL) {
		int bytecodeOffset = -1;
		if (mnemonic_index>=cat_array_wo_size(priv->e_opperationList)) {
			JagBytIMnemonic *mnemonic = (JagBytIMnemonic *) cat_array_wo_get_last(priv->e_opperationList);
			bytecodeOffset = jag_byt_imnemonic_get_continues_offset(mnemonic);
		} else {
			JagBytIMnemonic *mnemonic = (JagBytIMnemonic *) cat_array_wo_get(priv->e_opperationList, mnemonic_index);
			bytecodeOffset = jag_byt_imnemonic_get_offset(mnemonic);
		}
		label = jag_byt_label_repository_create_soft(priv->label_repository, mnemonic_index, bytecodeOffset);
	}
	return label;
}



static void l_resolve_labels(JagBytMnemonicBlock *block) {
	JagBytMnemonicBlockPrivate *priv = JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(block);
	priv->label_repository = jag_byt_label_repository_new();
	int oppIndex = 0;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_opperationList);
	while(cat_iiterator_has_next(iter)) {
		JagBytIMnemonic *opp = (JagBytIMnemonic *) cat_iiterator_next(iter);
		int bytecodeOffset = jag_byt_imnemonic_get_branch_offset(opp);
		if (bytecodeOffset>=0) {
			jag_byt_label_repository_create_soft(priv->label_repository, oppIndex, jag_byt_imnemonic_get_offset(opp));
			int oppOffset = jag_byt_mnemonic_block_find_by_bytecode_offset(block, bytecodeOffset);
			cat_log_detail("bytecode-offset=%d, mnemonic-index=%d", bytecodeOffset, oppOffset);
			jag_byt_label_repository_create_hard(priv->label_repository, oppOffset, bytecodeOffset);

			int contByteCodeOffset = jag_byt_imnemonic_get_continues_offset(opp);
			if (contByteCodeOffset>=0) {
				int oppContOffset = jag_byt_mnemonic_block_find_by_bytecode_offset(block, contByteCodeOffset);
				cat_log_detail("bytecode-offset=%d, mnemonic-continue-index=%d", contByteCodeOffset, oppContOffset);
				jag_byt_label_repository_create_soft(priv->label_repository, oppContOffset, contByteCodeOffset);
			}
		}
		oppIndex++;
	}
	cat_unref_ptr(iter);


}


static int l_read_signed_int(CatStringWo *e_bytecode, int *ptr) {
	int of = *ptr;
	int a = cat_string_wo_char_at(e_bytecode, of++);
	a = (a<<8) + (0xFF & cat_string_wo_char_at(e_bytecode, of++));
	a = (a<<8) + (0xFF & cat_string_wo_char_at(e_bytecode, of++));
	a = (a<<8) + (0xFF & cat_string_wo_char_at(e_bytecode, of++));
	*ptr = of;
	return a;
}


static int l_read_signed_short(CatStringWo *e_bytecode, int *ptr) {
	int of = *ptr;
	int a = cat_string_wo_char_at(e_bytecode, of++);
	int b = 0xFF & cat_string_wo_char_at(e_bytecode, of++);

	*ptr = of;
	return (a<<8) + b;
}

static int l_read_unsigned_short(CatStringWo *e_bytecode, int *ptr) {
	int of = *ptr;
	int a = 0xFF & cat_string_wo_char_at(e_bytecode, of++);
	int b = 0xFF & cat_string_wo_char_at(e_bytecode, of++);

	*ptr = of;
	return 0xFFFF & ((a<<8) + b);
}

static int l_read_signed_byte(CatStringWo *e_bytecode, int *ptr) {
	int of = *ptr;
	int a = cat_string_wo_char_at(e_bytecode, of++);

	*ptr = of;
	return a;
}

static int l_read_unsigned_byte(CatStringWo *e_bytecode, int *ptr) {
	int of = *ptr;
	int a = cat_string_wo_char_at(e_bytecode, of++);

	*ptr = of;
	return a & 0xFF;
}


static void l_run(JagBytMnemonicBlock *block) {
	JagBytMnemonicBlockPrivate *priv = JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(block);
	gboolean isWide = FALSE;
	int startAdres = 0;
	int bytecodePtr = 0;
	CatStringWo *e_bytecode = priv->e_bytecode;
	while(bytecodePtr<cat_string_wo_length(e_bytecode)) {
		startAdres = bytecodePtr;
		int opcode = (0xff & cat_string_wo_char_at(e_bytecode, bytecodePtr++));
//		cat_log_error("startAdres=%04x, opcode=%2x", startAdres, opcode);

		JagBytIMnemonic *opperation = NULL;
		switch(opcode) {
				case 0x00 : {
////					lines.add("nop");
				} break;
				case 0x01 : {
					opperation = (JagBytIMnemonic *) jag_byt_op_load_null_new(startAdres);
				} break;
				case 0x02 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_ICONST_M1, JAG_BYT_TYPE_INT, (CatIStringable *) cat_integer_new(-1), startAdres); break;
				case 0x03 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_ICONST_0, JAG_BYT_TYPE_INT, (CatIStringable *) cat_integer_new(0), startAdres); break;
				case 0x04 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_ICONST_1, JAG_BYT_TYPE_INT, (CatIStringable *) cat_integer_new(1), startAdres); break;
				case 0x05 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_ICONST_2, JAG_BYT_TYPE_INT, (CatIStringable *) cat_integer_new(2), startAdres); break;
				case 0x06 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_ICONST_3, JAG_BYT_TYPE_INT, (CatIStringable *) cat_integer_new(3), startAdres); break;
				case 0x07 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_ICONST_4, JAG_BYT_TYPE_INT, (CatIStringable *) cat_integer_new(4), startAdres); break;
				case 0x08 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_ICONST_5, JAG_BYT_TYPE_INT, (CatIStringable *) cat_integer_new(5), startAdres); break;
				case 0x09 : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_LCONST_0, JAG_BYT_TYPE_LONG, (CatIStringable *) cat_integer_new(0), startAdres); break;	// TODO value needs to be CatLong iso CatInteger
				case 0x0a : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_LCONST_1, JAG_BYT_TYPE_LONG, (CatIStringable *) cat_integer_new(1), startAdres); break;
				case 0x0b : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_FCONST_0, JAG_BYT_TYPE_FLOAT, (CatIStringable *) cat_integer_new(0), startAdres); break;
				case 0x0c : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_FCONST_1, JAG_BYT_TYPE_FLOAT, (CatIStringable *) cat_integer_new(1), startAdres); break;
				case 0x0d : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_FCONST_2, JAG_BYT_TYPE_FLOAT, (CatIStringable *) cat_integer_new(2), startAdres); break;
				case 0x0e : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_DCONST_0, JAG_BYT_TYPE_DOUBLE, (CatIStringable *) cat_integer_new(0), startAdres); break;
				case 0x0f : opperation = (JagBytIMnemonic *) jag_byt_op_load_constant_new(OP_DCONST_1, JAG_BYT_TYPE_DOUBLE, (CatIStringable *) cat_integer_new(1), startAdres); break;
				case 0x10 : {
					int value = l_read_signed_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_push_new_byte(startAdres, value);
				} break;
				case 0x11 : {
					int value = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_push_new_short(startAdres, value);
				} break;
				case 0x12 : {
					int index = l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_push_index_new(OP_LDC, startAdres, 2, FALSE, index, FALSE);
				} break;
				case 0x13 : {
					int index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_push_index_new(OP_LDC_W, startAdres, 3, TRUE, index, FALSE);
				} break;
				case 0x14 : {
					int index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_push_index_new(OP_LDC2_W, startAdres, 3, TRUE, index, TRUE);
				} break;
				case 0x15 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_load_index_new(OP_ILOAD, startAdres, JAG_BYT_TYPE_INT, index, isWide);
				} break;
				case 0x16 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_load_index_new(OP_LLOAD, startAdres, JAG_BYT_TYPE_LONG, index, isWide);
				} break;
				case 0x17 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_load_index_new(OP_FLOAD, startAdres, JAG_BYT_TYPE_FLOAT, index, isWide);
				} break;
				case 0x18 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_load_index_new(OP_DLOAD, startAdres, JAG_BYT_TYPE_DOUBLE, index, isWide);
				} break;
				case 0x19 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_load_index_new(OP_ALOAD, startAdres, JAG_BYT_TYPE_REFERENCE, index, isWide);
				} break;
				case 0x1a : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ILOAD_0, startAdres, JAG_BYT_TYPE_INT, 0); break;
				case 0x1b : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ILOAD_1, startAdres, JAG_BYT_TYPE_INT, 1); break;
				case 0x1c : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ILOAD_2, startAdres, JAG_BYT_TYPE_INT, 2); break;
				case 0x1d : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ILOAD_3, startAdres, JAG_BYT_TYPE_INT, 3); break;
				case 0x1e : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_LLOAD_0, startAdres, JAG_BYT_TYPE_LONG, 0); break;
				case 0x1f : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_LLOAD_1, startAdres, JAG_BYT_TYPE_LONG, 1); break;
				case 0x20 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_LLOAD_2, startAdres, JAG_BYT_TYPE_LONG, 2); break;
				case 0x21 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_LLOAD_3, startAdres, JAG_BYT_TYPE_LONG, 3); break;
				case 0x22 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_FLOAD_0, startAdres, JAG_BYT_TYPE_FLOAT, 0); break;
				case 0x23 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_FLOAD_1, startAdres, JAG_BYT_TYPE_FLOAT, 1); break;
				case 0x24 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_FLOAD_2, startAdres, JAG_BYT_TYPE_FLOAT, 2); break;
				case 0x25 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_FLOAD_3, startAdres, JAG_BYT_TYPE_FLOAT, 3); break;
				case 0x26 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_DLOAD_0, startAdres, JAG_BYT_TYPE_DOUBLE, 0); break;
				case 0x27 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_DLOAD_1, startAdres, JAG_BYT_TYPE_DOUBLE, 1); break;
				case 0x28 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_DLOAD_2, startAdres, JAG_BYT_TYPE_DOUBLE, 2); break;
				case 0x29 : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_DLOAD_3, startAdres, JAG_BYT_TYPE_DOUBLE, 3); break;
				case 0x2a : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ALOAD_0, startAdres, JAG_BYT_TYPE_REFERENCE, 0); break;
				case 0x2b : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ALOAD_1, startAdres, JAG_BYT_TYPE_REFERENCE, 1); break;
				case 0x2c : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ALOAD_2, startAdres, JAG_BYT_TYPE_REFERENCE, 2); break;
				case 0x2d : opperation = (JagBytIMnemonic *) jag_byt_op_load_fast_index_new(OP_ALOAD_3, startAdres, JAG_BYT_TYPE_REFERENCE, 3); break;
				case 0x2e : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_IALOAD, startAdres, JAG_BYT_TYPE_INT); break;
				case 0x2f : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_LALOAD, startAdres, JAG_BYT_TYPE_LONG); break;
				case 0x30 : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_FALOAD, startAdres, JAG_BYT_TYPE_FLOAT); break;
				case 0x31 : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_DALOAD, startAdres, JAG_BYT_TYPE_DOUBLE); break;
				case 0x32 : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_AALOAD, startAdres, JAG_BYT_TYPE_REFERENCE); break;
				case 0x33 : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_BALOAD, startAdres, JAG_BYT_TYPE_BYTE); break;
				case 0x34 : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_CALOAD, startAdres, JAG_BYT_TYPE_CHAR); break;
				case 0x35 : opperation = (JagBytIMnemonic *) jag_byt_op_load_array_new(OP_SALOAD, startAdres, JAG_BYT_TYPE_SHORT); break;
				case 0x36 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_store_index_new(OP_ISTORE, startAdres, JAG_BYT_TYPE_INT, index, isWide);
				} break;
				case 0x37 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_store_index_new(OP_LSTORE, startAdres, JAG_BYT_TYPE_LONG, index, isWide);
				} break;
				case 0x38 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_store_index_new(OP_FSTORE, startAdres, JAG_BYT_TYPE_FLOAT, index, isWide);
				} break;
				case 0x39 : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_store_index_new(OP_DSTORE, startAdres, JAG_BYT_TYPE_DOUBLE, index, isWide);
				} break;
				case 0x3a : {
					int index = isWide ? l_read_unsigned_short(e_bytecode, &bytecodePtr) : l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_store_index_new(OP_ASTORE, startAdres, JAG_BYT_TYPE_REFERENCE, index, isWide);
				} break;
				case 0x3b : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ISTORE_0, startAdres, JAG_BYT_TYPE_INT, 0); break;
				case 0x3c : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ISTORE_1, startAdres, JAG_BYT_TYPE_INT, 1); break;
				case 0x3d : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ISTORE_2, startAdres, JAG_BYT_TYPE_INT, 2); break;
				case 0x3e : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ISTORE_3, startAdres, JAG_BYT_TYPE_INT, 3); break;
				case 0x3f : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_LSTORE_0, startAdres, JAG_BYT_TYPE_LONG, 0); break;
				case 0x40 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_LSTORE_1, startAdres, JAG_BYT_TYPE_LONG, 1); break;
				case 0x41 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_LSTORE_2, startAdres, JAG_BYT_TYPE_LONG, 2); break;
				case 0x42 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_LSTORE_3, startAdres, JAG_BYT_TYPE_LONG, 3); break;
				case 0x43 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_FSTORE_0, startAdres, JAG_BYT_TYPE_FLOAT, 0); break;
				case 0x44 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_FSTORE_1, startAdres, JAG_BYT_TYPE_FLOAT, 1); break;
				case 0x45 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_FSTORE_2, startAdres, JAG_BYT_TYPE_FLOAT, 2); break;
				case 0x46 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_FSTORE_3, startAdres, JAG_BYT_TYPE_FLOAT, 3); break;
				case 0x47 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_DSTORE_0, startAdres, JAG_BYT_TYPE_DOUBLE, 0); break;
				case 0x48 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_DSTORE_1, startAdres, JAG_BYT_TYPE_DOUBLE, 1); break;
				case 0x49 : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_DSTORE_2, startAdres, JAG_BYT_TYPE_DOUBLE, 2); break;
				case 0x4a : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_DSTORE_3, startAdres, JAG_BYT_TYPE_DOUBLE, 3); break;
				case 0x4b : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ASTORE_0, startAdres, JAG_BYT_TYPE_REFERENCE, 0); break;
				case 0x4c : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ASTORE_1, startAdres, JAG_BYT_TYPE_REFERENCE, 1); break;
				case 0x4d : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ASTORE_2, startAdres, JAG_BYT_TYPE_REFERENCE, 2); break;
				case 0x4e : opperation = (JagBytIMnemonic *) jag_byt_op_store_fast_index_new(OP_ASTORE_3, startAdres, JAG_BYT_TYPE_REFERENCE, 3); break;
				case 0x4f : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_IASTORE, startAdres, JAG_BYT_TYPE_INT); break;
				case 0x50 : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_LASTORE, startAdres, JAG_BYT_TYPE_LONG); break;
				case 0x51 : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_FASTORE, startAdres, JAG_BYT_TYPE_FLOAT); break;
				case 0x52 : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_DASTORE, startAdres, JAG_BYT_TYPE_DOUBLE); break;
				case 0x53 : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_AASTORE, startAdres, JAG_BYT_TYPE_REFERENCE); break;
				case 0x54 : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_BASTORE, startAdres, JAG_BYT_TYPE_BYTE); break;
				case 0x55 : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_CASTORE, startAdres, JAG_BYT_TYPE_CHAR); break;
				case 0x56 : opperation = (JagBytIMnemonic *) jag_byt_op_store_array_new(OP_SASTORE, startAdres, JAG_BYT_TYPE_SHORT); break;
//				case 0x57 : {
//					lines.add("pop");
//				} break;
//				case 0x58 : {
//					lines.add("pop2");
//				} break;
				case 0x59 : opperation = (JagBytIMnemonic *) jag_byt_op_duplicate_new(OP_DUP, startAdres); break;
				case 0x5a : opperation = (JagBytIMnemonic *) jag_byt_op_duplicate_new(OP_DUP_X1, startAdres); break;
				case 0x5b : opperation = (JagBytIMnemonic *) jag_byt_op_duplicate_new(OP_DUP_X2, startAdres); break;
				case 0x5c : opperation = (JagBytIMnemonic *) jag_byt_op_duplicate_new(OP_DUP2, startAdres); break;
				case 0x5d : opperation = (JagBytIMnemonic *) jag_byt_op_duplicate_new(OP_DUP2_X1, startAdres); break;
				case 0x5e : opperation = (JagBytIMnemonic *) jag_byt_op_duplicate_new(OP_DUP2_X2, startAdres); break;
//				case 0x5f : {
//					lines.add("swap");
//				} break;
				case 0x60 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_IADD, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_ADD); break;
				case 0x61 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_LADD, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_ADD); break;
				case 0x62 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_FADD, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_MATH_OPERATOR_ADD); break;
				case 0x63 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_DADD, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_MATH_OPERATOR_ADD); break;
				case 0x64 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_ISUB, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_SUB); break;
				case 0x65 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_LSUB, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_SUB); break;
				case 0x66 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_FSUB, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_MATH_OPERATOR_SUB); break;
				case 0x67 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_DSUB, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_MATH_OPERATOR_SUB); break;
				case 0x68 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_IMUL, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_MULTIPLY); break;
				case 0x69 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_LMUL, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_MULTIPLY); break;
				case 0x6a : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_FMUL, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_MATH_OPERATOR_MULTIPLY); break;
				case 0x6b : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_DMUL, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_MATH_OPERATOR_MULTIPLY); break;
				case 0x6c : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_IDIV, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_DIVIDE); break;
				case 0x6d : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_LDIV, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_DIVIDE); break;
				case 0x6e : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_FDIV, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_MATH_OPERATOR_DIVIDE); break;
				case 0x6f : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_DDIV, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_MATH_OPERATOR_DIVIDE); break;
				case 0x70 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_IREM, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_REMAINDER); break;
				case 0x71 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_LREM, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_REMAINDER); break;
				case 0x72 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_FREM, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_MATH_OPERATOR_REMAINDER); break;
				case 0x73 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_DREM, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_MATH_OPERATOR_REMAINDER); break;
				case 0x74 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_INEG, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_UNARY_MINUS); break;
				case 0x75 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_LNEG, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_UNARY_MINUS); break;
				case 0x76 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_FNEG, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_MATH_OPERATOR_UNARY_MINUS); break;
				case 0x77 : opperation = (JagBytIMnemonic *) jag_byt_op_math_basic_new(OP_DNEG, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_MATH_OPERATOR_UNARY_MINUS); break;

				case 0x78 : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_ISHL, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_SHIFT_LEFT); break;
				case 0x79 : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_LSHL, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_SHIFT_LEFT); break;
				case 0x7a : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_ISHR, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_SHIFT_RIGHT); break;
				case 0x7b : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_LSHR, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_SHIFT_RIGHT); break;
				case 0x7c : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_IUSHR, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_UNSIGNED_SHIFT_RIGHT); break;
				case 0x7d : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_LUSHR, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_UNSIGNED_SHIFT_RIGHT); break;
				case 0x7e : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_IAND, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_BITWISE_AND); break;
				case 0x7f : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_LAND, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_BITWISE_AND); break;
				case 0x80 : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_IOR, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_BITWISE_OR); break;
				case 0x81 : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_LOR, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_BITWISE_OR); break;
				case 0x82 : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_IXOR, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_MATH_OPERATOR_BITWISE_XOR); break;
				case 0x83 : opperation = (JagBytIMnemonic *) jag_byt_op_math_logic_new(OP_LXOR, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_MATH_OPERATOR_BITWISE_XOR); break;
				case 0x84 : {
					int index = 0;
					int constant = 0;
					if (isWide) {
						index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
						constant = l_read_signed_short(e_bytecode, &bytecodePtr);
					} else {
						index =  l_read_unsigned_byte(e_bytecode, &bytecodePtr);
						constant =  l_read_signed_byte(e_bytecode, &bytecodePtr);
					}
					opperation = (JagBytIMnemonic *) jag_byt_op_increment_new(startAdres, index, constant, isWide);
				} break;
				case 0x85 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_I2L, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_TYPE_LONG); break;
				case 0x86 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_I2F, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_TYPE_FLOAT); break;
				case 0x87 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_I2D, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_TYPE_DOUBLE); break;
				case 0x88 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_L2I, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_TYPE_INT); break;
				case 0x89 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_L2F, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_TYPE_FLOAT); break;
				case 0x8a : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_L2D, startAdres, JAG_BYT_TYPE_LONG, JAG_BYT_TYPE_DOUBLE); break;
				case 0x8b : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_F2I, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_TYPE_INT); break;
				case 0x8c : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_F2L, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_TYPE_LONG); break;
				case 0x8d : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_F2D, startAdres, JAG_BYT_TYPE_FLOAT, JAG_BYT_TYPE_DOUBLE); break;
				case 0x8e : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_D2I, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_TYPE_INT); break;
				case 0x8f : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_D2L, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_TYPE_LONG); break;
				case 0x90 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_D2F, startAdres, JAG_BYT_TYPE_DOUBLE, JAG_BYT_TYPE_FLOAT); break;
				case 0x91 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_I2B, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_TYPE_BYTE); break;
				case 0x92 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_I2C, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_TYPE_CHAR); break;
				case 0x93 : opperation = (JagBytIMnemonic *) jag_byt_op_value_convert_new(OP_I2S, startAdres, JAG_BYT_TYPE_INT, JAG_BYT_TYPE_SHORT); break;
//				case 0x94 : {
//					lines.add("lcmp");
//				} break;
//				case 0x95 : {
//					lines.add("fcmpl");
//				} break;
//				case 0x96 : {
//					lines.add("fcmpg");
//				} break;
//				case 0x97 : {
//					lines.add("dcmpl");
//				} break;
//				case 0x98 : {
//					lines.add("dcmpg");
//				} break;
				case 0x99 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IFEQ, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_EQUAL, TRUE);
				} break;
				case 0x9a : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IFNE, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_NOT_EQUAL, TRUE);
				} break;
				case 0x9b : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IFLT, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_LESS_THEN, TRUE);
				} break;
				case 0x9c : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IFGE, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_GREATER_OR_EQUAL, TRUE);
				} break;
				case 0x9d : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IFGT, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_GREATER_THEN, TRUE);
				} break;
				case 0x9e : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IFLE, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_LESS_OR_EQUAL, TRUE);
				} break;
				case 0x9f : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IF_ICMPEQ, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_EQUAL, FALSE);
				} break;
				case 0xa0 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IF_ICMPNE, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_NOT_EQUAL, FALSE);
				} break;
				case 0xa1 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IF_ICMPLT, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_LESS_THEN, FALSE);
				} break;
				case 0xa2 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IF_ICMPGE, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_GREATER_OR_EQUAL, FALSE);
				} break;
				case 0xa3 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IF_ICMPGT, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_GREATER_THEN, FALSE);
				} break;
				case 0xa4 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_int_new(OP_IF_ICMPLE, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_LESS_OR_EQUAL, FALSE);
				} break;
				case 0xa5 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_reference_new(OP_IF_ACMPEQ, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_EQUAL, FALSE);
				} break;
				case 0xa6 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_reference_new(OP_IF_ACMPNE, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_NOT_EQUAL, FALSE);
				} break;
				case 0xa7 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_goto_new(OP_GOTO, startAdres, branch_offset);

				} break;
//				case 0xa8 : {
//					lines.add("jsr");
//				} break;
//				case 0xa9 : {
//					lines.add("ret");
//				} break;
				case 0xaa : {
					while(bytecodePtr%4!=0) {
						bytecodePtr++;
					}
					int default_value = l_read_signed_int(e_bytecode, &bytecodePtr);
					int low_value = l_read_signed_int(e_bytecode, &bytecodePtr);
					int high_value = l_read_signed_int(e_bytecode, &bytecodePtr);

					int count = high_value-low_value+1;
					int *table = g_malloc(sizeof(int) *count);
					cat_log_trace("nr.etnries=%d", count);
					int idx;
					for(idx=0; idx<count; idx++) {
						int offset = l_read_signed_int(e_bytecode, &bytecodePtr);
//						offset+=startAdres; // TODO test!
						table[idx] = offset;
					}
					opperation = (JagBytIMnemonic *) jag_byt_op_table_switch_new(startAdres, default_value, low_value, high_value, table);
				} break;
//				case 0xab : {
//					lines.add("lookupswitch");
//				} break;
				case 0xac : opperation = (JagBytIMnemonic *) jag_byt_op_return_new(OP_IRETURN, startAdres, JAG_BYT_TYPE_INT); break;
				case 0xad : opperation = (JagBytIMnemonic *) jag_byt_op_return_new(OP_LRETURN, startAdres, JAG_BYT_TYPE_LONG); break;
				case 0xae : opperation = (JagBytIMnemonic *) jag_byt_op_return_new(OP_FRETURN, startAdres, JAG_BYT_TYPE_FLOAT); break;
				case 0xaf : opperation = (JagBytIMnemonic *) jag_byt_op_return_new(OP_DRETURN, startAdres, JAG_BYT_TYPE_DOUBLE); break;
				case 0xb0 : opperation = (JagBytIMnemonic *) jag_byt_op_return_new(OP_ARETURN, startAdres, JAG_BYT_TYPE_REFERENCE); break;
				case 0xb1 : opperation = (JagBytIMnemonic *) jag_byt_op_return_new(OP_RETURN, startAdres, JAG_BYT_TYPE_VOID); break;
				case 0xb3 : {
					int index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_put_new(OP_PUTSTATIC, startAdres, index);
				} break;
				case 0xb5 : {
					int index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_put_new(OP_PUTFIELD, startAdres, index);
				} break;
				case 0xb2 : {
					int index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_get_new(OP_GETSTATIC, startAdres, index);
				} break;
				case 0xb4 : {
					int index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_get_new(OP_GETFIELD, startAdres, index);
				} break;
				case 0xb6 : {
					int sym_ref_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_invoke_new(OP_INVOKEVIRTUAL, startAdres, sym_ref_index);
				} break;
				case 0xb7 : {
					int sym_ref_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_invoke_new(OP_INVOKESPECIAL, startAdres, sym_ref_index);
				} break;
				case 0xb8 : {
					int sym_ref_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_invoke_new(OP_INVOKESTATIC, startAdres, sym_ref_index);
				} break;
				case 0xb9 : {
					int sym_ref_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					int count = l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					bytecodePtr++;
					opperation = (JagBytIMnemonic *) jag_byt_op_invoke_new_interface(startAdres, sym_ref_index, count);
				} break;
				case 0xba : {
					int sym_ref_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					bytecodePtr+=2;
					opperation = (JagBytIMnemonic *) jag_byt_op_invoke_new(OP_INVOKEDYNAMIC, startAdres, sym_ref_index);
				} break;
				case 0xbb : {
					int class_pool_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_new_new(startAdres, class_pool_index);
				} break;
				case 0xbc : {
					int type_val = l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					JagBytType type = JAG_BYT_TYPE_INT;
					switch(type_val) {
						case 4 : type=JAG_BYT_TYPE_BOOLEAN; break;
						case 5 : type=JAG_BYT_TYPE_CHAR; break;
						case 6 : type=JAG_BYT_TYPE_FLOAT; break;
						case 7 : type=JAG_BYT_TYPE_DOUBLE; break;
						case 8 : type=JAG_BYT_TYPE_BYTE; break;
						case 9 : type=JAG_BYT_TYPE_SHORT; break;
						case 10 : type=JAG_BYT_TYPE_INT; break;
						case 11 : type=JAG_BYT_TYPE_LONG; break;
					}
					opperation = (JagBytIMnemonic *) jag_byt_op_new_array_new_primitive(startAdres, type);
				} break;
				case 0xbd : {
					int class_pool_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_new_array_new_reference(startAdres, class_pool_index);
				} break;
				case 0xbe : {
					opperation = (JagBytIMnemonic *) jag_byt_op_array_length_new(startAdres);
				} break;
				case 0xbf : {
					opperation = (JagBytIMnemonic *) jag_byt_op_throw_new(startAdres);
				} break;
				case 0xc0 : {
					int class_pool_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_check_cast_new(OP_CHECKCAST, startAdres, class_pool_index);
				} break;
				case 0xc1 : {
					int class_pool_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_check_cast_new(OP_INSTANCEOF, startAdres, class_pool_index);
				} break;
				case 0xc2 : opperation = (JagBytIMnemonic *) jag_byt_op_monitor_new(OP_MONITORENTER, startAdres); break;
				case 0xc3 : opperation = (JagBytIMnemonic *) jag_byt_op_monitor_new(OP_MONITOREXIT, startAdres); break;
				case 0xc4 : {
					isWide = TRUE;
					continue;
				} break;
				case 0xc5 : {
					int class_pool_index = l_read_unsigned_short(e_bytecode, &bytecodePtr);
					int dim_count = l_read_unsigned_byte(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_multi_a_new_array_new(startAdres, class_pool_index, dim_count);
				} break;
				case 0xc6 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_reference_new(OP_IFNULL, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_EQUAL, TRUE);
				} break;
				case 0xc7 : {
					int branch_offset = l_read_signed_short(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_compare_reference_new(OP_IFNONNULL, startAdres, branch_offset, JAG_BYT_CONDITION_TYPE_NOT_EQUAL, TRUE);
				} break;
				case 0xc8 : {
					int branch_offset = l_read_signed_int(e_bytecode, &bytecodePtr);
					opperation = (JagBytIMnemonic *) jag_byt_op_goto_new(OP_GOTO_W, startAdres, branch_offset);
				} break;
//				case 0xc9 : {
//					lines.add("jsr_w");
//				} break;
//				default : {
//					Integer b = new Integer(0xff & opcode);
//					lines.add(""+Integer.toHexString(b));
//				}
		}
		if (opperation!=NULL) {
			cat_array_wo_append(priv->e_opperationList, (GObject *) opperation);
			cat_unref(opperation);
			isWide = FALSE;


			cat_log_on_detail({
				char buf[1000];
				char *bufptr = buf;
				int si;
				for(si=startAdres; si<bytecodePtr; si++) {
					sprintf(bufptr, "%02x ", (0xFF & cat_string_wo_char_at(e_bytecode, si)));
					bufptr+=3;
				}
				cat_log_detail("startAdres=%04x, %s", startAdres, buf);
			});


		} else {
			cat_log_error("unresolved:opcode=%2x", opcode);
		}
	}
}






static JagBytIMnemonic *l_mnemonic_block_get(JagBytIMnemonicBlock *self, int index) {
	return jag_byt_mnemonic_block_get(JAG_BYT_MNEMONIC_BLOCK(self), index);
}

static int l_mnemonic_block_count(JagBytIMnemonicBlock *self) {
	return jag_byt_mnemonic_block_count(JAG_BYT_MNEMONIC_BLOCK(self));
}

static int l_mnemonic_block_find_by_bytecode_offset(JagBytIMnemonicBlock *self, int bytecodeOffset) {
	return jag_byt_mnemonic_block_find_by_bytecode_offset(JAG_BYT_MNEMONIC_BLOCK(self), bytecodeOffset);
}

static JagBytLabelRepository *l_mnemonic_block_get_label_repository(JagBytIMnemonicBlock *self) {
	return JAG_BYT_MNEMONIC_BLOCK_GET_PRIVATE(self)->label_repository;
}


static void l_mnemonic_block_iface_init(JagBytIMnemonicBlockInterface *iface) {
	iface->get = l_mnemonic_block_get;
	iface->count = l_mnemonic_block_count;
	iface->findByBytecodeOffset = l_mnemonic_block_find_by_bytecode_offset;
	iface->getLabelRepository = l_mnemonic_block_get_label_repository;
}
