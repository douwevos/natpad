/*
   File:    groscanner.h
   Project: gross
   Author:  Douwe Vos
   Date:    Max 22, 2016
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

#ifndef GROSCANNER_H_
#define GROSCANNER_H_

#include <caterpillar.h>
//#include "generated/runtime/grotoken.h"

#include <grossglibcruntime.h>


G_BEGIN_DECLS

#define GRO_TYPE_SCANNER            (gro_scanner_get_type())
#define GRO_SCANNER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_scanner_get_type(), GroScanner))
#define GRO_SCANNER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_SCANNER, GroScannerClass))
#define GRO_IS_SCANNER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_SCANNER))
#define GRO_IS_SCANNER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_SCANNER))
#define GRO_SCANNER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_SCANNER, GroScannerClass))

typedef struct _GroScanner       GroScanner;
typedef struct _GroScannerClass  GroScannerClass;


struct _GroScanner {
	GObject parent;
	GroToken *buffered_token;
	CatIUtf8Scanner *utf8Scanner;
	CatStreamStatus streamStatus;
	gunichar lastChar;
	CatUnicharArray *buffer;
	int row, column, nextColumn, mark, mark_row;
	gboolean inFullComment;
	gboolean filter_non_parsable;

	gboolean create_ast_tokens;

};


struct _GroScannerClass {
	GObjectClass parent_class;
	CatHashMapWo *a_keywordMap;
};


GType gro_scanner_get_type(void);

GroScanner *gro_scanner_new(CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* GROSCANNER_H_ */
