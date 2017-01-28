/*
   File:    jagdecintermediateruncontext.h
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


#ifndef JAGDECINTERMEDIATERUNCONTEXT_H_
#define JAGDECINTERMEDIATERUNCONTEXT_H_

#include "../../type/jagdecvaluetypedefinitionlist.h"
#include "../../type/jagdecimportrepository.h"
#include "../subcode/jagdecisubcode.h"
#include "../subcode/jagdeciconsumer.h"
#include "../subcode/jagdecivalueproducer.h"
#include "../subcode/block/jagdecsubcodeblock.h"
#include "../subcode/impl/jagdecmethodframevalue.h"
#include "jagdecintermediategroup.h"
#include "jagdecipostimport.h"
#include <jaguarbytecode.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT              (jag_dec_intermediate_run_context_get_type())
#define JAG_DEC_INTERMEDIATE_RUN_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_run_context_get_type(), JagDecIntermediateRunContext))
#define JAG_DEC_INTERMEDIATE_RUN_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT, JagDecIntermediateRunContextClass))
#define JAG_DEC_IS_INTERMEDIATE_RUN_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT))
#define JAG_DEC_IS_INTERMEDIATE_RUN_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT))
#define JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT, JagDecIntermediateRunContextClass))
#define JAG_DEC_INTERMEDIATE_RUN_CONTEXT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_RUN_CONTEXT, JagDecIntermediateRunContextPrivate))

typedef struct _JagDecIntermediateRunContext               JagDecIntermediateRunContext;
typedef struct _JagDecIntermediateRunContextPrivate        JagDecIntermediateRunContextPrivate;
typedef struct _JagDecIntermediateRunContextClass          JagDecIntermediateRunContextClass;


struct _JagDecIntermediateRunContext {
	GObject parent;
	JagDecIntermediateRunContextPrivate *priv;
};

struct _JagDecIntermediateRunContextClass {
	GObjectClass parent_class;
};


GType jag_dec_intermediate_run_context_get_type();

JagDecIntermediateRunContext *jag_dec_intermediate_run_context_new(JagBytIMnemonicBlock *mnemonicBlock, JagBytLocalVariableTable *localVariableTable);

JagBytLocalVariableTable *jag_dec_intermediate_run_context_get_local_variable_table(JagDecIntermediateRunContext *run_context);

JagBytIMnemonicBlock *jag_dec_intermediate_run_context_get_mnemonic_block(JagDecIntermediateRunContext *run_context);

JagDecSubCodeBlock *jag_dec_intermediate_run_context_get_sub_code_block(JagDecIntermediateRunContext *run_context);
void jag_dec_intermediate_run_context_set_sub_code_block(JagDecIntermediateRunContext *run_context, JagDecSubCodeBlock *block);

void jag_dec_intermediate_run_context_clear_pseudo_stack(JagDecIntermediateRunContext *run_context);
CatArrayWo *jag_dec_intermediate_run_context_get_pseudo_stack(JagDecIntermediateRunContext *run_context);


void jag_dec_intermediate_run_context_set_this(JagDecIntermediateRunContext *run_context, JagBytName *import_type, JagBytName *super_type);

void jag_dec_intermediate_run_context_connect(JagDecIntermediateRunContext *run_context, JagDecImportRepository *import_repository);

JagDecIntermediateRunContext *jag_dec_intermediate_run_context_create_branch(JagDecIntermediateRunContext *run_context);


void jag_dec_intermediate_run_context_add_value_type_definition_list(JagDecIntermediateRunContext *run_context, JagDecValueTypeDefinitionList *valueTypeDefList);

void jag_dec_intermediate_run_context_add_sub_code(JagDecIntermediateRunContext *run_context, JagDecISubCode *subCode);

void jag_dec_intermediate_run_context_add_post_importer(JagDecIntermediateRunContext *run_context, JagDecIPostImport *postImport);

void jag_dec_intermediate_run_context_push_pseudo_producer(JagDecIntermediateRunContext *run_context, JagDecIValueProducer *producer);


void jag_dec_intermediate_run_context_push_value_provider(JagDecIntermediateRunContext *run_context, JagDecIValueProducer *producer);
void jag_dec_intermediate_run_context_push_value_provider_at(JagDecIntermediateRunContext *run_context, JagDecIValueProducer *producer, int index);
JagDecIValueProducer *jag_dec_intermediate_run_context_pop_producer(JagDecIntermediateRunContext *run_context);
JagDecIValueProducer *jag_dec_intermediate_run_context_peek_producer(JagDecIntermediateRunContext *run_context, int index);

CatArrayWo *jag_dec_intermediate_run_context_get_producer_stack(JagDecIntermediateRunContext *run_context);

void jag_dec_intermediate_run_context_dump_stacks(JagDecIntermediateRunContext *run_context);


void jag_dec_intermediate_run_context_push_value_consumer(JagDecIntermediateRunContext *run_context, JagDecIConsumer *consumer);

JagDecMethodFrameValue *jag_dec_intermediate_run_context_get_method_frame_value(JagDecIntermediateRunContext *run_context, int frameIndex);

JagDecMethodFrameValue *jag_dec_intermediate_run_context_create_method_frame_value(JagDecIntermediateRunContext *run_context, int frameIndex);

void jag_dec_intermediate_run_context_add_frame_value(JagDecIntermediateRunContext *run_context, int frameIndex, JagDecMethodFrameValue *methodFrameValue);

void jag_dec_intermediate_run_context_add_value_type_definition_list(JagDecIntermediateRunContext *run_context, JagDecValueTypeDefinitionList *valueTypeDefList);

void jag_dec_intermediate_run_context_add_sub_code(JagDecIntermediateRunContext *run_context, JagDecISubCode *subCode);

void jag_dec_intermediate_run_context_add_post_importer(JagDecIntermediateRunContext *run_context, JagDecIPostImport *postImport);

void jag_dec_intermediate_run_context_add_and_run_sub_code(JagDecIntermediateRunContext *run_context, JagDecISubCode *sub_code);



G_END_DECLS


#endif /* JAGDECINTERMEDIATERUNCONTEXT_H_ */
