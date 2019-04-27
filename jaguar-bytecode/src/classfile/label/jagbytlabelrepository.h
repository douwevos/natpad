/*
   File:    jagbytlabelrepository.h
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

#ifndef JAGBYTLABELREPOSITORY_H_
#define JAGBYTLABELREPOSITORY_H_

#include "jagbytlabel.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_LABEL_REPOSITORY              (jag_byt_label_repository_get_type())
#define JAG_BYT_LABEL_REPOSITORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_label_repository_get_type(), JagBytLabelRepository))
#define JAG_BYT_LABEL_REPOSITORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_LABEL_REPOSITORY, JagBytLabelRepositoryClass))
#define JAG_BYT_IS_LABEL_REPOSITORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_LABEL_REPOSITORY))
#define JAG_BYT_IS_LABEL_REPOSITORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_LABEL_REPOSITORY))
#define JAG_BYT_LABEL_REPOSITORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_LABEL_REPOSITORY, JagBytLabelRepositoryClass))

typedef struct _JagBytLabelRepository               JagBytLabelRepository;
typedef struct _JagBytLabelRepositoryPrivate        JagBytLabelRepositoryPrivate;
typedef struct _JagBytLabelRepositoryClass          JagBytLabelRepositoryClass;


struct _JagBytLabelRepository {
	GObject parent;
};

struct _JagBytLabelRepositoryClass {
	GObjectClass parent_class;
};


GType jag_byt_label_repository_get_type();

JagBytLabelRepository *jag_byt_label_repository_new();


JagBytLabel *jag_byt_label_repository_get(JagBytLabelRepository *label_repository, int mnemonicIndex);

JagBytLabel *jag_byt_label_repository_create_soft(JagBytLabelRepository *label_repository, int mnemonicIndex, int bytecodeOffset);

JagBytLabel *jag_byt_label_repository_create_hard(JagBytLabelRepository *label_repository, int mnemonicIndex, int bytecodeOffset);

G_END_DECLS

#endif /* JAGBYTLABELREPOSITORY_H_ */
