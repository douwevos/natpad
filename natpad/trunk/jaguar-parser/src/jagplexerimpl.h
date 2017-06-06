/*
   File:    jagplexerimpl.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 31, 2017
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

#ifndef JAGPLEXERIMPL_H_
#define JAGPLEXERIMPL_H_

#include <caterpillar.h>
#include "jagptoken.h"
#include "jagpscanner.h"

G_BEGIN_DECLS

#define JAGP_TYPE_LEXER_IMPL              (jagp_lexer_impl_get_type())
#define JAGP_LEXER_IMPL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_lexer_impl_get_type(), JagPLexerImpl))
#define JAGP_LEXER_IMPL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_LEXER_IMPL, JagPLexerImplClass))
#define JAGP_IS_LEXER_IMPL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_LEXER_IMPL))
#define JAGP_IS_LEXER_IMPL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_LEXER_IMPL))
#define JAGP_LEXER_IMPL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_LEXER_IMPL, JagPLexerImplClass))


typedef struct _JagPLexerImpl               JagPLexerImpl;
typedef struct _JagPLexerImplPrivate        JagPLexerImplPrivate;
typedef struct _JagPLexerImplClass          JagPLexerImplClass;


struct _JagPLexerImpl {
	GObject parent;
};

struct _JagPLexerImplClass {
	GObjectClass parent_class;
};


GType jagp_lexer_impl_get_type();

JagPLexerImpl *jagp_lexer_impl_new(JagPScanner *scanner);

G_END_DECLS

#endif /* JAGPLEXERIMPL_H_ */
