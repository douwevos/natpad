/*
   File:    cheaugmentrequest.c
   Project: cheetah
   Author:  Douwe Vos
   Date:    Dec 27, 2016
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

#include "cheaugmentrequest.h"
#include <gvlcodeparsing/keyWords.h>
#include "highlighting/CheKeyWordHighlighter.h"
#include "highlighting/cheExtraKeyWords.h"
#include "CheInputStream.h"
#include "checodes.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CheAugmentRequest"
#include <logging/catlog.h>

struct _CheAugmentRequestPrivate {
	CatLock *lock;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CheAugmentRequest, che_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(CheAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void che_augment_request_class_init(CheAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void che_augment_request_init(CheAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CheAugmentRequest *instance = CHE_AUGMENT_REQUEST(object);
	CheAugmentRequestPrivate *priv = che_augment_request_get_instance_private(instance);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(che_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(che_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CheAugmentRequest *che_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key, CatLock *lock) {
	CheAugmentRequest *result = g_object_new(CHE_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	CheAugmentRequestPrivate *priv = che_augment_request_get_instance_private(result);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	priv->lock = cat_ref_ptr(lock);
	return result;
}


///* Returns an error or warning code. */
static int che_get_error_code(Token token) {
    int errwarn = Token_getErrwarn(token);
	if (errwarn==GVLCP_WARN_NOT_ANSI) {
		return 0;
	}
    int result = 0;
    if (errwarn > 0) {
        /* TODO: not nice to use 0xFFFF here... This must be changed. */
    	result = (errwarn > 0xFFFF) ? CHE_ERROR : CHE_WARNING;
    } else if (token->code2 > CHE_CODE_ERROR) {
    	result = CHE_ERROR;
    }
    return result;
}


static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	CheAugmentRequestPrivate *priv = che_augment_request_get_instance_private(CHE_AUGMENT_REQUEST(request));
	cat_lock_lock(priv->lock);

    CheKeyWordHighlighter highlighter = newCheKeyWordHighlighter();
    PushbackStream stream = newPushbackStream((InputStream) newCheInputStream(a_revision));
    TokenList tokens = highlighter->call->getTokens(highlighter, stream);
    highlighter->call->dump(highlighter);

    DraLineTagWo *e_line_tag_wo = NULL;
    int current_error = -1;

    int i = 0;
    while (Token_getCode(tokens->buf[i]) != GVLCP_CODE_LAST_TOKEN) {
        Token token = tokens->buf[i];
//        cat_log_detail("token.code=%d", token->code);
        const int left = token->column - 1;
        const int right = left + token->length;
        int row = token->line - 1;
        int error = che_get_error_code(token);

//        if (token->element) {
//            cat_log_error("token.text=%s, code=%d, row=%d, left=%d", (gulong) token->element->buf, token->code, row, left);
//        }

        if (error>0) {
        	if (e_line_tag_wo) {
        		if ((dra_line_tag_wo_get_row(e_line_tag_wo) == row) && (current_error==error)) {
        			dra_line_tag_wo_set_end_index(e_line_tag_wo, right);
        		} else {
                	if (e_line_tag_wo) {
                		dra_keyword_printer_print_line_tag(line_tag_printer, e_line_tag_wo);
                		e_line_tag_wo = NULL;
                	}
        		}
        	}

        	if (e_line_tag_wo == NULL) {
        		e_line_tag_wo = dra_line_tag_wo_new(row, DRA_TAG_TYPE_PARSER_ERROR);
        		switch(error) {
        			case CHE_ERROR:
        				dra_line_tag_wo_set_color(e_line_tag_wo, 1,0.3,0.3);
        				break;

        			case CHE_WARNING:
        				dra_line_tag_wo_set_color(e_line_tag_wo, 0.7, 0.7,0);
        				break;
        		}
        		dra_line_tag_wo_set_start_and_end_index(e_line_tag_wo, left, right);
        		current_error = error;
        	}

        } else {
        	if (e_line_tag_wo) {
        		dra_keyword_printer_print_line_tag(line_tag_printer, e_line_tag_wo);
        		e_line_tag_wo = NULL;
        	}
        }
        token->code2 &= CHE_CODE_MASK;

        dra_keyword_printer_print_fg_color(keyword_printer, row, left, row, right, token->code2);
        ++i;
    }
    tokens->call->dump(tokens);

    dra_keyword_printer_print(keyword_printer, -1, 0, -1, 0, NULL);

    if (e_line_tag_wo) {
		dra_keyword_printer_print_line_tag(line_tag_printer, e_line_tag_wo);
		e_line_tag_wo = NULL;
	}

	cat_lock_unlock(priv->lock);
	return TRUE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CheAugmentRequest *instance = CHE_AUGMENT_REQUEST(self);
	CheAugmentRequestPrivate *priv = che_augment_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
