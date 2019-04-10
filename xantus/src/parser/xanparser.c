/*
   File:    xanparser.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 17, 2017
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

#include "xanparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "XanParser"
#include <logging/catlog.h>

struct _XanParserPrivate {
	XanScanner *scanner;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(XanParser, xan_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(XanParser)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void xan_parser_class_init(XanParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void xan_parser_init(XanParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	XanParser *instance = XAN_PARSER(object);
	XanParserPrivate *priv = xan_parser_get_instance_private(instance);
	cat_unref_ptr(priv->scanner);
	G_OBJECT_CLASS(xan_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


XanParser *xan_parser_new(XanScanner *scanner) {
	XanParser *result = g_object_new(XAN_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
	XanParserPrivate *priv = xan_parser_get_instance_private(result);
	priv->scanner = cat_ref_ptr(scanner);
	return result;
}



void xan_parser_parse(XanParser *parser) {
	XanParserPrivate *priv = xan_parser_get_instance_private(parser);

	gboolean keep_scanning = TRUE;

	while(keep_scanning) {
		GroRunIToken *token = xan_scanner_next(priv->scanner);
		if (token == NULL) {
			break;
		}
		GroRunSymbol *symbol = grorun_itoken_get_symbol(token);
		int sym_id = grorun_symbol_get_id(symbol);

		switch(sym_id) {
			case XAN_SYM_END_OF_INPUT : {
				keep_scanning = FALSE;
				break;
			}
			case XAN_SYM_ELEMENT_OPEN :
			case XAN_SYM_ELEMENT_CLOSE : {
			}
		}

	}

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

