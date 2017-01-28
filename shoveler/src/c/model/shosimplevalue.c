/*
   File:    shosimplevalue.c
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
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

#include "shosimplevalue.h"
#include "shoivalue.h"
#include "../shokeywords.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoSimpleValue"
#include <logging/catlog.h>

struct _ShoSimpleValuePrivate {
	GObject *holder;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_value_iface_init(ShoIValueInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ShoSimpleValue, sho_simple_value, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ShoSimpleValue)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(SHO_TYPE_IVALUE, l_value_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_simple_value_class_init(ShoSimpleValueClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_simple_value_init(ShoSimpleValue *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoSimpleValue *instance = SHO_SIMPLE_VALUE(object);
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(instance);
	cat_unref_ptr(priv->holder);
	G_OBJECT_CLASS(sho_simple_value_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_simple_value_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoSimpleValue *sho_simple_value_new_string(CatStringWo *value) {
	ShoSimpleValue *result = g_object_new(SHO_TYPE_SIMPLE_VALUE, NULL);
	cat_ref_anounce(result);
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(result);
	priv->holder = cat_ref_ptr(value);
	return result;
}


ShoSimpleValue *sho_simple_value_new_boolean(gboolean val) {
	ShoSimpleValue *result = g_object_new(SHO_TYPE_SIMPLE_VALUE, NULL);
	cat_ref_anounce(result);
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(result);
	if (val) {
		priv->holder = cat_ref_ptr(CAT_S(sho_s_true));
	} else {
		priv->holder = cat_ref_ptr(CAT_S(sho_s_false));
	}
	return result;
}


ShoSimpleValue *sho_simple_value_new_integer(int val) {
	ShoSimpleValue *result = g_object_new(SHO_TYPE_SIMPLE_VALUE, NULL);
	cat_ref_anounce(result);
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(result);
	char txt[60];
	sprintf(txt, "%d", val);
	priv->holder = (GObject *) cat_string_wo_new_with(txt);
	return result;
}


ShoSimpleValue *sho_simple_value_new_double(double val) {
	ShoSimpleValue *result = g_object_new(SHO_TYPE_SIMPLE_VALUE, NULL);
	cat_ref_anounce(result);
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(result);
	char txt[100];
	sprintf(txt, "%lf", val);
	priv->holder = (GObject *) cat_string_wo_new_with(txt);
	return result;
}


CatStringWo *sho_simple_value_get_string(ShoSimpleValue *value) {
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(value);
	return (CatStringWo *) priv->holder;
}

gboolean sho_simple_value_get_boolean(ShoSimpleValue *value, gboolean default_value) {
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(value);
	CatStringWo *txt = CAT_STRING_WO(priv->holder);
	if (txt) {
		if (cat_string_wo_equal(txt, CAT_S(sho_s_true))) {
			return TRUE;
		} else if (cat_string_wo_equal(txt, CAT_S(sho_s_false))) {
			return FALSE;
		}
	}
	return default_value;
}


int sho_simple_value_get_integer(ShoSimpleValue *value, int default_value) {
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(value);
	CatStringWo *txt = CAT_STRING_WO(priv->holder);
	if (txt) {
		int val = 0;
		int idx = 0;
		int str_len = cat_string_wo_length(txt);
		gboolean is_negative = FALSE;
		gboolean value_parsed = FALSE;
		while(idx<str_len) {
			char ch = cat_string_wo_char_at(txt, idx);
			if (ch!=' ' && ch!='\t') {
				if (ch=='-') {
					is_negative = TRUE;
					idx++;
				} else if (ch=='+') {
					idx++;
				}
				break;
			}
			idx++;
		}
		while(idx<str_len) {
			char ch = cat_string_wo_char_at(txt, idx);
			if (ch>='0' && ch<='9') {
				val = val*10+(int) (ch-'0');
				value_parsed = TRUE;
			} else {
				break;
			}
			idx++;
		}
		if (is_negative) {
			val = -val;
		}

		if (value_parsed) {
			return val;
		}
	}
	return default_value;
}

double sho_simple_value_get_double(ShoSimpleValue *value, int default_value) {
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(value);
	CatStringWo *txt = CAT_STRING_WO(priv->holder);
	return strtod(cat_string_wo_getchars(txt), NULL);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ShoSimpleValue *instance = SHO_SIMPLE_VALUE(self);
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %o]", iname, self, priv->holder);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* start ShoIValue implementation *********************/

static ShoIValue *l_value_deep_copy(ShoIValue *self) {
	ShoSimpleValue *result = g_object_new(SHO_TYPE_SIMPLE_VALUE, NULL);
	cat_ref_anounce(result);
	ShoSimpleValuePrivate *priv = sho_simple_value_get_instance_private(result);
	ShoSimpleValuePrivate *spriv = sho_simple_value_get_instance_private((ShoSimpleValue *) self);
	priv->holder = cat_ref_ptr(spriv->holder);
	return (ShoIValue *) result;
}

static void l_value_iface_init(ShoIValueInterface *iface) {
	iface->deepCopy = l_value_deep_copy;

}

/********************* end ShoIValue implementation *********************/
