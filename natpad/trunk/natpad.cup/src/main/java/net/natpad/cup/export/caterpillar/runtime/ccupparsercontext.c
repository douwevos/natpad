#include "[%pre%]parsercontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "[%Pre%]ParserContext"
#include <logging/catlog.h>


G_DEFINE_TYPE([%Pre%]ParserContext, [%pre%]_parser_context, G_TYPE_OBJECT)

static void _dispose(GObject *object);



static void [%pre%]_parser_context_class_init([%Pre%]ParserContextClass *clazz) {

	clazz->shift = [%pre%]_parser_context_shift_real;
	clazz->push = [%pre%]_parser_context_push_real;
	clazz->pop = [%pre%]_parser_context_pop_real;
	clazz->peek = [%pre%]_parser_context_peek_real;
	clazz->getFromTop = [%pre%]_parser_context_get_from_top_real;
	
	clazz->scanNext = [%pre%]_parser_context_scan_next_real;
	
	clazz->reduce = [%pre%]_parser_context_reduce_real;
	clazz->peek_at = [%pre%]_parser_context_peek_at_real;
	
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void [%pre%]_parser_context_init([%Pre%]ParserContext *obj) {
}

static void _dispose(GObject *object) {
	[%Pre%]ParserContext *instance = [%PRE%]_PARSER_CONTEXT(object);
	cat_unref_ptr(instance->scanner);
	cat_unref_ptr(instance->e_stack);
	cat_unref_ptr(instance->current_token);
	cat_unref_ptr(instance->e_lookahead);
}


[%Pre%]ParserContext *[%pre%]_parser_context_new([%Pre%]IScanner *scanner) {
	[%Pre%]ParserContext *result = g_object_new([%PRE%]_TYPE_PARSER_CONTEXT, NULL);
	cat_ref_anounce(result);
	[%pre%]_parser_context_construct(result, scanner);
	return result;
}

void [%pre%]_parser_context_construct([%Pre%]ParserContext *parser_context, [%Pre%]IScanner *scanner) {
	parser_context->scanner = cat_ref_ptr(scanner);
	parser_context->e_stack = cat_array_wo_new();
	parser_context->done_parsing = FALSE;
	parser_context->tos = 0;
	parser_context->current_token = NULL;
	parser_context->e_lookahead = NULL;
}





[%Pre%]Token *[%pre%]_parser_context_scan_next_real([%Pre%]ParserContext *parserContext, int eofSymbol) {
	[%Pre%]Token *sym = [%pre%]_iscanner_next_token(parserContext->scanner);
	if (sym==NULL) {
		[%PRE%]_ISCANNER_GET_INTERFACE(parserContext->scanner)->createToken(parserContext->scanner, eofSymbol, -1, TRUE, FALSE, FALSE, 0, 0, 0, 0, NULL);
//		sym = [%pre%]_token_new_symbol(eofSymbol);
		[%if-set:symbol.info%]sym->symbol_text= "eof"; [%end-if:symbol.info%]
	}
	cat_unref_ptr(parserContext->current_token);
	parserContext->current_token = sym;
	return sym;
}



void [%pre%]_parser_context_done_parsing([%Pre%]ParserContext *parser_context) {
	parser_context->done_parsing = TRUE;
}


[%Pre%]Token *[%pre%]_parser_context_peek_real([%Pre%]ParserContext *parser_context) {
	int idx = cat_array_wo_size(parser_context->e_stack);
	if (idx<=0) {
		return NULL;
	}
	return ([%Pre%]Token*) cat_array_wo_get(parser_context->e_stack, idx-1);
}


[%Pre%]Token *[%pre%]_parser_context_pop_real([%Pre%]ParserContext *parser_context) {
	int idx = cat_array_wo_size(parser_context->e_stack);
	if (idx<=0) {
		return NULL;
	}
	parser_context->tos--;
	[%Pre%]Token *result = NULL;
	cat_array_wo_remove(parser_context->e_stack, idx-1, (GObject **) (&result));
	return result;
}


void [%pre%]_parser_context_push_real([%Pre%]ParserContext *parser_context, [%Pre%]Token *token) {
	cat_array_wo_append(parser_context->e_stack, (GObject *) token);
}




void [%pre%]_parser_context_shift_real([%Pre%]ParserContext *parser_context, [%Pre%]Token *token) {
	cat_array_wo_append(parser_context->e_stack, (GObject *) token);
	parser_context->tos++;
}

[%Pre%]Token *[%pre%]_parser_context_get_from_top_real([%Pre%]ParserContext *parser_context, int reverseIndex) {
	int idx = parser_context->tos-reverseIndex;
	if (idx<0 || idx>=cat_array_wo_size(parser_context->e_stack)) {
		return NULL;
	}

	return ([%Pre%]Token *) cat_array_wo_get(parser_context->e_stack, idx);
}




[%Pre%]Token *[%pre%]_parser_context_peek_at_real([%Pre%]ParserContext *parser_context, int reverse_index) {
	if (reverse_index<0) {
		return NULL;
	}
	int idx = cat_array_wo_size(parser_context->e_stack)-1-reverse_index;
	
	if (idx<0) {
		return NULL;
	}
	return ([%Pre%]Token*) cat_array_wo_get(parser_context->e_stack, idx);
}

void [%pre%]_parser_context_reduce_real([%Pre%]ParserContext *parser_context, int replace_count, [%Pre%]Token *replace_with_symbol) {
	if (replace_count>0) {
		parser_context->tos -= replace_count;
		cat_array_wo_limit(parser_context->e_stack, 0, cat_array_wo_size(parser_context->e_stack)-replace_count);
	}
	cat_array_wo_append(parser_context->e_stack, (GObject *) replace_with_symbol);
	parser_context->tos++;
	cat_unref_ptr(replace_with_symbol);
}


[%Pre%]Token *[%pre%]_parser_context_current_error_token([%Pre%]ParserContext *parserContext) {
	return ([%Pre%]Token *) cat_array_wo_get(parserContext->e_lookahead, parserContext->lookahead_pos);
}


[%if-set:debug%]
void [%pre%]_parser_context_debug([%Pre%]ParserContext *parser_context) {
	int stackIdx;
	for(stackIdx=0; stackIdx<cat_array_wo_size(parser_context->e_stack); stackIdx++) {
		[%Pre%]Token *token = ([%Pre%]Token *) cat_array_wo_get(parser_context->e_stack, stackIdx);
		if (token!=NULL) {
			cat_log_debug("stack[%d%]=Token[sym=%d, state=%d, text=%s]", stackIdx, token->sym, token->parse_state, token->symbol_text);
		}
	}
}
[%end-if:debug%]
