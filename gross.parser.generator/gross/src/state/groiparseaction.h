
/*
   File:    groiparseaction.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 5, 2016
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

#ifndef PARSER_STATE_GROIPARSEACTION_H_
#define PARSER_STATE_GROIPARSEACTION_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_IPARSE_ACTION                 (gro_iparse_action_get_type ())
#define GRO_IPARSE_ACTION(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRO_TYPE_IPARSE_ACTION, GroIParseAction))
#define GRO_IS_IPARSE_ACTION(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_IPARSE_ACTION))
#define GRO_IPARSE_ACTION_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GRO_TYPE_IPARSE_ACTION, GroIParseActionInterface))

enum _GroActionKind {
	GRO_ACTION_KIND_ERROR,
	GRO_ACTION_KIND_NONASSOC,
	GRO_ACTION_KIND_SHIFT,
	GRO_ACTION_KIND_REDUCE,
	GRO_ACTION_KIND_FORK
};

typedef enum _GroActionKind GroActionKind;

typedef struct _GroIParseAction               GroIParseAction;
typedef struct _GroIParseActionInterface      GroIParseActionInterface;

struct _GroIParseActionInterface {
  GTypeInterface parent_iface;
  GroActionKind (*kind)(GroIParseAction *action);
};

GType gro_iparse_action_get_type (void);

GroActionKind gro_iparse_action_kind(GroIParseAction *action);

G_END_DECLS

#endif /* PARSER_STATE_GROIPARSEACTION_H_ */
