/*
   File:    jagdeccombineitem.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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


#ifndef JAGDECCOMBINEITEM_H_
#define JAGDECCOMBINEITEM_H_

#include "intermediate/jagdeciintermediatenode.h"
#include "intermediate/jagdecintermediategroup.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_COMBINE_ITEM              (jag_dec_combine_item_get_type())
#define JAG_DEC_COMBINE_ITEM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_combine_item_get_type(), JagDecCombineItem))
#define JAG_DEC_COMBINE_ITEM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_COMBINE_ITEM, JagDecCombineItemClass))
#define JAG_DEC_IS_COMBINE_ITEM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_COMBINE_ITEM))
#define JAG_DEC_IS_COMBINE_ITEM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_COMBINE_ITEM))
#define JAG_DEC_COMBINE_ITEM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_COMBINE_ITEM, JagDecCombineItemClass))

typedef struct _JagDecCombineItem               JagDecCombineItem;
typedef struct _JagDecCombineItemClass          JagDecCombineItemClass;


struct _JagDecCombineItem {
	GObject parent;
	JagDecIntermediateGroup *groupOne;
	JagDecIntermediateGroup *groupTwo;
	int lastIndex;
	JagDecIntermediateGroup *output;
	gboolean doBreak;

};

struct _JagDecCombineItemClass {
	GObjectClass parent_class;
};


GType jag_dec_combine_item_get_type();

JagDecCombineItem *jag_dec_combine_item_new();


void jag_dec_combine_item_prepend_combined(JagDecCombineItem *combine_item, JagDecIIntermediateNode *nodeOne, JagDecIIntermediateNode *nodeTwo);

void jag_dec_combine_item_prepend(JagDecCombineItem *combine_item, JagDecIntermediateGroup *group);


G_END_DECLS



#endif /* JAGDECCOMBINEITEM_H_ */
