#include "shoparsercontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoParserContext"
#include <logging/catlog.h>


G_DEFINE_TYPE(ShoParserContext, sho_parser_context, G_TYPE_OBJECT)

static void _dispose(GObject *object);



static void sho_parser_context_class_init(ShoParserContextClass *clazz) {

	clazz->shift = sho_parser_context_shift_real;
	clazz->push = sho_parser_context_push_real;
	clazz->pop = sho_parser_context_pop_real;
	clazz->peek = sho_parser_context_peek_real;
	clazz->getFromTop = sho_parser_context_get_from_top_real;
	
	clazz->scanNext = sho_parser_context_scan_next_real;
	
	clazz->reduce = sho_parser_context_reduce_real;
	clazz->peek_at = sho_parser_context_peek_at_real;
	
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void sho_parser_context_init(ShoParserContext *obj) {
}

static void _dispose(GObject *object) {
	ShoParserContext *instance = SHO_PARSER_CONTEXT(object);
	cat_unref_ptr(instance->scanner);
	cat_unref_ptr(instance->e_stack);
	cat_unref_ptr(instance->current_token);
	cat_unref_ptr(instance->e_lookahead);
}


ShoParserContext *sho_parser_context_new(ShoIScanner *scanner) {
	ShoParserContext *result = g_object_new(SHO_TYPE_PARSER_CONTEXT, NULL);
	cat_ref_anounce(result);
	sho_parser_context_construct(result, scanner);
	return result;
}

void sho_parser_context_construct(ShoParserContext *parser_context, ShoIScanner *scanner) {
	parser_context->scanner = cat_ref_ptr(scanner);
	parser_context->e_stack = cat_array_wo_new();
	parser_context->done_parsing = FALSE;
	parser_context->tos = 0;
	parser_context->current_token = NULL;
	parser_context->e_lookahead = NULL;
}





ShoToken *sho_parser_context_scan_next_real(ShoParserContext *parserContext, int eofSymbol) {
	ShoToken *sym = sho_iscanner_next_token(parserContext->scanner);
	if (sym==NULL) {
		SHO_ISCANNER_GET_INTERFACE(parserContext->scanner)->createToken(parserContext->scanner, eofSymbol, -1, TRUE, FALSE, FALSE, 0, 0, 0, 0, NULL);
//		sym = sho_token_new_symbol(eofSymbol);
		sym->symbol_text= "eof"; 
	}
	cat_unref_ptr(parserContext->current_token);
	parserContext->current_token = sym;
	return sym;
}



void sho_parser_context_done_parsing(ShoParserContext *parser_context) {
	parser_context->done_parsing = TRUE;
}


ShoToken *sho_parser_context_peek_real(ShoParserContext *parser_context) {
	int idx = cat_array_wo_size(parser_context->e_stack);
	if (idx<=0) {
		return NULL;
	}
	return (ShoToken*) cat_array_wo_get(parser_context->e_stack, idx-1);
}


ShoToken *sho_parser_context_pop_real(ShoParserContext *parser_context) {
	int idx = cat_array_wo_size(parser_context->e_stack);
	if (idx<=0) {
		return NULL;
	}
	parser_context->tos--;
	ShoToken *result = NULL;
	cat_array_wo_remove(parser_context->e_stack, idx-1, (GObject **) (&result));
	return result;
}


void sho_parser_context_push_real(ShoParserContext *parser_context, ShoToken *token) {
	cat_array_wo_append(parser_context->e_stack, (GObject *) token);
}




void sho_parser_context_shift_real(ShoParserContext *parser_context, ShoToken *token) {
	cat_array_wo_append(parser_context->e_stack, (GObject *) token);
	parser_context->tos++;
}

ShoToken *sho_parser_context_get_from_top_real(ShoParserContext *parser_context, int reverseIndex) {
	int idx = parser_context->tos-reverseIndex;
	if (idx<0 || idx>=cat_array_wo_size(parser_context->e_stack)) {
		return NULL;
	}

	return (ShoToken *) cat_array_wo_get(parser_context->e_stack, idx);
}




ShoToken *sho_parser_context_peek_at_real(ShoParserContext *parser_context, int reverse_index) {
	if (reverse_index<0) {
		return NULL;
	}
	int idx = cat_array_wo_size(parser_context->e_stack)-1-reverse_index;
	
	if (idx<0) {
		return NULL;
	}
	return (ShoToken*) cat_array_wo_get(parser_context->e_stack, idx);
}

void sho_parser_context_reduce_real(ShoParserContext *parser_context, int replace_count, ShoToken *replace_with_symbol) {
	if (replace_count>0) {
		parser_context->tos -= replace_count;
		cat_array_wo_limit(parser_context->e_stack, 0, cat_array_wo_size(parser_context->e_stack)-replace_count);
	}
	cat_array_wo_append(parser_context->e_stack, (GObject *) replace_with_symbol);
	parser_context->tos++;
	cat_unref_ptr(replace_with_symbol);
}


ShoToken *sho_parser_context_current_error_token(ShoParserContext *parserContext) {
	return (ShoToken *) cat_array_wo_get(parserContext->e_lookahead, parserContext->lookahead_pos);
}



void sho_parser_context_debug(ShoParserContext *parser_context) {
	int stackIdx;
	for(stackIdx=0; stackIdx<cat_array_wo_size(parser_context->e_stack); stackIdx++) {
		ShoToken *token = (ShoToken *) cat_array_wo_get(parser_context->e_stack, stackIdx);
		if (token!=NULL) {
			cat_log_debug("stack[%d]=Token[sym=%d, state=%d, text=%s]", stackIdx, token->sym, token->parse_state, token->symbol_text);
		}
	}
}

