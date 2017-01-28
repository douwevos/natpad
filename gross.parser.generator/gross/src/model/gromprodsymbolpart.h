
/*
   File:    gromprodsymbolpart.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 26, 2016
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

#ifndef PARSER_MODEL_GROMPRODSYMBOLPART_H_
#define PARSER_MODEL_GROMPRODSYMBOLPART_H_

#include "../parser/file/grobnfstring.h"
#include "gromsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_M_PROD_SYMBOL_PART              (gro_m_prod_symbol_part_get_type())
#define GRO_M_PROD_SYMBOL_PART(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_m_prod_symbol_part_get_type(), GroMProdSymbolPart))
#define GRO_M_PROD_SYMBOL_PART_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_M_PROD_SYMBOL_PART, GroMProdSymbolPartClass))
#define GRO_IS_M_PROD_SYMBOL_PART(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_M_PROD_SYMBOL_PART))
#define GRO_IS_M_PROD_SYMBOL_PART_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_M_PROD_SYMBOL_PART))
#define GRO_M_PROD_SYMBOL_PART_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_M_PROD_SYMBOL_PART, GroMProdSymbolPartClass))


typedef struct _GroMProdSymbolPart               GroMProdSymbolPart;
typedef struct _GroMProdSymbolPartPrivate        GroMProdSymbolPartPrivate;
typedef struct _GroMProdSymbolPartClass          GroMProdSymbolPartClass;


struct _GroMProdSymbolPart {
	GObject parent;
};

struct _GroMProdSymbolPartClass {
	GObjectClass parent_class;
};


GType gro_m_prod_symbol_part_get_type();

GroMProdSymbolPart *gro_m_prod_symbol_part_new(GroMSymbol *symbol, GroBnfString *label);

GroMSymbol *gro_m_prod_symbol_part_get_symbol(GroMProdSymbolPart *part);
GroBnfString *gro_m_prod_symbol_part_get_label(GroMProdSymbolPart *part);

G_END_DECLS

#endif /* PARSER_MODEL_GROMPRODSYMBOLPART_H_ */
