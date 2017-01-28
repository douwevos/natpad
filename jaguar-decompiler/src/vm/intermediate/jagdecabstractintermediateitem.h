/*
   File:    jagdecabstractintermediateitem.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 9, 2012
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


#ifndef JAGDECABSTRACTINTERMEDIATEITEM_H_
#define JAGDECABSTRACTINTERMEDIATEITEM_H_

#include "jagdecintermediatenodescope.h"
#include "jagdeciintermediatenode.h"
#include "jagdecintermediateruncontext.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM              (jag_dec_abstract_intermediate_item_get_type())
#define JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_abstract_intermediate_item_get_type(), JagDecAbstractIntermediateItem))
#define JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, JagDecAbstractIntermediateItemClass))
#define JAG_DEC_IS_ABSTRACT_INTERMEDIATE_ITEM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM))
#define JAG_DEC_IS_ABSTRACT_INTERMEDIATE_ITEM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM))
#define JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, JagDecAbstractIntermediateItemClass))
#define JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, JagDecAbstractIntermediateItemPrivate))

typedef struct _JagDecAbstractIntermediateItem               JagDecAbstractIntermediateItem;
typedef struct _JagDecAbstractIntermediateItemPrivate        JagDecAbstractIntermediateItemPrivate;
typedef struct _JagDecAbstractIntermediateItemClass          JagDecAbstractIntermediateItemClass;


struct _JagDecAbstractIntermediateItem {
	GObject parent;
	JagDecAbstractIntermediateItemPrivate *priv;
};

struct _JagDecAbstractIntermediateItemClass {
	GObjectClass parent_class;
};


GType jag_dec_abstract_intermediate_item_get_type();

void jag_dec_abstract_intermediate_item_construct(JagDecAbstractIntermediateItem *item, int mnemonic_index);

int jag_dec_abstract_intermediate_item_get_mnemonic_index(JagDecAbstractIntermediateItem *item);


G_END_DECLS


#endif /* JAGDECABSTRACTINTERMEDIATEITEM_H_ */
