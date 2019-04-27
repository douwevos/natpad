/*
   File:    shoparser.c
   Project: shoveler
   Author:  Douwe Vos
   Date:    May 7, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "shoparser.h"
#include "shoscanner.h"
#include "generated/sholrparser.h"
#include "generated/runtime/shoparsercontext.h"
#include "generated/runtime/shoiscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoParser"
#include <logging/catlog.h>


G_DEFINE_TYPE (ShoParser, sho_parser, G_TYPE_OBJECT) // @suppress("Unused static function")

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_parser_class_init(ShoParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_parser_init(ShoParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(sho_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoParser *sho_parser_new() {
	ShoParser *result = g_object_new(SHO_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
	return result;
}

ShoModel *sho_parser_parse(ShoParser *parser, CatIInputStream *stream_in) {
	ShoModel *result = NULL;
	ShoScanner *scanner = sho_scanner_new(stream_in);
	ShoParserContext *parser_context = sho_parser_context_new(SHO_ISCANNER(scanner));
	ShoLrParser *lr_parser = sho_lr_parser_new(SHO_ISCANNER(scanner));
	ShoToken *end_token = sho_parser_base_parse(SHO_PARSER_BASE(lr_parser), parser_context);
	if (end_token) {
		if (SHO_IS_MODEL(end_token->value)) {
			result = SHO_MODEL(end_token->value);
			cat_ref_ptr(result);
		}
	}
	cat_unref_ptr(lr_parser);
	cat_unref_ptr(parser_context);
	cat_unref_ptr(scanner);
	return result;
}
