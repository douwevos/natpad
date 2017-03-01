/*
   File:    jorscanner.h
   Project: jordanella
   Author:  Douwe Vos
   Date:    Feb 6, 2016
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

#ifndef EDITOR_JORSCANNER_H_
#define EDITOR_JORSCANNER_H_

#include "jortoken.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JOR_TYPE_SCANNER              (jor_scanner_get_type())
#define JOR_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_scanner_get_type(), JorScanner))
#define JOR_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_SCANNER, JorScannerClass))
#define JOR_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_SCANNER))
#define JOR_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_SCANNER))
#define JOR_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_SCANNER, JorScannerClass))


typedef struct _JorScanner               JorScanner;
typedef struct _JorScannerPrivate        JorScannerPrivate;
typedef struct _JorScannerClass          JorScannerClass;


struct _JorScanner {
	GObject parent;
};

struct _JorScannerClass {
	GObjectClass parent_class;
};


GType jor_scanner_get_type();

JorScanner *jor_scanner_new(CatIUtf8Scanner *scanner);

JorToken *jor_scanner_next(JorScanner *scanner);

G_END_DECLS

#endif /* EDITOR_JORSCANNER_H_ */
