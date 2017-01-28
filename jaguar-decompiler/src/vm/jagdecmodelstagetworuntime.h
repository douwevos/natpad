/*
   File:    jagdecmodelstagetworuntime.h
   Project: jaguar-decompiler
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


#ifndef JAGDECMODELSTAGETWORUNTIME_H_
#define JAGDECMODELSTAGETWORUNTIME_H_

#include "jagdecmnemoniclinenumbertable.h"
#include "subcode/jagdecisubcode.h"
#include "../type/jagdecimportrepository.h"
#include <jaguarbytecode.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME              (jag_dec_model_stage_two_runtime_get_type())
#define JAG_DEC_MODEL_STAGE_TWO_RUNTIME(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_model_stage_two_runtime_get_type(), JagDecModelStageTwoRuntime))
#define JAG_DEC_MODEL_STAGE_TWO_RUNTIME_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME, JagDecModelStageTwoRuntimeClass))
#define JAG_DEC_IS_MODEL_STAGE_TWO_RUNTIME(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME))
#define JAG_DEC_IS_MODEL_STAGE_TWO_RUNTIME_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME))
#define JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME, JagDecModelStageTwoRuntimeClass))
#define JAG_DEC_MODEL_STAGE_TWO_RUNTIME_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_MODEL_STAGE_TWO_RUNTIME, JagDecModelStageTwoRuntimePrivate))

typedef struct _JagDecModelStageTwoRuntime               JagDecModelStageTwoRuntime;
typedef struct _JagDecModelStageTwoRuntimePrivate        JagDecModelStageTwoRuntimePrivate;
typedef struct _JagDecModelStageTwoRuntimeClass          JagDecModelStageTwoRuntimeClass;


struct _JagDecModelStageTwoRuntime {
	GObject parent;
	JagDecModelStageTwoRuntimePrivate *priv;
};

struct _JagDecModelStageTwoRuntimeClass {
	GObjectClass parent_class;
};


GType jag_dec_model_stage_two_runtime_get_type();

JagDecModelStageTwoRuntime *jag_dec_model_stage_two_runtime_new(JagDecISubCode *sub_code_block, JagDecImportRepository *import_repository, JagDecMnemonicLineNumberTable *line_number_table);

JagDecModelStageTwoRuntime *jag_dec_model_stage_two_runtime_create_branch(JagDecModelStageTwoRuntime *runtime, JagAstBlock *new_current_block);

JagDecImportRepository *jag_dec_model_stage_two_runtime_get_import_repository(JagDecModelStageTwoRuntime *runtime);

void jag_dec_model_stage_two_runtime_append_statement(JagDecModelStageTwoRuntime *runtime, JagAstIStatement *statement);

void jag_dec_model_stage_two_runtime_set_mnemonic_index(JagDecModelStageTwoRuntime *runtime, int mnemonic_index);

JagAstBlock *jag_dec_model_stage_two_runtime_connect(JagDecModelStageTwoRuntime *runtime);

JagAstMethodHeader *jag_dec_model_stage_two_runtime_create_final_method_header(JagDecModelStageTwoRuntime *runtime, JagBytMethod *method);

JagAstIdentifier *jag_dec_model_stage_two_runtime_create_plain_name(JagDecModelStageTwoRuntime *runtime, JagAstDeclarationType *declarationType, JagAstIdentifier *fieldSourceClass);



G_END_DECLS


#endif /* JAGDECMODELSTAGETWORUNTIME_H_ */
