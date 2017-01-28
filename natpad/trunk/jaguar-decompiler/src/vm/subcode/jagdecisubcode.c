/*
   File:    jagdecisubcode.c
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

#include "jagdecisubcode.h"

G_DEFINE_INTERFACE(JagDecISubCode, jag_dec_isub_code, G_TYPE_OBJECT);


static void jag_dec_isub_code_default_init(JagDecISubCodeInterface *klass) {
}


void jag_dec_isub_code_connect_stage_two(JagDecISubCode *self, struct _JagDecModelStageTwoRuntime *modelRuntime) {
	g_return_if_fail(JAG_DEC_IS_ISUB_CODE(self));
	JAG_DEC_ISUB_CODE_GET_INTERFACE(self)->connectStageTwo(self, modelRuntime);
}


struct _JagDecIConsumer *jag_dec_isub_code_get_input_consumer_at(JagDecISubCode *self, int index) {
	g_return_val_if_fail(JAG_DEC_IS_ISUB_CODE(self), NULL);
	return JAG_DEC_ISUB_CODE_GET_INTERFACE(self)->getInputConsumerAt(self, index);
}

struct _JagDecIValueProducer *jag_dec_isub_code_get_output_producer(JagDecISubCode *self) {
	g_return_val_if_fail(JAG_DEC_IS_ISUB_CODE(self), NULL);
	return JAG_DEC_ISUB_CODE_GET_INTERFACE(self)->getOutputProducer(self);
}
