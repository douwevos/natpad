/*
   File:    jagdecmnemoniclinenumbertable.h
   Project: jaguar-decompiler
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


#ifndef JAGDECMNEMONICLINENUMBERTABLE_H_
#define JAGDECMNEMONICLINENUMBERTABLE_H_

#include <classfile/bytecode/jagbytimnemonicblock.h>
#include <classfile/attribute/jagbytlinenumbertable.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE              (jag_dec_mnemonic_line_number_table_get_type())
#define JAG_DEC_MNEMONIC_LINE_NUMBER_TABLE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_mnemonic_line_number_table_get_type(), JagDecMnemonicLineNumberTable))
#define JAG_DEC_MNEMONIC_LINE_NUMBER_TABLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE, JagDecMnemonicLineNumberTableClass))
#define JAG_DEC_IS_MNEMONIC_LINE_NUMBER_TABLE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE))
#define JAG_DEC_IS_MNEMONIC_LINE_NUMBER_TABLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE))
#define JAG_DEC_MNEMONIC_LINE_NUMBER_TABLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE, JagDecMnemonicLineNumberTableClass))
#define JAG_DEC_MNEMONIC_LINE_NUMBER_TABLE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE, JagDecMnemonicLineNumberTablePrivate))

typedef struct _JagDecMnemonicLineNumberTable               JagDecMnemonicLineNumberTable;
typedef struct _JagDecMnemonicLineNumberTablePrivate        JagDecMnemonicLineNumberTablePrivate;
typedef struct _JagDecMnemonicLineNumberTableClass          JagDecMnemonicLineNumberTableClass;


struct _JagDecMnemonicLineNumberTable {
	JagBytLineNumberTable parent;
	JagDecMnemonicLineNumberTablePrivate *priv;
};

struct _JagDecMnemonicLineNumberTableClass {
	JagBytLineNumberTableClass parent_class;
};


GType jag_dec_mnemonic_line_number_table_get_type();

JagDecMnemonicLineNumberTable *jag_dec_mnemonic_line_number_table_new(JagBytIMnemonicBlock *mnemonic_block, JagBytLineNumberTable *line_number_table);

int jag_dec_mnemonic_line_number_table_get_least_line_number(JagDecMnemonicLineNumberTable *table);

G_END_DECLS


#endif /* JAGDECMNEMONICLINENUMBERTABLE_H_ */
