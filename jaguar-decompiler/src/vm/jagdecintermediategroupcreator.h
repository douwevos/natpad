/*
   File:    jagdecintermediategroupcreator.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 5, 2012
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


#ifndef JAGDECINTERMEDIATEGROUPCREATOR_H_
#define JAGDECINTERMEDIATEGROUPCREATOR_H_

#include "../type/jagdecimportrepository.h"
#include "intermediate/jagdecintermediategroup.h"
#include <jaguarbytecode.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR              (jag_dec_intermediate_group_creator_get_type())
#define JAG_DEC_INTERMEDIATE_GROUP_CREATOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_group_creator_get_type(), JagDecIntermediateGroupCreator))
#define JAG_DEC_INTERMEDIATE_GROUP_CREATOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR, JagDecIntermediateGroupCreatorClass))
#define JAG_DEC_IS_INTERMEDIATE_GROUP_CREATOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR))
#define JAG_DEC_IS_INTERMEDIATE_GROUP_CREATOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR))
#define JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR, JagDecIntermediateGroupCreatorClass))
#define JAG_DEC_INTERMEDIATE_GROUP_CREATOR_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_GROUP_CREATOR, JagDecIntermediateGroupCreatorPrivate))

typedef struct _JagDecIntermediateGroupCreator               JagDecIntermediateGroupCreator;
typedef struct _JagDecIntermediateGroupCreatorPrivate        JagDecIntermediateGroupCreatorPrivate;
typedef struct _JagDecIntermediateGroupCreatorClass          JagDecIntermediateGroupCreatorClass;


struct _JagDecIntermediateGroupCreator {
	GObject parent;
	JagDecIntermediateGroupCreatorPrivate *priv;
};

struct _JagDecIntermediateGroupCreatorClass {
	GObjectClass parent_class;
};


GType jag_dec_intermediate_group_creator_get_type();

JagDecIntermediateGroupCreator *jag_dec_intermediate_group_creator_new(JagDecImportRepository *import_repository, JagBytClassfile *type_definition, JagBytMethod *method, JagBytMnemonicBlock *mnemonic_block, JagBytTryCatchList *try_catch_list);

JagDecIntermediateGroup *jag_dec_intermediate_group_creator_get_main_container(JagDecIntermediateGroupCreator *group_creator);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEGROUPCREATOR_H_ */
