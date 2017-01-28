/*
   File:    jagdecintermediatetrycatchgroup.h
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


#ifndef JAGDECINTERMEDIATETRYCATCHGROUP_H_
#define JAGDECINTERMEDIATETRYCATCHGROUP_H_

#include "jagdecintermediategroup.h"
#include "jagdecintermediatecatchgroup.h"
#include "jagdecintermediatefinallygroup.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP              (jag_dec_intermediate_try_catch_group_get_type())
#define JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_try_catch_group_get_type(), JagDecIntermediateTryCatchGroup))
#define JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP, JagDecIntermediateTryCatchGroupClass))
#define JAG_DEC_IS_INTERMEDIATE_TRY_CATCH_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP))
#define JAG_DEC_IS_INTERMEDIATE_TRY_CATCH_GROUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP))
#define JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP, JagDecIntermediateTryCatchGroupClass))
#define JAG_DEC_INTERMEDIATE_TRY_CATCH_GROUP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_TRY_CATCH_GROUP, JagDecIntermediateTryCatchGroupPrivate))

typedef struct _JagDecIntermediateTryCatchGroup               JagDecIntermediateTryCatchGroup;
typedef struct _JagDecIntermediateTryCatchGroupPrivate        JagDecIntermediateTryCatchGroupPrivate;
typedef struct _JagDecIntermediateTryCatchGroupClass          JagDecIntermediateTryCatchGroupClass;


struct _JagDecIntermediateTryCatchGroup {
	JagDecIntermediateGroup parent;
	JagDecIntermediateTryCatchGroupPrivate *priv;
};

struct _JagDecIntermediateTryCatchGroupClass {
	JagDecIntermediateGroupClass parent_class;
};


GType jag_dec_intermediate_try_catch_group_get_type();

JagDecIntermediateTryCatchGroup *jag_dec_intermediate_try_catch_group_new(JagBytTryCatch *try_catch);

JagBytTryCatch *jag_dec_intermediate_try_catch_group_get_try_catch(JagDecIntermediateTryCatchGroup *try_catch_group);


void jag_dec_intermediate_try_catch_group_add_catch_group(JagDecIntermediateTryCatchGroup *try_catch_group, JagDecIntermediateCatchGroup *catch_group);
void jag_dec_intermediate_try_catch_group_add_finally_group(JagDecIntermediateTryCatchGroup *try_catch_group, JagDecIntermediateFinallyGroup *finally_group);

JagDecIntermediateCatchGroup *jag_dec_intermediate_try_catch_group_find_catch_group(JagDecIntermediateTryCatchGroup *try_catch_group, JagAstDeclarationType *exceptionType);

JagDecIntermediateFinallyGroup *jag_dec_intermediate_try_catch_group_get_finally_group(JagDecIntermediateTryCatchGroup *try_catch_group);

gboolean jag_dec_intermediate_try_catch_group_is_stale(JagDecIntermediateTryCatchGroup *try_catch_group);

void jag_dec_intermediate_try_catch_group_set_catch_blocks_resolved(JagDecIntermediateTryCatchGroup *try_catch_group, gboolean val);
gboolean jag_dec_intermediate_try_catch_group_are_catch_blocks_resolved(JagDecIntermediateTryCatchGroup *try_catch_group);

G_END_DECLS

#endif /* JAGDECINTERMEDIATETRYCATCHGROUP_H_ */
