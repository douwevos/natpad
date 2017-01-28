
/*
   File:    grobnfstring.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 20, 2016
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

#ifndef PARSER_BNF_GROBNFSTRING_H_
#define PARSER_BNF_GROBNFSTRING_H_

#include "../grolocation.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_STRING              (gro_bnf_string_get_type())
#define GRO_BNF_STRING(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_string_get_type(), GroBnfString))
#define GRO_BNF_STRING_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_STRING, GroBnfStringClass))
#define GRO_IS_BNF_STRING(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_STRING))
#define GRO_IS_BNF_STRING_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_STRING))
#define GRO_BNF_STRING_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_STRING, GroBnfStringClass))


typedef struct _GroBnfString               GroBnfString;
typedef struct _GroBnfStringPrivate        GroBnfStringPrivate;
typedef struct _GroBnfStringClass          GroBnfStringClass;


struct _GroBnfString {
	GObject parent;
};

struct _GroBnfStringClass {
	GObjectClass parent_class;
};


GType gro_bnf_string_get_type();

GroBnfString *gro_bnf_string_new(const CatStringWo *text, GroLocation *start_location, GroLocation *end_location);

CatStringWo *gro_bnf_string_get_text(GroBnfString *bnf_string);

GroLocation *gro_bnf_string_get_start(GroBnfString *bnf_string);

GroLocation *gro_bnf_string_get_end(GroBnfString *bnf_string);

int gro_bnf_string_hash(const GroBnfString *bnf_string);

gboolean gro_bnf_string_equal(const GroBnfString *bnf_string_a, const GroBnfString *bnf_string_b);

G_END_DECLS

#endif /* PARSER_BNF_GROBNFSTRING_H_ */
