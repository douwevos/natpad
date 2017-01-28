/*
   File:    jagjarreaderut.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jan 27, 2012
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

#include <stdio.h>
//#include "../src/jar/jagjarreader.h"
#include <caterpillar.h>
#include <viper.h>
#ifdef G_OS_UNIX
#include <pwd.h>
#include <sys/stat.h>
#else
#include <windows.h>
#include <userenv.h>
#endif

//
//
//#ifdef G_OS_UNIX
//static int l_open_config_for_write(CatStringWo *a_path) {
//	int result = -1;
//#else
//static HANDLE l_open_config_for_write(CatStringWo *a_path) {
//	HANDLE result = NULL;
//#endif
//
//	const char *ucp_char = cat_string_wo_getchars(path);
//#ifdef G_OS_UNIX
//	result = open(ucp_char, O_CREAT|O_WRONLY|O_TRUNC, DEFFILEMODE);
//#else
//	result = CreateFile(ucp_char, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//#endif
//	return result;
//}
//
int main(int argc, char **argv) {
//	printf("starting argc=%d\n", argc);
//
//	CatStringWo *a_jar_path_txt = cat_string_wo_new_with(argv[1]);
//	printf("jar_path_txt=%s\n", cat_string_wo_getchars(jar_path_txt));
//	VipPath *jar_path = vip_path_new(cat_ref_sink_ptr(jar_path_txt));
//	cat_unref_ptr(jar_path_txt);
//	JagJarReader *jar_reader = jag_jar_reader_new(jar_path);
//	printf("jar_reader=%p\n", jar_reader);
//	if (jag_jar_reader_init_directory(jar_reader)) {
//		printf("init dir successfull\n");
//		CatIIterator *entry_iter = jag_jar_reader_entry_iterator(jar_reader);
//		printf("entry_iter=%p\n",entry_iter);
//		while(cat_iiterator_has_next(entry_iter)) {
//			JagJarCentralDirHeader *jar_entry = (JagJarCentralDirHeader *) cat_iiterator_next(entry_iter);
//			printf("jar_entry=%p\n",jar_entry);
//			CatStringWo *e_entry_chars = jag_jar_reader_read_entry(jar_reader, jar_entry, TRUE);
//
//
//			printf("entry_chars=%p\n",entry_chars);
//
//			CatStringWo *a_outpath = cat_string_wo_new_with("C:\\douwe\\");
//			CatStringWo *a_config_path = cat_string_wo_append(outpath, jar_entry->fileName);
//
//
//
//			CatFileOutputStream *file_stream = cat_file_output_stream_new(config_path);
//			file_stream->file_handle = l_open_config_for_write(config_path);
//			file_stream->state = STREAM_OPENED;
//			cat_file_output_stream_write_length(file_stream, cat_string_wo_getchars(e_entry_chars), cat_string_wo_length(e_entry_chars));
//			cat_file_output_stream_close(file_stream);
//
//			cat_unref_ptr(entry_chars);
//			cat_unref_ptr(file_stream);
//
//
//		}
//		cat_unref_ptr(entry_iter);
//
//	} else {
//		printf("could not init directory");
//	}
//
//	printf("done\n");
	return 0;
}
