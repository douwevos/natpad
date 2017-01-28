/*
   File:    jagbytlabel.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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


#ifndef JAGBYTLABEL_H_
#define JAGBYTLABEL_H_

#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum _JagBytLabelType JagBytLabelType;

enum _JagBytLabelType {
	JAG_BYT_LABEL_TYPE_HARD,
	JAG_BYT_LABEL_TYPE_SOFT
};

#define JAG_BYT_TYPE_LABEL              (jag_byt_label_get_type())
#define JAG_BYT_LABEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_label_get_type(), JagBytLabel))
#define JAG_BYT_LABEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_LABEL, JagBytLabelClass))
#define JAG_BYT_IS_LABEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_LABEL))
#define JAG_BYT_IS_LABEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_LABEL))
#define JAG_BYT_LABEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_LABEL, JagBytLabelClass))
#define JAG_BYT_LABEL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_LABEL, JagBytLabelPrivate))

typedef struct _JagBytLabel               JagBytLabel;
typedef struct _JagBytLabelPrivate        JagBytLabelPrivate;
typedef struct _JagBytLabelClass          JagBytLabelClass;


struct _JagBytLabel {
	GObject parent;
	JagBytLabelPrivate *priv;
};

struct _JagBytLabelClass {
	GObjectClass parent_class;
};


GType jag_byt_label_get_type();

JagBytLabel *jag_byt_label_new(JagBytLabelType type, int mnemonicIndex, int bytecodeOffset, CatStringWo *a_name);


void jag_byt_label_set_name(JagBytLabel *label, CatStringWo *a_newName);
CatStringWo *jag_byt_label_get_name(JagBytLabel *label);

void jag_byt_label_set_label_type(JagBytLabel *label, JagBytLabelType type);
JagBytLabelType jag_byt_label_get_label_type(JagBytLabel *label);

gboolean jag_byt_label_equal(JagBytLabel *label, JagBytLabel *other);
int jag_byt_label_hash(JagBytLabel *label);

G_END_DECLS

#endif /* JAGBYTLABEL_H_ */
