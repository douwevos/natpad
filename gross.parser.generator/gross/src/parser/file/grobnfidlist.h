/*
   File:    grobnfidlist.h
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

#ifndef PARSER_FILE_GROBNFIDLIST_H_
#define PARSER_FILE_GROBNFIDLIST_H_

#include "grobnfstring.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_ID_LIST              (gro_bnf_id_list_get_type())
#define GRO_BNF_ID_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_id_list_get_type(), GroBnfIdList))
#define GRO_BNF_ID_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_ID_LIST, GroBnfIdListClass))
#define GRO_IS_BNF_ID_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_ID_LIST))
#define GRO_IS_BNF_ID_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_ID_LIST))
#define GRO_BNF_ID_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_ID_LIST, GroBnfIdListClass))


typedef struct _GroBnfIdList               GroBnfIdList;
typedef struct _GroBnfIdListPrivate        GroBnfIdListPrivate;
typedef struct _GroBnfIdListClass          GroBnfIdListClass;


struct _GroBnfIdList {
	GObject parent;
};

struct _GroBnfIdListClass {
	GObjectClass parent_class;
};


GType gro_bnf_id_list_get_type();

GroBnfIdList *gro_bnf_id_list_new(GroBnfIdList *linked, GroBnfString *id);

CatArrayWo *gro_bnf_id_list_enlist(GroBnfIdList *list);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFIDLIST_H_ */
