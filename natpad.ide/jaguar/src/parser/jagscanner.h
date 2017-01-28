/*
   File:    jagscanner.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 16, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef JAGSCANNER_H_
#define JAGSCANNER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_SCANNER            (jag_scanner_get_type())
#define JAG_SCANNER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_scanner_get_type(), JagScanner))
#define JAG_SCANNER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SCANNER, JagScannerClass))
#define JAG_IS_SCANNER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SCANNER))
#define JAG_IS_SCANNER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SCANNER))
#define JAG_SCANNER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SCANNER, JagScannerClass))

typedef struct _JagScanner       JagScanner;
typedef struct _JagScannerClass  JagScannerClass;


struct _JagScanner {
	GObject parent;
	CatIUtf8Scanner *utf8Scanner;
	CatStreamStatus streamStatus;
	gunichar lastChar;
	CatUnicharArray *buffer;
	int row, column, nextColumn, mark, mark_row;
	gboolean inFullComment;
	gboolean filter_non_parsable;

	gboolean create_ast_tokens;

#ifdef NATPAD_TESTING
	CatHashMapWo *a_keyword_map;
#endif

};


struct _JagScannerClass {
	GObjectClass parent_class;
#ifndef NATPAD_TESTING
	CatHashMapWo *a_keywordMap;
#endif
};


GType jag_scanner_get_type(void);

JagScanner *jag_scanner_new(CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* JAGSCANNER_H_ */
