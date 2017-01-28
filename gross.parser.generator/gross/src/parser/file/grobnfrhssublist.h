/*
   File:    grobnfrhssublist.h
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

#ifndef PARSER_FILE_GROBNFRHSSUBLIST_H_
#define PARSER_FILE_GROBNFRHSSUBLIST_H_

#include "grobnfrhssub.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_RHS_SUB_LIST              (gro_bnf_rhs_sub_list_get_type())
#define GRO_BNF_RHS_SUB_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_sub_list_get_type(), GroBnfRhsSubList))
#define GRO_BNF_RHS_SUB_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS_SUB_LIST, GroBnfRhsSubListClass))
#define GRO_IS_BNF_RHS_SUB_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS_SUB_LIST))
#define GRO_IS_BNF_RHS_SUB_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS_SUB_LIST))
#define GRO_BNF_RHS_SUB_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS_SUB_LIST, GroBnfRhsSubListClass))


typedef struct _GroBnfRhsSubList               GroBnfRhsSubList;
typedef struct _GroBnfRhsSubListPrivate        GroBnfRhsSubListPrivate;
typedef struct _GroBnfRhsSubListClass          GroBnfRhsSubListClass;


struct _GroBnfRhsSubList {
	GObject parent;
};

struct _GroBnfRhsSubListClass {
	GObjectClass parent_class;
};


GType gro_bnf_rhs_sub_list_get_type();

GroBnfRhsSubList *gro_bnf_rhs_sub_list_new(GroBnfRhsSubList *linked, GroBnfRhsSub *sub);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFRHSSUBLIST_H_ */
