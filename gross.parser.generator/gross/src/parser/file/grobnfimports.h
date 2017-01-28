/*
   File:    grobnfimports.h
   Project: gross
   Author:  Douwe Vos
   Date:    Nov 17, 2016
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

#ifndef PARSER_FILE_GROBNFIMPORTS_H_
#define PARSER_FILE_GROBNFIMPORTS_H_

#include "grobnfstring.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_IMPORTS              (gro_bnf_imports_get_type())
#define GRO_BNF_IMPORTS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_imports_get_type(), GroBnfImports))
#define GRO_BNF_IMPORTS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_IMPORTS, GroBnfImportsClass))
#define GRO_IS_BNF_IMPORTS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_IMPORTS))
#define GRO_IS_BNF_IMPORTS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_IMPORTS))
#define GRO_BNF_IMPORTS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_IMPORTS, GroBnfImportsClass))


typedef struct _GroBnfImports               GroBnfImports;
typedef struct _GroBnfImportsPrivate        GroBnfImportsPrivate;
typedef struct _GroBnfImportsClass          GroBnfImportsClass;


struct _GroBnfImports {
	GObject parent;
};

struct _GroBnfImportsClass {
	GObjectClass parent_class;
};


GType gro_bnf_imports_get_type();

GroBnfImports *gro_bnf_imports_new(GroBnfString *code_text);

GroBnfString *gro_bnf_imports_get_code_text(GroBnfImports *imports);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFIMPORTS_H_ */
