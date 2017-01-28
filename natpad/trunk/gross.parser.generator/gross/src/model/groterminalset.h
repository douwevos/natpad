
/*
   File:    groterminalset.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 31, 2016
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

#ifndef PARSER_MODEL_GROTERMINALSET_H_
#define PARSER_MODEL_GROTERMINALSET_H_

#include "gromterminal.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_TERMINAL_SET              (gro_terminal_set_get_type())
#define GRO_TERMINAL_SET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_terminal_set_get_type(), GroTerminalSet))
#define GRO_TERMINAL_SET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_TERMINAL_SET, GroTerminalSetClass))
#define GRO_IS_TERMINAL_SET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_TERMINAL_SET))
#define GRO_IS_TERMINAL_SET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_TERMINAL_SET))
#define GRO_TERMINAL_SET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_TERMINAL_SET, GroTerminalSetClass))


typedef struct _GroTerminalSet               GroTerminalSet;
typedef struct _GroTerminalSetPrivate        GroTerminalSetPrivate;
typedef struct _GroTerminalSetClass          GroTerminalSetClass;


struct _GroTerminalSet {
	GObject parent;
};

struct _GroTerminalSetClass {
	GObjectClass parent_class;
};


GType gro_terminal_set_get_type();

GroTerminalSet *gro_terminal_set_new();
GroTerminalSet *gro_terminal_set_clone(const GroTerminalSet *src);

gboolean gro_terminal_set_add(GroTerminalSet *terminal_set, GroMTerminal *terminal);

gboolean gro_terminal_set_add_all(GroTerminalSet *terminal_set, GroTerminalSet *to_add);

gboolean gro_terminal_set_is_empty(GroTerminalSet *terminal_set);
gboolean gro_terminal_set_contains(const GroTerminalSet *terminal_set, GroMTerminal *terminal);
GroMTerminalIterator *gro_terminal_set_iterator(GroTerminalSet *terminal_set);

G_END_DECLS

#endif /* PARSER_MODEL_GROTERMINALSET_H_ */
