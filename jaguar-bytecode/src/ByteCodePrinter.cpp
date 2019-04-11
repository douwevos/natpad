/*
 * ByteCodePrinter.cpp
 *
 *  Created on: Apr 10, 2019
 *      Author: dvos
 */

#include <string.h>
#include <string>
#include "ByteCodePrinter.h"
#include "classfile/bytecode/jagbytmnemonicblock.h"
#include "classfile/bytecode/jagbytimnemonic.h"
#include <caterpillar.h>

ByteCodePrinter::ByteCodePrinter() {

}

ByteCodePrinter::~ByteCodePrinter() {
}

char *ByteCodePrinter::print(JagBytClassfile *classfile) {
	std::string buf;
	JagBytMethodList *method_list = jag_byt_classfile_get_method_list(classfile);
	CatIIterator *method_iter = jag_byt_method_list_iterator(method_list);
	while(cat_iiterator_has_next(method_iter)) {
		JagBytMethod *method = (JagBytMethod *) cat_iiterator_next(method_iter);
		CatStringWo *name = jag_byt_method_get_name(method);
		buf.append(cat_string_wo_getchars(name));
		buf.append("\n");

		CatStringWo *bytecode = jag_byt_method_get_bytecode(method);
		const char *raw_bytes = cat_string_wo_getchars(bytecode);
		JagBytMnemonicBlock *mnemonic_block = jag_byt_mnemonic_block_new(bytecode);
		JagBytLabelRepository *label_repo = jag_byt_mnemonic_block_get_label_repository(mnemonic_block);
		int count = jag_byt_mnemonic_block_count(mnemonic_block);
		int idx;
		for(idx=0; idx<count; idx++) {
			JagBytIMnemonic *mnemonic = jag_byt_mnemonic_block_get(mnemonic_block, idx);
			int offset = jag_byt_imnemonic_get_offset(mnemonic);
			int length = jag_byt_imnemonic_get_length(mnemonic);
			CatStringWo *e_line = cat_string_wo_new_with("      ");
//			cat_string_wo_fo
			int boff;
			for(boff=0; boff<length; boff++) {
				cat_string_wo_append_hexadecimal(e_line, 0xff & raw_bytes[boff+offset], 2);
				cat_string_wo_append_char(e_line, ' ');
			}

			CatStringWo *m2t = jag_byt_imnemonic_to_string(mnemonic, label_repo);
			if (m2t) {
				cat_string_wo_append_chars(e_line, "   ");
				while(cat_string_wo_length(e_line)<40) {
					cat_string_wo_append_char(e_line, ' ');
				}
				cat_string_wo_append(e_line, m2t);
				cat_unref_ptr(m2t);
			}

			cat_string_wo_append_char(e_line, '\n');
			buf.append(cat_string_wo_getchars(e_line));
			cat_unref_ptr(e_line);
		}

		cat_unref_ptr(mnemonic_block);

		buf.append("\n\n");

	}
	return strdup(buf.data());
}



JagByteCodePrinter *jag_byte_code_printer_new() {
	return (JagByteCodePrinter*) new ByteCodePrinter();
}

char *jag_byte_code_printer_print(JagByteCodePrinter *printer, JagBytClassfile *classfile) {
	ByteCodePrinter *p = (ByteCodePrinter *) printer;
	return p->print(classfile);
}

