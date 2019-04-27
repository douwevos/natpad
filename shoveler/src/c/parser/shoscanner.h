/*
   File:    shoscanner.h
   Project: shoveler
   Author:  Douwe Vos
   Date:    May 7, 2012
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

#ifndef SHOSCANNER_H_
#define SHOSCANNER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_SCANNER              (sho_scanner_get_type())
#define SHO_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_scanner_get_type(), ShoScanner))
#define SHO_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_SCANNER, ShoScannerClass))
#define SHO_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_SCANNER))
#define SHO_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_SCANNER))
#define SHO_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_SCANNER, ShoScannerClass))

typedef struct _ShoScanner               ShoScanner;
typedef struct _ShoScannerPrivate        ShoScannerPrivate;
typedef struct _ShoScannerClass          ShoScannerClass;


struct _ShoScanner {
	GObject parent;
};

struct _ShoScannerClass {
	GObjectClass parent_class;
};


GType sho_scanner_get_type();

ShoScanner *sho_scanner_new(CatIInputStream *stream_in);

G_END_DECLS

#endif /* SHOSCANNER_H_ */
