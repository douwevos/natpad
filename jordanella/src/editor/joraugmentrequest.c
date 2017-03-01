/*
   File:    joraugmentrequest.c
   Project: jordanella
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

#include "joraugmentrequest.h"
#include "jorscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JorAugmentRequest"
#include <logging/catlog.h>

struct _JorAugmentRequestPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JorAugmentRequest, jor_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(JorAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void jor_augment_request_class_init(JorAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void jor_augment_request_init(JorAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JorAugmentRequest *instance = JOR_AUGMENT_REQUEST(object);
//	JorAugmentRequestPrivate *priv = jor_augment_request_get_instance_private(instance);
	G_OBJECT_CLASS(jor_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jor_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JorAugmentRequest *jor_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key) {
	JorAugmentRequest *result = g_object_new(JOR_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	JorAugmentRequestPrivate *priv = jor_augment_request_get_instance_private(result);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	return result;
}





// expect [ or {
#define JOR_STATE_MAIN	                0

// expect <value>
#define JOR_STATE_LIST	                1

// expect , or ]
#define JOR_STATE_LIST_ENTRY	        2

// expect <key> or ]
#define JOR_STATE_MAP	                3

// expect :
#define JOR_STATE_MAP_KEY               4

// expect <value>
#define JOR_STATE_MAP_KEY_COLON         5

// expect , or ]
#define JOR_STATE_MAP_KEY_COLON_VALUE   6

/*
 case JOR_STATE_MAIN :
 case JOR_STATE_LIST :
 case JOR_STATE_LIST_ENTRY :
 case JOR_STATE_MAP :
 case JOR_STATE_MAP_KEY :
 case JOR_STATE_MAP_KEY_COLON :
 case JOR_STATE_MAP_KEY_COLON_VALUE :
 */

static int l_map_symbol(int symbol) {
	switch (symbol) {
		case JOR_SYMBOL_LIST_OPEN:
		case JOR_SYMBOL_LIST_CLOSE:
			return 3;
		case JOR_SYMBOL_MAP_OPEN:
		case JOR_SYMBOL_MAP_CLOSE:
			return 4;
		case JOR_SYMBOL_STRING:
			return 11;
		case JOR_SYMBOL_COLON:
		case JOR_SYMBOL_COMMA:
			return 12;
		case JOR_SYMBOL_TRUE:
		case JOR_SYMBOL_FALSE:
		case JOR_SYMBOL_NULL:
			return 13;

		case JOR_SYMBOL_NUMBER:
			return 7;

		case JOR_SYMBOL_KEY:
			return 9;

	}
	return 1;
}

struct _JorContext {
	JorAugmentRequestPrivate *priv;
	CatIntArrayWo *state_stack;
	int state_current;
	JorScanner *scanner;
	DraKeywordPrinter *keyword_printer;
	DraKeywordPrinter *line_tag_printer;
};

typedef struct _JorContext JorContext;


static void l_write_error(JorContext *context, JorToken *token, int error) {
	if (error != 0) {
		DraLineTagWo *line_tag = dra_line_tag_wo_new(token->start_row, DRA_TAG_TYPE_PARSER_ERROR);
		dra_line_tag_wo_set_color(line_tag, 1, 0, 0);
		dra_line_tag_wo_set_start_and_end_index(line_tag, token->start_column, token->end_column);
		dra_keyword_printer_print_line_tag(context->line_tag_printer, line_tag);
	}
}


static void l_handle_list_open(JorContext *context, JorToken *token) {
	dra_keyword_printer_print_fg_color(context->keyword_printer,
			token->start_row, token->start_column, token->end_row,
			token->end_column, l_map_symbol(token->code));
	int error = 0;
	cat_int_array_wo_append(context->state_stack, context->state_current);
	switch (context->state_current) {
		case JOR_STATE_MAIN:
		case JOR_STATE_LIST:
		case JOR_STATE_MAP_KEY_COLON:
			break;

		case JOR_STATE_LIST_ENTRY:
			error = 1;
			break;

		case JOR_STATE_MAP:
		case JOR_STATE_MAP_KEY:
		case JOR_STATE_MAP_KEY_COLON_VALUE:
			error = 2;
			break;
	}
	context->state_current = JOR_STATE_LIST;

	l_write_error(context, token, error);
}

static int l_handle_value_sub(JorContext *context) {
	switch (context->state_current) {
		case JOR_STATE_MAIN:
	//			context->state_current = JOR_STATE_DONE
			break;
		case JOR_STATE_LIST:
			context->state_current = JOR_STATE_LIST_ENTRY;
			break;
		case JOR_STATE_LIST_ENTRY:
			return 1;
		case JOR_STATE_MAP:
			return 1;
		case JOR_STATE_MAP_KEY:
			return 1;
		case JOR_STATE_MAP_KEY_COLON:
			context->state_current = JOR_STATE_MAP_KEY_COLON_VALUE;
			break;
		case JOR_STATE_MAP_KEY_COLON_VALUE:
			return 1;
	}

	return 0;
}

static void l_handle_list_close(JorContext *context, JorToken *token) {
	dra_keyword_printer_print_fg_color(context->keyword_printer,
			token->start_row, token->start_column, token->end_row,
			token->end_column, l_map_symbol(token->code));
	int error = 0;
	switch (context->state_current) {
		case JOR_STATE_MAIN:
			error = 1;
			break;
		case JOR_STATE_LIST_ENTRY:
		case JOR_STATE_LIST: {
				int old_state;
				if (cat_int_array_wo_remove_last(context->state_stack, &old_state)) {
					context->state_current = old_state;
					error = l_handle_value_sub(context);
				} else {
					error = 1;
				}
			}
			break;

		case JOR_STATE_MAP:
		case JOR_STATE_MAP_KEY:
		case JOR_STATE_MAP_KEY_COLON:
		case JOR_STATE_MAP_KEY_COLON_VALUE:
			error = 1;
			break;
	}
	l_write_error(context, token, error);
}

static void l_handle_map_open(JorContext *context, JorToken *token) {
	dra_keyword_printer_print_fg_color(context->keyword_printer,
			token->start_row, token->start_column, token->end_row,
			token->end_column, l_map_symbol(token->code));
	cat_int_array_wo_append(context->state_stack, context->state_current);
	int error = 0;
	switch (context->state_current) {
		case JOR_STATE_MAP_KEY_COLON :
		case JOR_STATE_LIST :
		case JOR_STATE_MAIN :
			context->state_current = JOR_STATE_MAP;
			break;
		case JOR_STATE_MAP_KEY :
		case JOR_STATE_MAP :
		case JOR_STATE_LIST_ENTRY :
		case JOR_STATE_MAP_KEY_COLON_VALUE :
		default :
			error = 1;
			context->state_current = JOR_STATE_MAP;
			break;
	}
	l_write_error(context, token, error);
}


static void l_handle_map_close(JorContext *context, JorToken *token) {
	dra_keyword_printer_print_fg_color(context->keyword_printer,
			token->start_row, token->start_column, token->end_row,
			token->end_column, l_map_symbol(token->code));

	int error = 0;
	switch (context->state_current) {
		case JOR_STATE_MAIN:
			error = 1;
			break;
		case JOR_STATE_MAP_KEY :
		case JOR_STATE_MAP_KEY_COLON : {
				error = 1;
				int old_state;
				if (cat_int_array_wo_remove_last(context->state_stack, &old_state)) {
					context->state_current = old_state;
					error = l_handle_value_sub(context);
				} else {
					error = 1;
				}
			}
			break;
		case JOR_STATE_MAP :
		case JOR_STATE_MAP_KEY_COLON_VALUE: {
				int old_state;
				if (cat_int_array_wo_remove_last(context->state_stack, &old_state)) {
					context->state_current = old_state;
					error = l_handle_value_sub(context);
				} else {
					error = 1;
				}
			}
			break;

		case JOR_STATE_LIST:
		case JOR_STATE_LIST_ENTRY:
			error = 1;
			break;
	}

	l_write_error(context, token, error);
}


static void l_handle_key_or_value(JorContext *context, JorToken *token) {

	int code = token->code;

	int error = 0;
	switch (context->state_current) {
		case JOR_STATE_MAIN:
			error = 1;
			break;
		case JOR_STATE_LIST:
			context->state_current = JOR_STATE_LIST_ENTRY;
			break;
		case JOR_STATE_LIST_ENTRY:
			error = 1;
			break;
		case JOR_STATE_MAP:
			context->state_current = JOR_STATE_MAP_KEY;
			code = JOR_SYMBOL_KEY;
			break;
		case JOR_STATE_MAP_KEY:
			context->state_current = JOR_STATE_MAP_KEY_COLON_VALUE;
			error = 1;
			break;
		case JOR_STATE_MAP_KEY_COLON:
			context->state_current = JOR_STATE_MAP_KEY_COLON_VALUE;
			break;
		case JOR_STATE_MAP_KEY_COLON_VALUE:
			error = 1;
			context->state_current = JOR_STATE_MAP_KEY;
			code = JOR_SYMBOL_KEY;
			break;
	}

	dra_keyword_printer_print_fg_color(context->keyword_printer,
			token->start_row, token->start_column, token->end_row,
			token->end_column, l_map_symbol(code));

	l_write_error(context, token, error);
}


static void l_handle_colon(JorContext *context, JorToken *token) {
	dra_keyword_printer_print_fg_color(context->keyword_printer,
			token->start_row, token->start_column, token->end_row,
			token->end_column, l_map_symbol(token->code));

	int error = 0;
	switch (context->state_current) {
		case JOR_STATE_LIST:
		case JOR_STATE_MAIN:
			error = 1;
			break;
		case JOR_STATE_LIST_ENTRY:
			context->state_current = JOR_STATE_LIST;
			error = 1;
			break;
		case JOR_STATE_MAP:
			error = 1;
			context->state_current = JOR_STATE_MAP_KEY_COLON;
			break;
		case JOR_STATE_MAP_KEY:
			context->state_current = JOR_STATE_MAP_KEY_COLON;
			break;
		case JOR_STATE_MAP_KEY_COLON:
			error = 1;
			break;
		case JOR_STATE_MAP_KEY_COLON_VALUE:
			error = 1;
			context->state_current = JOR_STATE_MAP_KEY;
			break;
	}

	l_write_error(context, token, error);
}


static void l_handle_comma(JorContext *context, JorToken *token) {
	dra_keyword_printer_print_fg_color(context->keyword_printer, token->start_row, token->start_column, token->end_row, token->end_column, l_map_symbol(token->code));

	int error = 0;
	switch (context->state_current) {
		case JOR_STATE_MAIN:
			error = 1;
			break;
		case JOR_STATE_LIST:
			error = 1;
			break;
		case JOR_STATE_LIST_ENTRY:
			context->state_current = JOR_STATE_LIST;
			break;
		case JOR_STATE_MAP:
			error = 1;
			break;
		case JOR_STATE_MAP_KEY:
			error = 1;
			context->state_current = JOR_STATE_MAP_KEY_COLON;
			break;
		case JOR_STATE_MAP_KEY_COLON:
			error = 1;
			break;
		case JOR_STATE_MAP_KEY_COLON_VALUE:
			context->state_current = JOR_STATE_MAP;
			break;
	}

	l_write_error(context, token, error);
}


static void l_handle_value(JorContext *context, JorToken *token) {
	dra_keyword_printer_print_fg_color(context->keyword_printer, token->start_row, token->start_column, token->end_row, token->end_column, l_map_symbol(token->code));

	int error = 0;
	switch (context->state_current) {
		case JOR_STATE_MAIN:
			error = 1;
			break;
		case JOR_STATE_LIST:
			context->state_current = JOR_STATE_LIST_ENTRY;
			break;
		case JOR_STATE_LIST_ENTRY :
			error = 1;
			break;
		case JOR_STATE_MAP:
			context->state_current = JOR_STATE_MAP_KEY;
			error = 1;
			break;
		case JOR_STATE_MAP_KEY:
			error = 1;
			context->state_current = JOR_STATE_MAP_KEY_COLON_VALUE;
			break;
		case JOR_STATE_MAP_KEY_COLON:
			context->state_current = JOR_STATE_MAP_KEY_COLON_VALUE;
			break;
		case JOR_STATE_MAP_KEY_COLON_VALUE:
			error = 1;
			context->state_current = JOR_STATE_MAP_KEY;
			break;
	}

	l_write_error(context, token, error);
}



static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	JorAugmentRequestPrivate *priv = jor_augment_request_get_instance_private(JOR_AUGMENT_REQUEST(request));

	JorContext context;
	context.keyword_printer = keyword_printer;
	context.line_tag_printer = line_tag_printer;

	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);

	context.priv = priv;
	context.state_stack = cat_int_array_wo_new();
	context.state_current = JOR_STATE_MAIN;

	context.scanner = jor_scanner_new(utf8_scanner);
	gboolean keep_scanning = TRUE;
	while (keep_scanning) {
		JorToken *token = jor_scanner_next(context.scanner);
		if (token == NULL) {
			break;
		}
		int error = 0;
		int symbol = token->code;
		switch (symbol) {
			case JOR_SYMBOL_LIST_OPEN: {
					l_handle_list_open(&context, token);
				}
				break;
			case JOR_SYMBOL_LIST_CLOSE:
				l_handle_list_close(&context, token);
				break;

			case JOR_SYMBOL_MAP_OPEN:
				l_handle_map_open(&context, token);
				break;

			case JOR_SYMBOL_MAP_CLOSE:
				l_handle_map_close(&context, token);
				break;

			case JOR_SYMBOL_STRING:
				l_handle_key_or_value(&context, token);
				break;

			case JOR_SYMBOL_COLON:
				l_handle_colon(&context, token);
				break;

			case JOR_SYMBOL_COMMA:
				l_handle_comma(&context, token);
				break;

			case JOR_SYMBOL_NUMBER:
			case JOR_SYMBOL_NULL:
			case JOR_SYMBOL_FALSE:
			case JOR_SYMBOL_TRUE:
				l_handle_value(&context, token);
				break;

			case JOR_SYMBOL_INVALID: {
		//				dra_keyword_printer_print_fg_color(keyword_printer, token->start_row, token->start_column, token->end_row, token->end_column, l_map_symbol(token->code));
		//				dra_keyword_printer_print_line_tag()
		//
					DraLineTagWo *line_tag = dra_line_tag_wo_new(token->start_row, DRA_TAG_TYPE_PARSER_ERROR);
					dra_line_tag_wo_set_color(line_tag, 1, 0, 0);
					dra_line_tag_wo_set_start_and_end_index(line_tag,
							token->start_column, token->end_column);
					dra_keyword_printer_print_line_tag(context.line_tag_printer,
							line_tag);

				}
				break;

			case JOR_SYMBOL_EOF:
			default: {
				keep_scanning = FALSE;
				break;
			}
		}
	}


	cat_unref_ptr(utf8_scanner);

	return TRUE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
