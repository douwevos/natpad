/*
   File:    jagdecintermediatecatchgroup.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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


#ifndef JAGDECINTERMEDIATECATCHGROUP_H_
#define JAGDECINTERMEDIATECATCHGROUP_H_

#include "jagdecintermediategroup.h"
#include "../subcode/impl/jagdecsubcodecatch.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP              (jag_dec_intermediate_catch_group_get_type())
#define JAG_DEC_INTERMEDIATE_CATCH_GROUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_catch_group_get_type(), JagDecIntermediateCatchGroup))
#define JAG_DEC_INTERMEDIATE_CATCH_GROUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP, JagDecIntermediateCatchGroupClass))
#define JAG_DEC_IS_INTERMEDIATE_CATCH_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP))
#define JAG_DEC_IS_INTERMEDIATE_CATCH_GROUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP))
#define JAG_DEC_INTERMEDIATE_CATCH_GROUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP, JagDecIntermediateCatchGroupClass))
#define JAG_DEC_INTERMEDIATE_CATCH_GROUP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CATCH_GROUP, JagDecIntermediateCatchGroupPrivate))

typedef struct _JagDecIntermediateCatchGroup               JagDecIntermediateCatchGroup;
typedef struct _JagDecIntermediateCatchGroupPrivate        JagDecIntermediateCatchGroupPrivate;
typedef struct _JagDecIntermediateCatchGroupClass          JagDecIntermediateCatchGroupClass;


struct _JagDecIntermediateCatchGroup {
	JagDecIntermediateGroup parent;
	JagDecIntermediateCatchGroupPrivate *priv;
};

struct _JagDecIntermediateCatchGroupClass {
	JagDecIntermediateGroupClass parent_class;
};


GType jag_dec_intermediate_catch_group_get_type();

JagDecIntermediateCatchGroup *jag_dec_intermediate_catch_group_new(JagAstDeclarationType *exception_type);

JagAstDeclarationType *jag_dec_intermediate_catch_group_get_exception_type(JagDecIntermediateCatchGroup *catch_group);

JagDecSubCodeCatch *jag_dec_intermediate_catch_group_get_sub_code_catch(JagDecIntermediateCatchGroup *catch_group);

G_END_DECLS

#endif /* JAGDECINTERMEDIATECATCHGROUP_H_ */
