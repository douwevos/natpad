/*
   File:    jagbytecodereaderut.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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

#include <glib-object.h>
#include <stdlib.h>
#include <stdio.h>
#include <caterpillar.h>
#include "../src/jagbytecodereader.h"

int main(int argc, char **argv) {
	printf("starting jaguar-bytecode-test\n");

//	CatFileInputStream *istream = cat_file_input_stream_new(cat_string_wo_new_with("/home/superman/work/Long.class"));
	CatFileInputStream *istream = cat_file_input_stream_new(cat_string_wo_new_with("/home/superman/work/cpp-workspace/natpad/natpad.cup/build/classes/net/natpad/cup/TerminalSet.class"));

	JagBytecodeReader *bytecode_reader = jag_bytecode_reader_new(CAT_IINPUT_STREAM(istream));

	JagBytClassfile *classfile = jag_bytecode_reader_parse_bytecode(bytecode_reader);

	CatStringWo *a_fqname = jag_byt_classfile_get_fq_classname(classfile);
	printf("fqname=%s\n", cat_string_wo_getchars(a_fqname));

//	CatStringWo *e_source = jag_byt_classfile_as_source(classfile);
//	printf("%s", cat_string_wo_getchars(e_source));


	cat_unref_ptr(classfile);

	cat_unref_ptr(bytecode_reader);

//	cat_ref_entry_report();
	printf("done\n");
	return 0;
}
