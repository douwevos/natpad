/*
   File:    xanscanner.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 11, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef EDITOR_XANSCANNER_H_
#define EDITOR_XANSCANNER_H_

#include <caterpillar.h>
#include <grossruntime.h>


#define XAN_SYM_END_OF_INPUT              0
#define XAN_SYM_ELEMENT_OPEN              1
#define XAN_SYM_ELEMENT_CLOSE             2
#define XAN_SYM_LITERAL                   3
#define XAN_SYM_IS                        4
#define XAN_SYM_ID                        5



#define XAN_SYM_TEXT                    6
// <!-- nnn -->
#define XAN_SYM_COMMENT                 7
// </
#define XAN_SYM_TAG_OPEN_TERMINATOR		8
// <
#define XAN_SYM_TAG_OPEN				9
#define XAN_SYM_TAG						10

#define XAN_SYM_NAME					11

// >
#define XAN_SYM_TAG_CLOSE				12
// />
#define XAN_SYM_TAG_TERMINATOR_CLOSE	13
// :
#define XAN_SYM_COLON					14

G_BEGIN_DECLS

#define XAN_TYPE_SCANNER              (xan_scanner_get_type())
#define XAN_SCANNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_scanner_get_type(), XanScanner))
#define XAN_SCANNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_SCANNER, XanScannerClass))
#define XAN_IS_SCANNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_SCANNER))
#define XAN_IS_SCANNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_SCANNER))
#define XAN_SCANNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_SCANNER, XanScannerClass))


typedef struct _XanScanner               XanScanner;
typedef struct _XanScannerPrivate        XanScannerPrivate;
typedef struct _XanScannerClass          XanScannerClass;


struct _XanScanner {
	GroRunScannerBase parent;
};

struct _XanScannerClass {
	GroRunScannerBaseClass parent_class;
};


GType xan_scanner_get_type();

XanScanner *xan_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *input, CatArrayWo *connected_symbols);

GroRunIToken *xan_scanner_next(XanScanner *scanner);

G_END_DECLS

#endif /* EDITOR_XANSCANNER_H_ */
