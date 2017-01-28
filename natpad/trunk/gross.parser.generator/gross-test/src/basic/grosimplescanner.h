
/*
   File:    grosimplescanner.h
   Project: gross-test
   Author:  Douwe Vos
   Date:    Jun 27, 2016
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

#ifndef GROSIMPLESCANNER_H_
#define GROSIMPLESCANNER_H_

#include <grossglibcruntime.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_SIMPLE_SCANNER              (gro_simple_scanner_get_type())
#define GRO_SIMPLE_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_simple_scanner_get_type(), GroSimpleScanner))
#define GRO_SIMPLE_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_SIMPLE_SCANNER, GroSimpleScannerClass))
#define GRO_IS_SIMPLE_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_SIMPLE_SCANNER))
#define GRO_IS_SIMPLE_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_SIMPLE_SCANNER))
#define GRO_SIMPLE_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_SIMPLE_SCANNER, GroSimpleScannerClass))


typedef struct _GroSimpleScanner               GroSimpleScanner;
typedef struct _GroSimpleScannerPrivate        GroSimpleScannerPrivate;
typedef struct _GroSimpleScannerClass          GroSimpleScannerClass;


struct _GroSimpleScanner {
	GObject parent;
};

struct _GroSimpleScannerClass {
	GObjectClass parent_class;
};


GType gro_simple_scanner_get_type();

GroSimpleScanner *gro_simple_scanner_new(CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* GROSIMPLESCANNER_H_ */
