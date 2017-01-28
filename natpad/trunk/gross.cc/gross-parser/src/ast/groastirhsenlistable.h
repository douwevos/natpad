/*
   File:    groastirhsenlistable.h
   Project: gross-parser
   Author:  Douwe Vos
   Date:    Nov 28, 2016
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

#ifndef AST_GROASTIRHSENLISTABLE_H_
#define AST_GROASTIRHSENLISTABLE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GROAST_TYPE_IRHS_ENLISTABLE                 (groast_irhs_enlistable_get_type())
#define GROAST_IRHS_ENLISTABLE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GROAST_TYPE_IRHS_ENLISTABLE, GroAstIRhsEnlistable))
#define GROAST_IS_IRHS_ENLISTABLE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GROAST_TYPE_IRHS_ENLISTABLE))
#define GROAST_IRHS_ENLISTABLE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GROAST_TYPE_IRHS_ENLISTABLE, GroAstIRhsEnlistableInterface))


typedef struct _GroAstIRhsEnlistable               GroAstIRhsEnlistable; /* dummy object */
typedef struct _GroAstIRhsEnlistableInterface      GroAstIRhsEnlistableInterface;

typedef CatArrayWo *(*GroAstEnlistFunc)(GroAstIRhsEnlistable *self);


struct _GroAstIRhsEnlistableInterface {
	GTypeInterface parent_iface;
	CatArrayWo *(*enlist)(GroAstIRhsEnlistable *self);
};

GType groast_irhs_enlistable_get_type(void);

CatArrayWo *groast_irhs_enlistable_enlist(GroAstIRhsEnlistable *enlistable);

G_END_DECLS

#endif /* AST_GROASTIRHSENLISTABLE_H_ */
