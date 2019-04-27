/*
   File:    shoblockvalue.h
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
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

#ifndef SHOBLOCKVALUE_H_
#define SHOBLOCKVALUE_H_

#include "shoreference.h"
#include "shoreferencelist.h"
#include "shotemplatecall.h"
#include "shosimplevalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_BLOCK_VALUE              (sho_block_value_get_type())
#define SHO_BLOCK_VALUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_block_value_get_type(), ShoBlockValue))
#define SHO_BLOCK_VALUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_BLOCK_VALUE, ShoBlockValueClass))
#define SHO_IS_BLOCK_VALUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_BLOCK_VALUE))
#define SHO_IS_BLOCK_VALUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_BLOCK_VALUE))
#define SHO_BLOCK_VALUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_BLOCK_VALUE, ShoBlockValueClass))


typedef struct _ShoBlockValue               ShoBlockValue;
typedef struct _ShoBlockValuePrivate        ShoBlockValuePrivate;
typedef struct _ShoBlockValueClass          ShoBlockValueClass;


struct _ShoBlockValue {
	ShoReferenceList parent;
};

struct _ShoBlockValueClass {
	ShoReferenceListClass parent_class;
};


GType sho_block_value_get_type();

ShoBlockValue *sho_block_value_new();

void sho_block_value_set_template_call(ShoBlockValue *block, ShoTemplateCall *template_call);

G_END_DECLS

#endif /* SHOBLOCKVALUE_H_ */
