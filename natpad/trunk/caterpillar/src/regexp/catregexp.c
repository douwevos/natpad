/*
   File:    catregexp.c
   Project: Caterpillar
   Author:  Douwe Vos
   Date:    Dec 12, 2008
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

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

#include "catregexp.h"

#include "catregexpnodelist.h"
#include "catregexpnoderepeater.h"
#include "catregexpnodesinglecharacter.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatRegexp"
#include "../logging/catlog.h"



static void l_dispose(GObject *object);

G_DEFINE_TYPE(CatRegexp, cat_regexp, G_TYPE_OBJECT)

static void cat_regexp_class_init(CatRegexpClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void cat_regexp_init(CatRegexp *obj) {
}

static void l_dispose(GObject *object) {
	CatRegexp *regexp = CAT_REGEXP(object);
	cat_unref_ptr(regexp->root);
	cat_unref_ptr(regexp->a_pattern);
}

typedef struct _PatternContext PatternContext;

struct _PatternContext {
	int pattern_char_idx;
	CatStringWo *a_pattern;
};

typedef struct _PatternToken PatternToken;

struct _PatternToken {
	int pattern_char_idx;
	int code;
	int ref_count;
	CatStringWo *a_text;
};




char next_char(PatternContext *context) {
	if (context->pattern_char_idx>=cat_string_wo_length(context->a_pattern)) {
		return -1;
	}
	return cat_string_wo_char_at(context->a_pattern, context->pattern_char_idx++);
}

char peek_char(PatternContext *context) {
	if (context->pattern_char_idx>=cat_string_wo_length(context->a_pattern)) {
		return -1;
	}
	return cat_string_wo_char_at(context->a_pattern, context->pattern_char_idx);
}

PatternToken *create_token(int code, int offset, CatStringWo *a_text) {
	PatternToken *token = (PatternToken *) g_malloc(sizeof(PatternToken));
	token->code = code;
	token->pattern_char_idx = offset;
	token->a_text = cat_ref_ptr(a_text);
	return token;
}

static void _free_token(PatternToken *token) {
	cat_unref_ptr(token->a_text);
	g_free(token);
}

#define REGEXP_STATE_MAIN		1
#define REGEXP_STATE_ANY_OF		2


#define REGEXP_TOKEN_SINGLE_CHAR       0
#define REGEXP_TOKEN_ASTERISK          1
#define REGEXP_TOKEN_LPARAM            2
#define REGEXP_TOKEN_RPARAM            3
#define REGEXP_TOKEN_ANY_START         4
#define REGEXP_TOKEN_ANY_START_NEG     5
#define REGEXP_TOKEN_ANY_END           6
#define REGEXP_TOKEN_DOT               7

PatternToken *next_token(PatternContext *context, int state) {
	int mark = context->pattern_char_idx;
	char ch = next_char(context);
	switch(ch) {
		case -1 : {
			return NULL;
		}
		case '*' : {
			if (state==REGEXP_STATE_MAIN) {
				return create_token(REGEXP_TOKEN_ASTERISK, mark, NULL);
			}
		} break;
		case '\\' : {
			ch = peek_char(context);

			switch(ch) {
				case '(' : {
					next_char(context);
					if (state==REGEXP_STATE_MAIN) {
						return create_token(REGEXP_TOKEN_LPARAM, mark, NULL);
					}
				} break;
				case ')' : {
					next_char(context);
					if (state==REGEXP_STATE_MAIN) {
						return create_token(REGEXP_TOKEN_RPARAM, mark, NULL);
					}
				} break;
				case '.' : {
					next_char(context);
					if (state==REGEXP_STATE_MAIN) {
						return create_token(REGEXP_TOKEN_DOT, mark, NULL);
					}
				} break;
				default : {
					ch = next_char(context);
				}
			}
		} break;
		case '[' : {
			if (state==REGEXP_STATE_MAIN) {
				ch = peek_char(context);
				if (ch=='^') {
					next_char(context);
					return create_token(REGEXP_TOKEN_ANY_START_NEG, mark, NULL);
				}
				return create_token(REGEXP_TOKEN_ANY_START, mark, NULL);
			}
		} break;
		case ']' : {
			if (state==REGEXP_STATE_ANY_OF) {
				ch = peek_char(context);
				return create_token(REGEXP_TOKEN_ANY_END, mark, NULL);
			}
		} break;

		default : {
			if (state==REGEXP_STATE_ANY_OF) {
				char start_ch = ch;
				ch = next_char(context);
				if (ch=='-') {
					ch = next_char(context);
					if (ch!=-1 && ch>start_ch) {
						int length = 1+ch-start_ch;
						char *data = (char *) g_malloc(sizeof(char)*(length));
						int idx = 0;
						for(; start_ch<=ch; start_ch++) {
							data[idx] = start_ch;
						}
						CatStringWo *chars = cat_string_wo_new_with_len(data, length);
						g_free(data);
						PatternToken *result = create_token(REGEXP_TOKEN_SINGLE_CHAR, mark, chars);
						cat_unref_ptr(chars);
						return result;
					}
				}
				context->pattern_char_idx = mark;
				ch = next_char(context);
			}
		} break;
	}
	CatStringWo *chars = cat_string_wo_new_with_len(&ch, 1);
	PatternToken *result = create_token(REGEXP_TOKEN_SINGLE_CHAR, mark, chars);
	cat_unref_ptr(chars);
	return result;
}


CatRegexp *cat_regexp_new(CatStringWo *a_pattern) {
	CatRegexp *regexp = g_object_new(CAT_TYPE_REGEXP, NULL);
	cat_ref_anounce(regexp);
	regexp->a_pattern = cat_ref_ptr(a_pattern);

	CatRegexpNodeList *current_node = cat_regexp_node_list_new();
	regexp->root = (CatRegexpNode *) cat_ref_ptr(current_node);

	PatternContext context;
	context.a_pattern = a_pattern;
	context.pattern_char_idx = 0;
	CatArrayWo *e_stack = cat_array_wo_new();
	CatRegexpNodeSingleCharacter *any_of_node = NULL;

	int state = REGEXP_STATE_MAIN;
	while(TRUE) {
		PatternToken *token = next_token(&context, state);
		if (token == NULL) {
			break;
		}
		cat_log_trace("token:code=%d", token->code);

		switch(state) {
			case REGEXP_STATE_MAIN : {
				switch(token->code) {
					case REGEXP_TOKEN_LPARAM : {
						cat_array_wo_append(e_stack, (GObject *) current_node);
						CatRegexpNodeList *new_node = cat_regexp_node_list_new();
						cat_regexp_node_list_add(current_node, (CatRegexpNode *) new_node);
						cat_unref_ptr(current_node);
						current_node = new_node;
					} break;

					case REGEXP_TOKEN_RPARAM : {
						CatRegexpNodeList *top_node = NULL;
						cat_array_wo_remove_last(e_stack, (GObject **) (&top_node));
						cat_unref_ptr(current_node);
						current_node = top_node;
					} break;

					case REGEXP_TOKEN_SINGLE_CHAR : {
						CatRegexpNode *single_char_node = (CatRegexpNode *) cat_regexp_node_single_character_new(token->a_text, FALSE, TRUE);
						cat_regexp_node_list_add(current_node, single_char_node);
						cat_unref_ptr(single_char_node);
					} break;
					case REGEXP_TOKEN_DOT : {
						CatRegexpNode *single_char_node = (CatRegexpNode *) cat_regexp_node_single_character_new(NULL, TRUE, FALSE);
						cat_regexp_node_list_add(current_node, single_char_node);
						cat_unref_ptr(single_char_node);
					} break;
					case REGEXP_TOKEN_ANY_START : {
						any_of_node = cat_regexp_node_single_character_new(NULL, FALSE, TRUE);
						cat_regexp_node_list_add(current_node, (CatRegexpNode *) any_of_node);
						cat_unref(any_of_node);
						state = REGEXP_STATE_ANY_OF;
					} break;
					case REGEXP_TOKEN_ASTERISK : {
						CatRegexpNode *last = cat_regexp_node_list_get_last(current_node);
						if (last==NULL) {
							last = (CatRegexpNode *) cat_regexp_node_single_character_new(NULL, TRUE, FALSE);
							cat_regexp_node_list_add(current_node, last);
							cat_unref(last);
						}

						CatRegexpNode *rep_node = (CatRegexpNode *) cat_regexp_node_repeater_new(last);
						cat_regexp_node_list_replace_last(current_node, rep_node);
						cat_unref_ptr(rep_node);
					} break;
				}
			} break;
			case REGEXP_STATE_ANY_OF : {
				switch(token->code) {
					case REGEXP_TOKEN_SINGLE_CHAR : {
						cat_regexp_node_single_character_append(any_of_node, token->a_text);
					} break;
					case REGEXP_TOKEN_ANY_END : {
						state = REGEXP_STATE_MAIN;
					} break;
				}
			} break;

		}

		_free_token(token);
	}

	cat_unref_ptr(current_node);
	cat_unref_ptr(e_stack);
	cat_log_detail("cat_regexp_new:end");
	cat_log_on_info({
		cat_regexp_node_dump(regexp->root, 0, TRUE);
	});

	return regexp;
}


gboolean cat_regexp_match(CatRegexp *this, CatStringWo *a_text) {
	CatRegexpNode *node = this->root;
	cat_log_debug("node=%p", node);
	return cat_regexp_node_matches(node, a_text, 0, cat_string_wo_length(a_text));
}

