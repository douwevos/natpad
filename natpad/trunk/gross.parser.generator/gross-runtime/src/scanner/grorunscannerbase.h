/*
   File:    grorunscannerbase.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Nov 11, 2016
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

#ifndef SCANNER_GRORUNSCANNERBASE_H_
#define SCANNER_GRORUNSCANNERBASE_H_

#include "../grorunitokenfactory.h"
#include "../grorunitoken.h"
#include "../grorunsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_SCANNER_BASE              (grorun_scanner_base_get_type())
#define GRORUN_SCANNER_BASE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_scanner_base_get_type(), GroRunScannerBase))
#define GRORUN_SCANNER_BASE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_SCANNER_BASE, GroRunScannerBaseClass))
#define GRORUN_IS_SCANNER_BASE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_SCANNER_BASE))
#define GRORUN_IS_SCANNER_BASE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_SCANNER_BASE))
#define GRORUN_SCANNER_BASE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_SCANNER_BASE, GroRunScannerBaseClass))


typedef struct _GroRunScannerBase               GroRunScannerBase;
typedef struct _GroRunScannerBasePrivate        GroRunScannerBasePrivate;
typedef struct _GroRunScannerBaseClass          GroRunScannerBaseClass;

#define GRORUN_ADVANCE_STRIP_LINE_BREAKS   1
#define GRORUN_ADVANCE_STRIP_WHITE_SPACES  2

struct _GroRunScannerBase {
	GObject parent;
};

struct _GroRunScannerBaseClass {
	GObjectClass parent_class;
	gunichar *(*getLookaheadBuffer)(GroRunScannerBase *self);
	GroRunSymbol *(*getConnectedSymbol)(GroRunScannerBase *self, int sym_conn_id);
	gunichar (*advance)(GroRunScannerBase *self, int advance_flags);
	GroRunIToken *(*createToken)(GroRunScannerBase *scanner, int conn_sym_id, long long end_row, int end_column, void *c_value);
	GroRunIToken *(*createTokenBasic)(GroRunScannerBase *scanner, int conn_sym_id, int nr_of_characters);

	GroRunIToken *(*scanForIdOrKeyword)(GroRunScannerBase *scanner, int conn_sym_id_for_identifier, gboolean case_sensitive);
	GroRunIToken *(*scanForEndOfLine)(GroRunScannerBase *scanner, int conn_sym_id_for_eol, gboolean get_text);
	GroRunIToken *(*scanForQuotedString)(GroRunScannerBase *scanner, int conn_sym_id_for_string);


	void (*markLocation)(GroRunScannerBase *scanner);
	int (*getColumn)(GroRunScannerBase *scanner);
	long long (*getRow)(GroRunScannerBase *scanner);
	int (*getMarkedColumn)(GroRunScannerBase *scanner);
	long long (*getMarkedRow)(GroRunScannerBase *scanner);
};


GType grorun_scanner_base_get_type();

void grorun_scanner_base_construct(GroRunScannerBase *scanner_base, GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols, int lookahead_max);

G_END_DECLS

#endif /* SCANNER_GRORUNSCANNERBASE_H_ */
