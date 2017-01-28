/*
   File:    drauowindent.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 14, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef UOW_DRAUOWINDENT_H_
#define UOW_DRAUOWINDENT_H_

#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_UOW_INDENT              (dra_uow_indent_get_type())
#define DRA_UOW_INDENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_uow_indent_get_type(), DraUowIndent))
#define DRA_UOW_INDENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_UOW_INDENT, DraUowIndentClass))
#define DRA_IS_UOW_INDENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_UOW_INDENT))
#define DRA_IS_UOW_INDENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_UOW_INDENT))
#define DRA_UOW_INDENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_UOW_INDENT, DraUowIndentClass))


typedef struct _DraUowIndent               DraUowIndent;
typedef struct _DraUowIndentPrivate        DraUowIndentPrivate;
typedef struct _DraUowIndentClass          DraUowIndentClass;


struct _DraUowIndent {
	ChaUow parent;
};

struct _DraUowIndentClass {
	ChaUowClass parent_class;
};


GType dra_uow_indent_get_type();

DraUowIndent *dra_uow_indent_new();

G_END_DECLS

#endif /* UOW_DRAUOWINDENT_H_ */
