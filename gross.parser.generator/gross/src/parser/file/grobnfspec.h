
/*
   File:    grobnfspec.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 22, 2016
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

#ifndef PARSER_FILE_GROBNFSPEC_H_
#define PARSER_FILE_GROBNFSPEC_H_

#include "grobnfterminaldeclarationlist.h"
#include "grobnfproductiondefinitionlist.h"
#include "grobnfimports.h"
#include "grobnftypedefinitionlist.h"
#include "grobnfcodedefinitionlist.h"
#include "grobnfstring.h"
#include "../../model/gromodel.h"
#include "../groimessagehandler.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_SPEC              (gro_bnf_spec_get_type())
#define GRO_BNF_SPEC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_spec_get_type(), GroBnfSpec))
#define GRO_BNF_SPEC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_SPEC, GroBnfSpecClass))
#define GRO_IS_BNF_SPEC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_SPEC))
#define GRO_IS_BNF_SPEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_SPEC))
#define GRO_BNF_SPEC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_SPEC, GroBnfSpecClass))


typedef struct _GroBnfSpec               GroBnfSpec;
typedef struct _GroBnfSpecPrivate        GroBnfSpecPrivate;
typedef struct _GroBnfSpecClass          GroBnfSpecClass;


struct _GroBnfSpec {
	GObject parent;
};

struct _GroBnfSpecClass {
	GObjectClass parent_class;
};


GType gro_bnf_spec_get_type();

GroBnfSpec *gro_bnf_spec_new(GroBnfTerminalDeclarationList *term_decl_list, GroBnfProductionDefinitionList *prod_def_list, GroBnfImports *imports, GroBnfTypeDefinitionList *type_def_list, GroBnfCodeDefinitionList *code_def_list);

GroModel *gro_bnf_spec_create_model(GroIMessageHandler *msg_handler, GroBnfSpec *spec);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFSPEC_H_ */
