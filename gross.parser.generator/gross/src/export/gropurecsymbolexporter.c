//
///*
//   File:    gropurecsymbolexporter.c
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jun 8, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "gropurecsymbolexporter.h"
//#include <gio/gio.h>
//
//void gro_pure_c_export_symbols(GroModel *model, GroStateMachine *state_machine, GFile *output_path, GroClassName *cname) {
//
//	CatStringWo *symhname = cat_string_wo_new();
//	cat_string_wo_format(symhname, "%O%O.h", cname->prefix_lower, cname->name_lower);
//
//	GFile *file = g_file_get_child(output_path, cat_string_wo_getchars(symhname));
//
//	GFileOutputStream *out_stream = g_file_replace(file, NULL, FALSE, 0, NULL, NULL);
//	if (out_stream) {
//
//		CatStringWo *buf = cat_string_wo_new();
//
//		cat_string_wo_format(buf, "#ifndef %O%O_H\n", cname->prefix_up, cname->name_up);
//		cat_string_wo_format(buf, "#define %O%O_H\n", cname->prefix_up, cname->name_up);
//		cat_string_wo_format(buf, "\n");
//		cat_string_wo_format(buf, "/* terminals */\n");
//
//		CatArrayWo *terminals = gro_model_enlist_terminals(model);
//		CatIIterator *term_iter = cat_array_wo_iterator(terminals);
//
//		while(cat_iiterator_has_next(term_iter)) {
//			GroMSymbol *symbol = (GroMSymbol *) cat_iiterator_next(term_iter);
//			CatStringWo *name = gro_m_symbol_get_name(symbol);
//			name = cat_string_wo_clone(name, CAT_CLONE_DEPTH_FULL);
//			cat_string_wo_to_uppercase(name);
//			CatStringWo *sl = cat_string_wo_new();
//			cat_string_wo_format(sl, "#define %O_%O_TERM_%O", cname->prefix_up, cname->name_up, name);
//			while(cat_string_wo_length(sl) < 50) {
//				cat_string_wo_append_char(sl, ' ');
//			}
//			cat_string_wo_format(sl, "%d\n", gro_m_symbol_get_index(symbol));
//			cat_string_wo_append(buf, sl);
//			cat_unref_ptr(sl);
//			cat_unref_ptr(name);
//		}
//
//		cat_unref_ptr(term_iter);
//		cat_string_wo_format(buf, "#define %O_%O_TERMINAL_COUNT %d\n", cname->prefix_up, cname->name_up, cat_array_wo_size(terminals));
//
//		cat_string_wo_format(buf, "\n");
//		cat_string_wo_format(buf, "/* non terminals */\n");
//		CatArrayWo *non_terminals = gro_model_enlist_non_terminals(model);
//		CatIIterator *nterm_iter = cat_array_wo_iterator(non_terminals);
//		while(cat_iiterator_has_next(nterm_iter)) {
//			GroMSymbol *symbol = (GroMSymbol *) cat_iiterator_next(nterm_iter);
//			CatStringWo *name = gro_m_symbol_get_name((GroMSymbol *) symbol);
//			name = cat_string_wo_clone(name, CAT_CLONE_DEPTH_FULL);
//			cat_string_wo_to_uppercase(name);
//			CatStringWo *sl = cat_string_wo_new();
//			cat_string_wo_format(sl, "#define %O_%O_NONTERM_%O", cname->prefix_up, cname->name_up, name);
//			while(cat_string_wo_length(sl) < 65) {
//				cat_string_wo_append_char(sl, ' ');
//			}
//			cat_string_wo_format(sl, "%d\n", gro_m_symbol_get_index((GroMSymbol *) symbol));
//			cat_string_wo_append(buf, sl);
//			cat_unref_ptr(sl);
//			cat_unref_ptr(name);
//		}
//		cat_unref_ptr(nterm_iter);
//
//
//		cat_string_wo_format(buf, "#endif /* %O%O_H */\n", cname->prefix_up, cname->name_up);
//
//		const char *data = cat_string_wo_getchars(buf);
//		int data_len = cat_string_wo_length(buf);
//
//		g_output_stream_write((GOutputStream *) out_stream, data, data_len, NULL, NULL);
//		g_output_stream_close((GOutputStream *) out_stream, NULL, NULL);
//	}
//}
