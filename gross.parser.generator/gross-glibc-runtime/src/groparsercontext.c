#include "groparsercontext.h"

G_DEFINE_TYPE(GroParserContext, gro_parser_context, G_TYPE_OBJECT)

static void _dispose(GObject *object);


static void gro_parser_context_class_init(GroParserContextClass *clazz) {

	clazz->shift = gro_parser_context_shift_real;
	clazz->push = gro_parser_context_push_real;
	clazz->pop = gro_parser_context_pop_real;
	clazz->peek = gro_parser_context_peek_real;
	clazz->getFromTop = gro_parser_context_get_from_top_real;
	
	clazz->scanNext = gro_parser_context_scan_next_real;
	
	clazz->reduce = gro_parser_context_reduce_real;
	clazz->peek_at = gro_parser_context_peek_at_real;
	
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void gro_parser_context_init(GroParserContext *obj) {
}

static void _dispose(GObject *object) {
	GroParserContext *instance = GRO_PARSER_CONTEXT(object);
	g_object_unref(instance->scanner);
	instance->scanner = NULL;
	g_object_unref(instance->e_stack);
	instance->e_stack = NULL;
	g_object_unref(instance->current_token);
	instance->current_token = NULL;
	g_object_unref(instance->e_lookahead);
	instance->e_lookahead = NULL;
}


GroParserContext *gro_parser_context_new(GroIScanner *scanner) {
	GroParserContext *result = g_object_new(GRO_TYPE_PARSER_CONTEXT, NULL);
	gro_parser_context_construct(result, scanner);
	return result;
}

void gro_parser_context_construct(GroParserContext *parser_context, GroIScanner *scanner) {
	parser_context->scanner = g_object_ref(scanner);
	parser_context->e_stack = g_ptr_array_new();
	parser_context->done_parsing = FALSE;
	parser_context->tos = 0;
	parser_context->current_token = NULL;
	parser_context->e_lookahead = NULL;
}





GroToken *gro_parser_context_scan_next_real(GroParserContext *parserContext, int eofSymbol) {
	GroToken *sym = gro_iscanner_next_token(parserContext->scanner);
	if (sym==NULL) {
		GRO_ISCANNER_GET_INTERFACE(parserContext->scanner)->createToken(parserContext->scanner, eofSymbol, -1, TRUE, FALSE, FALSE, 0, 0, 0, 0, NULL);
//		sym = gro_token_new_symbol(eofSymbol);
		
	}
	if (parserContext->current_token) {
		g_object_unref(parserContext->current_token);
	}
	parserContext->current_token = sym;
	return sym;
}



void gro_parser_context_done_parsing(GroParserContext *parser_context) {
	parser_context->done_parsing = TRUE;
}


GroToken *gro_parser_context_peek_real(GroParserContext *parser_context) {
	int idx = parser_context->e_stack->len;
	if (idx<=0) {
		return NULL;
	}
	return (GroToken*) g_ptr_array_index(parser_context->e_stack, idx-1);
}


GroToken *gro_parser_context_pop_real(GroParserContext *parser_context) {
	int idx = parser_context->e_stack->len;
	if (idx<=0) {
		return NULL;
	}
	parser_context->tos--;
	GroToken *result = NULL;
	result = g_ptr_array_index(parser_context->e_stack, idx-1);
	g_ptr_array_remove_index(parser_context->e_stack, idx-1);
	return result;
}


void gro_parser_context_push_real(GroParserContext *parser_context, GroToken *token) {
	g_ptr_array_add(parser_context->e_stack, token);
	g_object_ref(token);
}




void gro_parser_context_shift_real(GroParserContext *parser_context, GroToken *token) {
	g_ptr_array_add(parser_context->e_stack, token);
	g_object_ref(token);
	parser_context->tos++;
}

GroToken *gro_parser_context_get_from_top_real(GroParserContext *parser_context, int reverseIndex) {
	int idx = parser_context->tos-reverseIndex;
	if (idx<0 || idx>=parser_context->e_stack->len) {
		return NULL;
	}

	return (GroToken *) g_ptr_array_index(parser_context->e_stack, idx);
}




GroToken *gro_parser_context_peek_at_real(GroParserContext *parser_context, int reverse_index) {
	if (reverse_index<0) {
		return NULL;
	}
	int idx = parser_context->e_stack->len-1-reverse_index;
	
	if (idx<0) {
		return NULL;
	}
	return (GroToken*) g_ptr_array_index(parser_context->e_stack, idx);
}

void gro_parser_context_reduce_real(GroParserContext *parser_context, int replace_count, GroToken *replace_with_symbol) {
	if (replace_count>0) {
		parser_context->tos -= replace_count;
		int maxlen = parser_context->e_stack->len-replace_count;
		if (maxlen<0) {
			maxlen = 0;
		}
		while(parser_context->e_stack->len>maxlen) {
			GObject *dou = g_ptr_array_index(parser_context->e_stack, parser_context->e_stack->len-1);
			if (dou) {
//				g_object_unref(dou);
			}
			g_ptr_array_remove_index(parser_context->e_stack, parser_context->e_stack->len-1);
		}
	}
	g_ptr_array_add(parser_context->e_stack, replace_with_symbol);
	parser_context->tos++;
	if (replace_with_symbol) {
//		g_object_unref(replace_with_symbol);
	}
}


GroToken *gro_parser_context_current_error_token(GroParserContext *parserContext) {
	return (GroToken *) g_ptr_array_index(parserContext->e_lookahead, parserContext->lookahead_pos);
}

void gro_parser_context_dump(GroParserContext *parser_context) {
	int size = parser_context->e_stack->len;
	int idx;
	for(idx=size-1; idx>=0; idx--) {
		GroToken *token = (GroToken *) g_ptr_array_index(parser_context->e_stack, idx);
		printf("stack[%d] %s=%d, parse_state=%d\n", idx, token->is_terminal ?"terminal" : "non-term", token->sym, token->parse_state);
	}
	GroToken *token = parser_context->current_token;
	if (token) {
		printf("cur %s=%d, parse_state=%d\n", token->is_terminal ?"terminal" : "non-term", token->sym, token->parse_state);
	}
}


