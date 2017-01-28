/*
   File:    elkarguments.h
   Project: elk
   Author:  Douwe Vos
   Date:    Dec 25, 2010
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

#ifndef ELKARGUMENTS_H_
#define ELKARGUMENTS_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_ARGUMENTS            (elk_arguments_get_type())
#define ELK_ARGUMENTS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_arguments_get_type(), ElkArguments))
#define ELK_ARGUMENTS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ARGUMENTS, ElkArgumentsClass))
#define ELK_IS_ARGUMENTS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ARGUMENTS))
#define ELK_IS_ARGUMENTS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ARGUMENTS))
#define ELK_ARGUMENTS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ARGUMENTS, ElkArgumentsClass))

typedef struct _ElkArguments       ElkArguments;
typedef struct _ElkArgumentsClass  ElkArgumentsClass;


struct _ElkArguments {
	GObject parent;
	CatArrayWo *documents_to_load;
};


struct _ElkArgumentsClass {
	GObjectClass parent_class;
};


GType elk_arguments_get_type(void);

ElkArguments *elk_arguments_new();

void elk_arguments_parse_and_add_document(ElkArguments *arguments, CatStringWo *a_arg);

G_END_DECLS

#endif /* ELKARGUMENTS_H_ */
