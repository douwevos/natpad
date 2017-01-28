/*
   File:    terscanner.h
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 19, 2012
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

#ifndef TERSCANNER_H_
#define TERSCANNER_H_

#include <caterpillar.h>
#include <dragonfly.h>

#include "tersyntax.h"

G_BEGIN_DECLS

#define TER_TYPE_SCANNER              (ter_scanner_get_type())
#define TER_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_scanner_get_type(), TerScanner))
#define TER_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_SCANNER, TerScannerClass))
#define TER_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_SCANNER))
#define TER_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_SCANNER))
#define TER_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_SCANNER, TerScannerClass))
#define TER_SCANNER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TER_TYPE_SCANNER, TerScannerPrivate))

typedef struct _TerScanner               TerScanner;
typedef struct _TerScannerPrivate        TerScannerPrivate;
typedef struct _TerScannerClass          TerScannerClass;


struct _TerScanner {
	GObject parent;
	TerScannerPrivate *priv;
};

struct _TerScannerClass {
	GObjectClass parent_class;
};


GType ter_scanner_get_type();

TerScanner *ter_scanner_new(TerSyntax *syntax, CatIUtf8Scanner *scanner, DraKeywordPrinter *token_printer);

void ter_scanner_run(TerScanner *lexer);

G_END_DECLS

#endif /* TERSCANNER_H_ */
