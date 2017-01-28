/*
   File:    jagdecisubcode.h
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


#ifndef JAGDECISUBCODE_H_
#define JAGDECISUBCODE_H_

#include <glib-object.h>

#define JAG_DEC_TYPE_ISUB_CODE                 (jag_dec_isub_code_get_type())
#define JAG_DEC_ISUB_CODE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_DEC_TYPE_ISUB_CODE, JagDecISubCode))
#define JAG_DEC_IS_ISUB_CODE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_DEC_TYPE_ISUB_CODE))
#define JAG_DEC_ISUB_CODE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_DEC_TYPE_ISUB_CODE, JagDecISubCodeInterface))


typedef struct _JagDecISubCode               JagDecISubCode; /* dummy object */
typedef struct _JagDecISubCodeInterface      JagDecISubCodeInterface;

struct _JagDecModelStageTwoRuntime;
struct _JagDecIConsumer;
struct _JagDecIValueProducer;

struct _JagDecISubCodeInterface {
	GTypeInterface parent_iface;

	void (*connectStageTwo)(JagDecISubCode *self, struct _JagDecModelStageTwoRuntime *modelRuntime);

	struct _JagDecIConsumer *(*getInputConsumerAt)(JagDecISubCode *self, int index);

	struct _JagDecIValueProducer *(*getOutputProducer)(JagDecISubCode *self);

};

GType jag_dec_isub_code_get_type(void);

void jag_dec_isub_code_connect_stage_two(JagDecISubCode *self, struct _JagDecModelStageTwoRuntime *modelRuntime);

struct _JagDecIConsumer *jag_dec_isub_code_get_input_consumer_at(JagDecISubCode *self, int index);

struct _JagDecIValueProducer *jag_dec_isub_code_get_output_producer(JagDecISubCode *self);


#endif /* JAGDECISUBCODE_H_ */
