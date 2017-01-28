#include "jagparsercontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagParserContext"
#include <logging/catlog.h>


G_DEFINE_TYPE(JagParserContext, jag_parser_context, G_TYPE_OBJECT)

static void _dispose(GObject *object);



static void jag_parser_context_class_init(JagParserContextClass *clazz) {

	clazz->shift = jag_parser_context_shift_real;
	clazz->push = jag_parser_context_push_real;
	clazz->pop = jag_parser_context_pop_real;
	clazz->peek = jag_parser_context_peek_real;
	clazz->getFromTop = jag_parser_context_get_from_top_real;
	
	clazz->scanNext = jag_parser_context_scan_next_real;
	
	clazz->reduce = jag_parser_context_reduce_real;
	clazz->peek_at = jag_parser_context_peek_at_real;
	
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void jag_parser_context_init(JagParserContext *obj) {
}

static void _dispose(GObject *object) {
	JagParserContext *instance = JAG_PARSER_CONTEXT(object);
	cat_unref_ptr(instance->scanner);
	cat_unref_ptr(instance->e_stack);
	cat_unref_ptr(instance->current_token);
	cat_unref_ptr(instance->e_lookahead);
}


JagParserContext *jag_parser_context_new(JagIScanner *scanner) {
	JagParserContext *result = g_object_new(JAG_TYPE_PARSER_CONTEXT, NULL);
	cat_ref_anounce(result);
	jag_parser_context_construct(result, scanner);
	return result;
}

void jag_parser_context_construct(JagParserContext *parser_context, JagIScanner *scanner) {
	parser_context->scanner = cat_ref_ptr(scanner);
	parser_context->e_stack = cat_array_wo_new();
	parser_context->done_parsing = FALSE;
	parser_context->tos = 0;
	parser_context->current_token = NULL;
	parser_context->e_lookahead = NULL;
}





JagToken *jag_parser_context_scan_next_real(JagParserContext *parserContext, int eofSymbol) {
	JagToken *sym = jag_iscanner_next_token(parserContext->scanner);
	if (sym==NULL) {
		JAG_ISCANNER_GET_INTERFACE(parserContext->scanner)->createToken(parserContext->scanner, eofSymbol, -1, TRUE, FALSE, FALSE, 0, 0, 0, 0, NULL);
//		sym = jag_token_new_symbol(eofSymbol);
		sym->symbol_text= "eof"; 
	}
	cat_unref_ptr(parserContext->current_token);
	parserContext->current_token = sym;
	return sym;
}



void jag_parser_context_done_parsing(JagParserContext *parser_context) {
	parser_context->done_parsing = TRUE;
}


JagToken *jag_parser_context_peek_real(JagParserContext *parser_context) {
	int idx = cat_array_wo_size(parser_context->e_stack);
	if (idx<=0) {
		return NULL;
	}
	return (JagToken*) cat_array_wo_get(parser_context->e_stack, idx-1);
}


JagToken *jag_parser_context_pop_real(JagParserContext *parser_context) {
	int idx = cat_array_wo_size(parser_context->e_stack);
	if (idx<=0) {
		return NULL;
	}
	parser_context->tos--;
	JagToken *result = NULL;
	cat_array_wo_remove(parser_context->e_stack, idx-1, (GObject **) (&result));
	return result;
}


void jag_parser_context_push_real(JagParserContext *parser_context, JagToken *token) {
	cat_array_wo_append(parser_context->e_stack, (GObject *) token);
}




void jag_parser_context_shift_real(JagParserContext *parser_context, JagToken *token) {
	cat_array_wo_append(parser_context->e_stack, (GObject *) token);
	parser_context->tos++;
}

JagToken *jag_parser_context_get_from_top_real(JagParserContext *parser_context, int reverseIndex) {
	int idx = parser_context->tos-reverseIndex;
	if (idx<0 || idx>=cat_array_wo_size(parser_context->e_stack)) {
		return NULL;
	}

	return (JagToken *) cat_array_wo_get(parser_context->e_stack, idx);
}




JagToken *jag_parser_context_peek_at_real(JagParserContext *parser_context, int reverse_index) {
	if (reverse_index<0) {
		return NULL;
	}
	int idx = cat_array_wo_size(parser_context->e_stack)-1-reverse_index;
	
	if (idx<0) {
		return NULL;
	}
	return (JagToken*) cat_array_wo_get(parser_context->e_stack, idx);
}

void jag_parser_context_reduce_real(JagParserContext *parser_context, int replace_count, JagToken *replace_with_symbol) {
	if (replace_count>0) {
		parser_context->tos -= replace_count;
		cat_array_wo_limit(parser_context->e_stack, 0, cat_array_wo_size(parser_context->e_stack)-replace_count);
	}
	cat_array_wo_append(parser_context->e_stack, (GObject *) replace_with_symbol);
	parser_context->tos++;
	cat_unref_ptr(replace_with_symbol);
}


JagToken *jag_parser_context_current_error_token(JagParserContext *parserContext) {
	return (JagToken *) cat_array_wo_get(parserContext->e_lookahead, parserContext->lookahead_pos);
}



void jag_parser_context_debug(JagParserContext *parser_context) {
	int stackIdx;
	for(stackIdx=0; stackIdx<cat_array_wo_size(parser_context->e_stack); stackIdx++) {
		JagToken *token = (JagToken *) cat_array_wo_get(parser_context->e_stack, stackIdx);
		if (token!=NULL) {
			cat_log_debug("stack[%d]=Token[sym=%d, state=%d, text=%s]", stackIdx, token->sym, token->parse_state, token->symbol_text);
		}
	}
}

