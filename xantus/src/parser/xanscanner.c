/*
   File:    xanscanner.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 11, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "xanscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "XanScanner"
#include <logging/catlog.h>


typedef enum _XanScanMode XanScanMode;

enum _XanScanMode {
	XAN_MODE_TEXT,
	XAN_MODE_OPENING,
	XAN_MODE_TERMINATOR_START,
	XAN_MODE_TERMINATOR_NAME,
	XAN_MODE_TERMINATOR_NAME_COLON,
	XAN_MODE_ELEMENT_START,
	XAN_MODE_ELEMENT_NAME,
	XAN_MODE_ELEMENT_NAME_COLON,
	XAN_MODE_TERMINATOR,
	XAN_MODE_ELEMENT
};

struct _XanScannerPrivate {
	XanScanMode scan_mode;
	XanScanMode prev_scan_mode;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);


G_DEFINE_TYPE_WITH_CODE(XanScanner, xan_scanner, GRORUN_TYPE_SCANNER_BASE,
		G_ADD_PRIVATE(XanScanner)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void xan_scanner_class_init(XanScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;


}

static void xan_scanner_init(XanScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	XanScanner *instance = XAN_SCANNER(object);
//	XanScannerPrivate *priv = xan_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(xan_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


XanScanner *xan_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *input, CatArrayWo *connected_symbols) {
	XanScanner *result = g_object_new(XAN_TYPE_SCANNER, NULL);
	cat_ref_anounce(result);
	XanScannerPrivate *priv = xan_scanner_get_instance_private(result);
	priv->scan_mode = XAN_MODE_TEXT;
	priv->prev_scan_mode = XAN_MODE_TEXT;
	grorun_scanner_base_construct((GroRunScannerBase *) result, token_factory, input, NULL, connected_symbols, 5);
	return result;
}

static GroRunIToken *l_scanner_next(GroRunIScanner *self);

GroRunIToken *xan_scanner_next(XanScanner *scanner) {
	return l_scanner_next((GroRunIScanner *) scanner);
}


/********************* start GroRunIScanner implementation *********************/

static GroRunIToken *l_scan_mode_text(XanScanner *xan_scanner);
static GroRunIToken *l_scan_mode_opening(XanScanner *xan_scanner);
static GroRunIToken *l_scan_mode_start(XanScanner *xan_scanner);
static GroRunIToken *l_scan_mode_open_name(XanScanner *xan_scanner);
static GroRunIToken *l_scan_mode_open_name_colon(XanScanner *xan_scanner);
static GroRunIToken *l_scan_mode_element(XanScanner *xan_scanner);

static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	GroRunIToken *result = NULL;
	XanScanner *scanner = XAN_SCANNER(self);
	XanScannerPrivate *priv = xan_scanner_get_instance_private(scanner);


	while(result==NULL) {
		cat_log_debug("mode=%d", priv->scan_mode);
		switch(priv->scan_mode) {
			case XAN_MODE_TEXT :
				result = l_scan_mode_text(scanner);
				break;
			case XAN_MODE_OPENING :
				result = l_scan_mode_opening(scanner);
				break;

			case XAN_MODE_ELEMENT_START :
			case XAN_MODE_TERMINATOR_START :
				result = l_scan_mode_start(scanner);
				break;

			case XAN_MODE_ELEMENT_NAME :
			case XAN_MODE_TERMINATOR_NAME :
				result = l_scan_mode_open_name(scanner);
				break;

			case XAN_MODE_ELEMENT_NAME_COLON :
			case XAN_MODE_TERMINATOR_NAME_COLON :
				result = l_scan_mode_open_name_colon(scanner);
				break;


			case XAN_MODE_TERMINATOR :
			case XAN_MODE_ELEMENT :
				result = l_scan_mode_element(scanner);
				break;


			default :
				priv->scan_mode = XAN_MODE_TEXT;
				break;

		}
	}


	cat_log_info("result=%O", result);

	return result;
}


static GroRunIToken *l_scan_mode_text(XanScanner *xan_scanner) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);

	base_class->markLocation(scanner);

	gboolean has_chars = FALSE;

	gboolean keep_running = TRUE;

	while(keep_running) {
		switch(lookahead[0]) {
			case -1 :
				if (!has_chars) {
					result = base_class->createToken(scanner, XAN_SYM_END_OF_INPUT, -1, -1, NULL);
				}
				keep_running = FALSE;
				break;

			case '<' :
				priv->prev_scan_mode = priv->scan_mode;
				priv->scan_mode = XAN_MODE_OPENING;
				keep_running = FALSE;
				break;

			default :
				base_class->advance(scanner, 0);
				has_chars = TRUE;
				break;
		}

	}

	if (has_chars) {
		int row = base_class->getLeftRow(scanner);
		int column = base_class->getLeftColumn(scanner);
		result = base_class->createToken(scanner, XAN_SYM_TEXT, row, column, NULL);
	}
	return result;
}


static GroRunIToken *l_scan_comment(XanScanner *xan_scanner) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);
	base_class->advance(scanner, 0);	// <
	base_class->advance(scanner, 0);	// !
	base_class->advance(scanner, 0);	// -
	base_class->advance(scanner, 0);	// -

	while(TRUE) {

		if (lookahead[0]==-1) {
			break;
		}
		if ((lookahead[0]=='-') && (lookahead[1]=='-') && (lookahead[2]=='>')) {
			base_class->advance(scanner, 0);	// -
			base_class->advance(scanner, 0);	// -
			base_class->advance(scanner, 0);	// >
			break;
		}

		base_class->advance(scanner, 0);
	}

	priv->scan_mode = priv->prev_scan_mode;
	int row = base_class->getRow(scanner);
	int column = base_class->getColumn(scanner);
	return base_class->createToken(scanner, XAN_SYM_COMMENT, row, column, NULL);
}


static GroRunIToken *l_scan_mode_opening(XanScanner *xan_scanner) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);

	base_class->markLocation(scanner);

	if (lookahead[0]!='<') {
		cat_log_error("No '<' character found, but was expected");
		priv->scan_mode = priv->prev_scan_mode;
		return NULL;
	}

	if  ((lookahead[1]=='!') && (lookahead[2]=='-') && (lookahead[3]=='-')) {
		return l_scan_comment(xan_scanner);
	}

	if (lookahead[1]=='/') {
		priv->scan_mode = XAN_MODE_TERMINATOR_START;
		return base_class->createTokenBasic(scanner, XAN_SYM_TAG_OPEN_TERMINATOR, 2);
	}


	// TODO DATA


	priv->scan_mode = XAN_MODE_ELEMENT_START;
	return base_class->createTokenBasic(scanner, XAN_SYM_TAG_OPEN, 1);

}


static GroRunIToken *l_scan_mode_start_or_name_colon(XanScanner *xan_scanner, gboolean *has_chars_out) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);
	base_class->advance(scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES|GRORUN_ADVANCE_ALLOW_STAY);
	base_class->markLocation(scanner);

	gboolean has_chars = FALSE;
	gboolean keep_running = TRUE;


	while(keep_running) {
		gunichar ch = lookahead[0];
		cat_log_debug("%d", ch);
		if (ch==-1) {
			if (!has_chars) {
				result = base_class->createToken(scanner, XAN_SYM_END_OF_INPUT, -1, -1, NULL);
			}
			keep_running = FALSE;
			break;
		}


		if (g_unichar_isalpha(ch) || ch=='-' || ch=='_') {
			base_class->advance(scanner, 0);
			has_chars = TRUE;
		} else if (ch=='>'){
			if (!has_chars) {
				result = base_class->createTokenBasic(scanner, XAN_SYM_TAG_CLOSE, 1);
				priv->scan_mode = XAN_SYM_TEXT;
			}
			keep_running = FALSE;
			break;
		} else if ((ch=='/') && (lookahead[1]=='>')) {
			if (!has_chars) {
				result = base_class->createTokenBasic(scanner, XAN_SYM_TAG_TERMINATOR_CLOSE, 2);
				priv->scan_mode = XAN_SYM_TEXT;
			}
			keep_running = FALSE;
			break;
		} else {
			if (!has_chars) {
				// TODO create text token and mark it erroneous
				base_class->advance(scanner, 0);
				base_class->markLocation(scanner);
				continue;
			}
			keep_running = FALSE;
		}
	}
	*has_chars_out = has_chars;
	return result;
}

static GroRunIToken *l_scan_mode_start(XanScanner *xan_scanner) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);

	gboolean has_chars = FALSE;
	GroRunIToken *result = l_scan_mode_start_or_name_colon(xan_scanner, &has_chars);

	if (has_chars && result == NULL) {
		int row = base_class->getLeftRow(scanner);
		int column = base_class->getLeftColumn(scanner);
		result = base_class->createToken(scanner, XAN_SYM_NAME, row, column, NULL);
		if (lookahead[0]==':') {
			if (priv->scan_mode == XAN_MODE_TERMINATOR_START) {
				priv->scan_mode = XAN_MODE_TERMINATOR_NAME;
			} else {
				priv->scan_mode = XAN_MODE_ELEMENT_NAME;
			}
		} else {
			if (priv->scan_mode == XAN_MODE_TERMINATOR_START) {
				priv->scan_mode = XAN_MODE_TERMINATOR;
			} else {
				priv->scan_mode = XAN_MODE_ELEMENT;
			}
		}
	}
	return result;
}


static GroRunIToken *l_scan_mode_open_name_colon(XanScanner *xan_scanner) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
//	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);

	gboolean has_chars = FALSE;
	GroRunIToken *result = l_scan_mode_start_or_name_colon(xan_scanner, &has_chars);

	if (has_chars && result == NULL) {
		int row = base_class->getRow(scanner);
		int column = base_class->getColumn(scanner);
		result = base_class->createToken(scanner, XAN_SYM_NAME, row, column, NULL);
		if (priv->scan_mode == XAN_MODE_TERMINATOR_NAME_COLON) {
			priv->scan_mode = XAN_MODE_TERMINATOR;
		} else {
			priv->scan_mode = XAN_MODE_ELEMENT;
		}
	}
	return result;
}


static GroRunIToken *l_scan_mode_open_name(XanScanner *xan_scanner) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	base_class->markLocation(scanner);
	if (priv->scan_mode==XAN_MODE_ELEMENT_NAME) {
		priv->scan_mode = XAN_MODE_ELEMENT_NAME_COLON;
	} else {
		priv->scan_mode = XAN_MODE_TERMINATOR_NAME_COLON;
	}
	return base_class->createTokenBasic(scanner, XAN_SYM_COLON, 1);
}


static GroRunIToken *l_scan_mode_element(XanScanner *xan_scanner) {
	XanScannerPrivate *priv = xan_scanner_get_instance_private(xan_scanner);
	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
	GroRunIToken *result = NULL;
	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);
	base_class->advance(scanner, GRORUN_ADVANCE_STRIP_LINE_BREAKS|GRORUN_ADVANCE_STRIP_WHITE_SPACES|GRORUN_ADVANCE_ALLOW_STAY);
	base_class->markLocation(scanner);

	while(result==NULL) {
		gunichar ch = lookahead[0];
		switch(ch) {
			case -1 :
				result = base_class->createToken(scanner, XAN_SYM_END_OF_INPUT, -1, -1, NULL);
				break;
			case '<' :
				if (lookahead[1]=='/') {
					result = base_class->createTokenBasic(scanner, XAN_SYM_TAG_OPEN_TERMINATOR, 2);
					grorun_itoken_set_state(result, GRORUN_STATE_SCANNER_ERROR);
					priv->scan_mode = XAN_MODE_TERMINATOR_START;
				} else {
					result = base_class->createTokenBasic(scanner, XAN_SYM_TAG_OPEN, 1);
					grorun_itoken_set_state(result, GRORUN_STATE_SCANNER_ERROR);
					priv->scan_mode = XAN_MODE_ELEMENT_START;
				}
				break;
			case '>' :
				result = base_class->createTokenBasic(scanner, XAN_SYM_TAG_CLOSE, 1);
				priv->scan_mode = XAN_MODE_TEXT;
				break;
			case '\'' :
			case '"' :
				result = base_class->scanForQuotedString(scanner, XAN_SYM_LITERAL);
				break;
			default :
				if (g_unichar_isalpha(ch)) {
					result = base_class->scanForIdOrKeyword(scanner, XAN_SYM_ID, TRUE);
				} else {
					// TODO mark error
					base_class->advance(scanner, 0);
					base_class->markLocation(scanner);
				}
		}
	}
	return result;
}


//
//static GroRunIToken *l_scan_text(GroRunIScanner *self) {
//	GroRunScannerBase *scanner = (GroRunScannerBase *) self;
//	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
//	GroRunIToken *result = NULL;
//	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);
//
//	while(result==NULL) {
//		base_class->markLocation(scanner);
////		cat_log_debug("lookahead[0]=%c", lookahead[0]);
//		switch(lookahead[0]) {
//			case -1 :
//				result = base_class->createToken(scanner, XAN_SYM_END_OF_INPUT, -1, -1, NULL);
//				break;
//			case '<' :
//				result = base_class->createTokenBasic(scanner, XAN_SYM_ELEMENT_OPEN, 1);
//				break;
//			case '>' :
//				result = base_class->createTokenBasic(scanner, XAN_SYM_ELEMENT_CLOSE, 1);
//				break;
//			case '=' :
//				result = base_class->createTokenBasic(scanner, XAN_SYM_IS, 1);
//				break;
//			case '"' :
//				result = base_class->scanForQuotedString(scanner, XAN_SYM_LITERAL);
//				break;
//			case '\t' :
//			case ' ' :
//				result = l_scan_spaces(scanner);
//				break;
//			default :
//				if (g_unichar_isalpha(lookahead[0])) {
//					result = base_class->scanForIdOrKeyword(scanner, XAN_SYM_ID, TRUE);
//				}
//				base_class->advance(scanner, 0);
//				break;
//		}
//	}
//	return result;
//
//}
//
//static GroRunIToken *l_scan_spaces(XanScanner *xan_scanner) {
//	GroRunScannerBase *scanner = (GroRunScannerBase *) xan_scanner;
//	GroRunScannerBaseClass *base_class = GRORUN_SCANNER_BASE_GET_CLASS(scanner);
//	GroRunIToken *result = NULL;
//	gunichar *lookahead = base_class->getLookaheadBuffer(scanner);
//	while(TRUE) {
//		gunichar ch = lookahead[0];
//		if (ch==-1) {
//			break;
//		}
//		if (ch=='\t' || ch==' ' || ch==0 || ch==0xa || ch==0xd) {
//			base_class->advance(scanner, 0);
//		} else {
//			break;
//		}
//	}
//	int row = base_class->getRow(scanner);
//	int column = base_class->getColumn(scanner);
//	return base_class->createToken(scanner, XAN_SYM_SPACES, row, column, NULL);
//}


static void l_scanner_iface_init(GroRunIScannerInterface *iface) {

	iface->next = l_scanner_next;
}

/********************* end GroRunIScanner implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
