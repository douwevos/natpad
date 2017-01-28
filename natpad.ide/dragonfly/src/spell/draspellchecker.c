/*
   File:    draspellchecker.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "draspellchecker.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraSpellChecker"
#include <logging/catlog.h>

struct _DraSpellCheckerPrivate {
	CatIUtf8Scanner *input;
	CatStreamStatus input_status;
	gunichar lookahead[2];
	int left_column;
	int right_column;
	long long row;
	CatStringWo *buf;
	DraSpellWord spell_word;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraSpellChecker, dra_spell_checker, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraSpellChecker)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_spell_checker_class_init(DraSpellCheckerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_spell_checker_init(DraSpellChecker *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraSpellChecker *instance = DRA_SPELL_CHECKER(object);
//	DraSpellCheckerPrivate *priv = dra_spell_checker_get_instance_private(instance);
	G_OBJECT_CLASS(dra_spell_checker_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_spell_checker_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_scan_one_unichar(DraSpellCheckerPrivate *priv);
static gunichar l_advance_raw(DraSpellCheckerPrivate *priv);

DraSpellChecker *dra_spell_checker_new_at(CatIUtf8Scanner *input, int column, long long row) {
	DraSpellChecker *result = g_object_new(DRA_TYPE_SPELL_CHECKER, NULL);
	cat_ref_anounce(result);
	DraSpellCheckerPrivate *priv = dra_spell_checker_get_instance_private(result);
	priv->input = cat_ref_ptr(input);
	priv->input_status = CAT_STREAM_OK;
	priv->right_column = column;
	priv->left_column = column;
	priv->row = row;
	priv->buf = cat_string_wo_new();
	priv->spell_word.word = NULL;
	l_scan_one_unichar(priv);
	l_advance_raw(priv);
	return result;
}

DraSpellChecker *dra_spell_checker_new(CatIUtf8Scanner *input) {
	return dra_spell_checker_new_at(input, 0,0);
}



static void l_scan_one_unichar(DraSpellCheckerPrivate *priv) {
	int idx;
	priv->lookahead[0] = priv->lookahead[1];
	priv->lookahead[1] = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
}


static gunichar l_advance_raw(DraSpellCheckerPrivate *priv) {
	l_scan_one_unichar(priv);

	priv->left_column = priv->right_column;
	gunichar result = priv->lookahead[0];
	if (result==0xa) {
		if (priv->lookahead[1]==0xd) {
			l_scan_one_unichar(priv);
		}
		priv->row++;
		priv->right_column = 0;
	} else 	if (result==0xd) {
		if (priv->lookahead[1]==0xa) {
			l_scan_one_unichar(priv);
		}
		priv->row++;
		priv->right_column = -1;
	} else if (result==-1) {
		return -1;
	} else {
		if (result < 0x80) {
			priv->right_column++;
		} else if (result< (1<<12)) {
			priv->right_column+= 2;
		} else if (result< (1<<18)) {
			priv->right_column+= 3;
		} else if (result< (1<<24)) {
			priv->right_column+= 4;
		} else if (result< (1<<30)) {
			priv->right_column+= 5;
		} else {
			priv->right_column+= 6;
		}
	}
	return result;
}

const DraSpellWord dra_spell_checker_next_word(DraSpellChecker *checker) {
	DraSpellCheckerPrivate *priv = dra_spell_checker_get_instance_private(checker);
	cat_string_wo_clear(priv->buf);
	gboolean has_chars = FALSE;
	while(TRUE) {

		if (priv->lookahead[0]==-1) {
			break;
		}

		if (g_unichar_isalpha(priv->lookahead[0])) {
			cat_string_wo_append_unichar(priv->buf, priv->lookahead[0]);
			if (!has_chars) {
				has_chars = TRUE;
				priv->spell_word.column_start = priv->left_column;
				priv->spell_word.row = priv->row;
			}
			priv->spell_word.column_end = priv->right_column;
		} else {

			if (has_chars) {
				if (cat_string_wo_length(priv->buf)>1) {
					priv->spell_word.word = cat_string_wo_clone(priv->buf, CAT_CLONE_DEPTH_AS_ANCHORED);
				}
				return priv->spell_word;
			}
			has_chars = FALSE;
			cat_string_wo_clear(priv->buf);
		}
		l_advance_raw(priv);
	}

	cat_unref_ptr(priv->spell_word.word);
	if (has_chars) {
		if (cat_string_wo_length(priv->buf)>1) {
			priv->spell_word.word = cat_string_wo_clone(priv->buf, CAT_CLONE_DEPTH_AS_ANCHORED);
		}
	}
	return priv->spell_word;

}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraSpellChecker *instance = DRA_SPELL_CHECKER(self);
	DraSpellCheckerPrivate *priv = dra_spell_checker_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
