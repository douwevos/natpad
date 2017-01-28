#include "smplparsercontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SmplParserContext"
#include <logging/catlog.h>


G_DEFINE_TYPE(SmplParserContext, smpl_parser_context, G_TYPE_OBJECT)

static void _dispose(GObject *object);



static void smpl_parser_context_class_init(SmplParserContextClass *clazz) {

	clazz->shift = smpl_parser_context_shift_real;
	clazz->push = smpl_parser_context_push_real;
	clazz->pop = smpl_parser_context_pop_real;
	clazz->peek = smpl_parser_context_peek_real;
	clazz->getFromTop = smpl_parser_context_get_from_top_real;
	
	clazz->scanNext = smpl_parser_context_scan_next_real;
	
	clazz->reduce = smpl_parser_context_reduce_real;
	clazz->peek_at = smpl_parser_context_peek_at_real;
	
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void smpl_parser_context_init(SmplParserContext *obj) {
}

static void _dispose(GObject *object) {
	SmplParserContext *instance = SMPL_PARSER_CONTEXT(object);
	cat_unref_ptr(instance->scanner);
	cat_unref_ptr(instance->stack);
	cat_unref_ptr(instance->current_token);
	cat_unref_ptr(instance->lookahead);
}


SmplParserContext *smpl_parser_context_new(SmplIScanner *scanner) {
	SmplParserContext *result = g_object_new(SMPL_TYPE_PARSER_CONTEXT, NULL);
	cat_ref_anounce(result);
	smpl_parser_context_construct(result, scanner);
	return result;
}

void smpl_parser_context_construct(SmplParserContext *parser_context, SmplIScanner *scanner) {
	parser_context->scanner = cat_ref_ptr(scanner);
	parser_context->stack = cat_array_new();
	parser_context->done_parsing = FALSE;
	parser_context->tos = 0;
	parser_context->current_token = NULL;
	parser_context->lookahead = NULL;
}





SmplToken *smpl_parser_context_scan_next_real(SmplParserContext *parserContext, int eofSymbol) {
	SmplToken *sym = smpl_iscanner_next_token(parserContext->scanner);
	if (sym==NULL) {
		SMPL_ISCANNER_GET_INTERFACE(parserContext->scanner)->createToken(parserContext->scanner, eofSymbol, -1, TRUE, FALSE, FALSE, 0, 0, 0, 0, NULL);
//		sym = smpl_token_new_symbol(eofSymbol);
		
	}
	cat_unref_ptr(parserContext->current_token);
	parserContext->current_token = sym;
	return sym;
}



void smpl_parser_context_done_parsing(SmplParserContext *parser_context) {
	parser_context->done_parsing = TRUE;
}


SmplToken *smpl_parser_context_peek_real(SmplParserContext *parser_context) {
	int idx = cat_array_size(parser_context->stack);
	if (idx<=0) {
		return NULL;
	}
	return (SmplToken*) cat_array_get(parser_context->stack, idx-1);
}


SmplToken *smpl_parser_context_pop_real(SmplParserContext *parser_context) {
	int idx = cat_array_size(parser_context->stack);
	if (idx<=0) {
		return NULL;
	}
	parser_context->tos--;
	SmplToken *result = NULL;
	cat_array_remove(parser_context->stack, idx-1, (GObject **) (&result));
	return result;
}


void smpl_parser_context_push_real(SmplParserContext *parser_context, SmplToken *token) {
	cat_array_append(parser_context->stack, (GObject *) token);
}




void smpl_parser_context_shift_real(SmplParserContext *parser_context, SmplToken *token) {
	cat_array_append(parser_context->stack, (GObject *) token);
	parser_context->tos++;
}

SmplToken *smpl_parser_context_get_from_top_real(SmplParserContext *parser_context, int reverseIndex) {
	int idx = parser_context->tos-reverseIndex;
	if (idx<0 || idx>=cat_array_size(parser_context->stack)) {
		return NULL;
	}

	return (SmplToken *) cat_array_get(parser_context->stack, idx);
}




SmplToken *smpl_parser_context_peek_at_real(SmplParserContext *parser_context, int reverse_index) {
	if (reverse_index<0) {
		return NULL;
	}
	int idx = cat_array_size(parser_context->stack)-1-reverse_index;
	
	if (idx<0) {
		return NULL;
	}
	return (SmplToken*) cat_array_get(parser_context->stack, idx);
}

void smpl_parser_context_reduce_real(SmplParserContext *parser_context, int replace_count, SmplToken *replace_with_symbol) {
	if (replace_count>0) {
		parser_context->tos -= replace_count;
		cat_array_set_size(parser_context->stack, cat_array_size(parser_context->stack)-replace_count);
	}
	cat_array_append(parser_context->stack, (GObject *) replace_with_symbol);
	parser_context->tos++;
	cat_unref_ptr(replace_with_symbol);
}


SmplToken *smpl_parser_context_current_error_token(SmplParserContext *parserContext) {
	return (SmplToken *) cat_array_get(parserContext->lookahead, parserContext->lookahead_pos);
}



