/*
   File:    jagbytattributesignature.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Mar 7, 2012
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

#include "jagbytattributesignature.h"
#include "../constant/jagbytconstantutf8text.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeSignature"
#include <logging/catlog.h>

CatS jag_txt_attribute_signature  = CAT_S_DEF("Signature");

struct _JagBytAttributeSignaturePrivate {
	CatStringWo *e_attribute_data;
	JagBytIConstantProvider *constant_provider;

	CatStringWo *a_signature_text;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytAttributeSignature, jag_byt_attribute_signature, JAG_BYT_TYPE_ATTRIBUTE)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_attribute_signature_class_init(JagBytAttributeSignatureClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_attribute_signature_init(JagBytAttributeSignature *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytAttributeSignature *instance = JAG_BYT_ATTRIBUTE_SIGNATURE(object);
	JagBytAttributeSignaturePrivate *priv = jag_byt_attribute_signature_get_instance_private(instance);
	cat_unref_ptr(priv->e_attribute_data);
	cat_unref_ptr(priv->constant_provider);
	cat_unref_ptr(priv->a_signature_text);
	G_OBJECT_CLASS(jag_byt_attribute_signature_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_attribute_signature_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeSignature *jag_byt_attribute_signature_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider) {
	JagBytAttributeSignature *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_SIGNATURE, NULL);
	cat_ref_anounce(result);
	JagBytAttributeSignaturePrivate *priv = jag_byt_attribute_signature_get_instance_private(result);
	priv->e_attribute_data = cat_ref_ptr(e_attribute_data);
	priv->constant_provider = cat_ref_ptr(constant_provider);
	priv->a_signature_text = NULL;

	int sign_idx1 = cat_string_wo_char_at(e_attribute_data, 0);
	int sign_idx2 = cat_string_wo_char_at(e_attribute_data, 1);
	int sign_idx = ((sign_idx1&0xff) << 8) + (sign_idx2&0xff);
	JagBytIConstant *const_obj = jag_byt_iconstant_provider_get(constant_provider, sign_idx-1);
	cat_dump_hex(cat_string_wo_getchars(e_attribute_data), cat_string_wo_length(e_attribute_data));
	if (JAG_BYT_IS_CONSTANT_UTF8_TEXT(const_obj)) {
		JagBytConstantUtf8Text *sign_utf8 = JAG_BYT_CONSTANT_UTF8_TEXT(const_obj);
		priv->a_signature_text = cat_ref_ptr(jag_byt_constant_utf8_text_get_value(sign_utf8));
		cat_log_detail("signature_text=%s", cat_string_wo_getchars(priv->a_signature_text));
	} else {
		cat_log_error("invalid signature:%d", sign_idx);
	}
	return result;
}

CatStringWo *jag_byt_attribute_signature_as_text(JagBytAttributeSignature *signature) {
	JagBytAttributeSignaturePrivate *priv = jag_byt_attribute_signature_get_instance_private(signature);
	return priv->a_signature_text;
}
