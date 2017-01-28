/*
   File:    chasearchquerywo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 9, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef SEARCH_CHASEARCHQUERYWO_H_
#define SEARCH_CHASEARCHQUERYWO_H_

#include "../chaeditor.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_SEARCH_QUERY_WO              (cha_search_query_wo_get_type())
#define CHA_SEARCH_QUERY_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_search_query_wo_get_type(), ChaSearchQueryWo))
#define CHA_SEARCH_QUERY_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SEARCH_QUERY_WO, ChaSearchQueryWoClass))
#define CHA_IS_SEARCH_QUERY_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SEARCH_QUERY_WO))
#define CHA_IS_SEARCH_QUERY_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SEARCH_QUERY_WO))
#define CHA_SEARCH_QUERY_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SEARCH_QUERY_WO, ChaSearchQueryWoClass))


typedef struct _ChaSearchQueryWo               ChaSearchQueryWo;
typedef struct _ChaSearchQueryWoPrivate        ChaSearchQueryWoPrivate;
typedef struct _ChaSearchQueryWoClass          ChaSearchQueryWoClass;


struct _ChaSearchQueryWo {
	CatWo parent;
};

struct _ChaSearchQueryWoClass {
	CatWoClass parent_class;
};


GType cha_search_query_wo_get_type();

ChaSearchQueryWo *cha_search_query_wo_new();

CatStringWo *cha_search_query_wo_get_text(const ChaSearchQueryWo *query);

gboolean cha_search_query_wo_get_regexp(const ChaSearchQueryWo *query);

gboolean cha_search_query_wo_get_match_case(const ChaSearchQueryWo *query);
gboolean cha_search_query_wo_get_match_words(const ChaSearchQueryWo *query);

void cha_search_query_wo_set_text(ChaSearchQueryWo *e_query, CatStringWo *text);

void cha_search_query_wo_set_regexp(ChaSearchQueryWo *e_query, gboolean regexp);

void cha_search_query_wo_set_match_case(ChaSearchQueryWo *e_query, gboolean match_case);
void cha_search_query_wo_set_match_words(ChaSearchQueryWo *e_query, gboolean match_case);

//CatArrayWo *cha_search_query_wo_run(const ChaSearchQueryWo *query, ChaEditor *editor);

CAT_WO_BASE_H(ChaSearchQueryWo,cha_search_query_wo);

G_END_DECLS

#endif /* SEARCH_CHASEARCHQUERYWO_H_ */
