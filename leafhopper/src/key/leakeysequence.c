/*
   File:    leakeysequence.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 30, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#include "leakeysequence.h"
#include <stdarg.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaKeySequence"
#include <logging/catlog.h>

struct _LeaKeySequencePrivate {
	CatArrayWo *a_sequence;
	int hash_code;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaKeySequence, lea_key_sequence, G_TYPE_INITIALLY_UNOWNED, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static int l_validate_and_calculate_hashcode(LeaKeySequence *sequence);

static void lea_key_sequence_class_init(LeaKeySequenceClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaKeySequencePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_key_sequence_init(LeaKeySequence *instance) {
	LeaKeySequencePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, LEA_TYPE_KEY_SEQUENCE, LeaKeySequencePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaKeySequence *instance = LEA_KEY_SEQUENCE(object);
	LeaKeySequencePrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_sequence);
	G_OBJECT_CLASS(lea_key_sequence_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_key_sequence_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

LeaKeySequence *lea_key_sequence_new(LeaKey *key) {
	LeaKeySequence *result = g_object_new(LEA_TYPE_KEY_SEQUENCE, NULL);
	cat_ref_anounce(result);
	LeaKeySequencePrivate *priv = result->priv;
	priv->a_sequence = cat_array_wo_new();
	cat_array_wo_append(priv->a_sequence, (GObject *) key);
	priv->hash_code = l_validate_and_calculate_hashcode(result);
	return result;
}

LeaKeySequence *lea_key_sequence_new_with(LeaKey *firstKey, ...) {
	LeaKeySequence *result = g_object_new(LEA_TYPE_KEY_SEQUENCE, NULL);
	cat_ref_anounce(result);
	LeaKeySequencePrivate *priv = result->priv;

	CatArrayWo *keys = cat_array_wo_new();

	va_list argp;
	va_start(argp, firstKey);
	while(TRUE) {
		void *val = va_arg(argp, void *);
		if (val==NULL) {
			break;
		}
		if (!LEA_IS_KEY(val)) {
			cat_log_error("one of the keys provided is not a LeaKey or the sequence was not ended with NULL");
			break;
		}
		cat_array_wo_append(keys, (GObject *) val);
	}
	va_end(argp);

	priv->a_sequence = keys;
	priv->hash_code = l_validate_and_calculate_hashcode(result);
	return result;
}


LeaKeySequence *lea_key_sequence_from_string(const CatStringWo *ca_txt) {
	LeaKeySequence *result = NULL;
	CatArrayWo *split = cat_string_wo_split(ca_txt, ' ', FALSE);
	if (split) {
		CatArrayWo *seq = cat_array_wo_new();
		CatIIterator *iter = cat_array_wo_iterator(split);
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *key_part = (CatStringWo *) cat_iiterator_next(iter);
			LeaKey *skey = lea_key_from_string(key_part);
			cat_log_debug("key_part=%o, skey=%o", key_part, skey);
			if (skey) {
				cat_array_wo_append(seq, (GObject *) skey);
				cat_unref_ptr(skey);
			}
		}
		cat_unref_ptr(iter);
		if (cat_array_wo_size(seq)>0) {
			result = g_object_new(LEA_TYPE_KEY_SEQUENCE, NULL);
			cat_ref_anounce(result);
			LeaKeySequencePrivate *priv = result->priv;
			priv->a_sequence = seq;
			priv->hash_code = l_validate_and_calculate_hashcode(result);
		} else {
			cat_unref_ptr(seq);
		}
		cat_unref_ptr(split);
	}
	cat_unref_ptr(ca_txt);
	return result;
}


static int l_validate_and_calculate_hashcode(LeaKeySequence *sequence) {
	int result = 0;
	LeaKeySequencePrivate *priv = LEA_KEY_SEQUENCE_GET_PRIVATE(sequence);
	CatIIterator *iter = cat_array_wo_iterator(priv->a_sequence);
	while(cat_iiterator_has_next(iter)) {
		LeaKey *key = (LeaKey *) cat_iiterator_next(iter);
		result = result*-13 + lea_key_hashcode(key);
	}
	cat_unref_ptr(iter);
	return result;
}

CatStringWo *lea_key_sequence_to_string(LeaKeySequence *sequence) {
	CatStringWo *result = cat_string_wo_new();
	LeaKeySequencePrivate *priv = LEA_KEY_SEQUENCE_GET_PRIVATE(sequence);
	CatIIterator *iter = cat_array_wo_iterator(priv->a_sequence);
	while(cat_iiterator_has_next(iter)) {
		LeaKey *key = (LeaKey *) cat_iiterator_next(iter);
		if (cat_string_wo_length(result)!=0) {
			cat_string_wo_append_char(result, ' ');
		}
		CatStringWo *key_txt = lea_key_to_string(key);
		cat_string_wo_append(result, key_txt);
		cat_unref_ptr(key_txt);
	}
	cat_unref_ptr(iter);
	return result;
}

CatIIterator *lea_key_sequence_iterator(LeaKeySequence *sequence) {
	LeaKeySequencePrivate *priv = LEA_KEY_SEQUENCE_GET_PRIVATE(sequence);
	return cat_array_wo_iterator(priv->a_sequence);
}


int lea_key_sequence_hash(LeaKeySequence *sequence) {
	return LEA_KEY_SEQUENCE_GET_PRIVATE(sequence)->hash_code;
}

gboolean lea_key_sequence_equal(LeaKeySequence *sequence_a, LeaKeySequence *sequence_b) {
	if (sequence_a==sequence_b) {
		return TRUE;
	}

	if ((sequence_a==NULL) || (sequence_b==NULL)) {
		return FALSE;
	}
	LeaKeySequencePrivate *priv_a = LEA_KEY_SEQUENCE_GET_PRIVATE(sequence_a);
	LeaKeySequencePrivate *priv_b = LEA_KEY_SEQUENCE_GET_PRIVATE(sequence_b);

	if ((priv_a->hash_code!=priv_b->hash_code) ||
			cat_array_wo_size(priv_a->a_sequence)!=cat_array_wo_size(priv_b->a_sequence)) {
		return FALSE;
	}
	gboolean result = TRUE;
	int idx;
	for(idx=cat_array_wo_size(priv_a->a_sequence)-1; idx>=0; idx--) {
		LeaKey *key_a = (LeaKey *) cat_array_wo_get(priv_a->a_sequence, idx);
		LeaKey *key_b = (LeaKey *) cat_array_wo_get(priv_b->a_sequence, idx);
		if (!lea_key_equal(key_a, key_b)) {
			result = FALSE;
			break;
		}
	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	CatStringWo *key_seq_text = lea_key_sequence_to_string(LEA_KEY_SEQUENCE(self));
	cat_string_wo_format(append_to, "%s[%o]", iname, key_seq_text);
	cat_unref_ptr(key_seq_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
