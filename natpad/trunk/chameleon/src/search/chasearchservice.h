/*
   File:    chasearchservice.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 16, 2015
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

#ifndef SEARCH_CHASEARCHSERVICE_H_
#define SEARCH_CHASEARCHSERVICE_H_

#include "chasearchquerywo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_SEARCH_SERVICE              (cha_search_service_get_type())
#define CHA_SEARCH_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_search_service_get_type(), ChaSearchService))
#define CHA_SEARCH_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SEARCH_SERVICE, ChaSearchServiceClass))
#define CHA_IS_SEARCH_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SEARCH_SERVICE))
#define CHA_IS_SEARCH_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SEARCH_SERVICE))
#define CHA_SEARCH_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SEARCH_SERVICE, ChaSearchServiceClass))


typedef struct _ChaSearchService               ChaSearchService;
typedef struct _ChaSearchServicePrivate        ChaSearchServicePrivate;
typedef struct _ChaSearchServiceClass          ChaSearchServiceClass;


struct _ChaSearchService {
	GObject parent;
};

struct _ChaSearchServiceClass {
	GObjectClass parent_class;
};


GType cha_search_service_get_type();

void cha_search_service_construct(ChaSearchService *service);

ChaSearchService *cha_search_service_new();

struct _ChaEditor;

void cha_search_service_set_editor(ChaSearchService *search_service, struct _ChaEditor *editor);

void cha_search_service_set_query(ChaSearchService *search_service, ChaSearchQueryWo *ca_query);
ChaSearchQueryWo *cha_search_service_get_query(ChaSearchService *search_service);

void cha_search_service_find(ChaSearchService *search_service, gboolean backward_search, gboolean wrap_search);

void cha_search_service_mark(ChaSearchService *search_service);

void cha_search_service_show_dialog(ChaSearchService *search_service, GtkWindow *owner, struct _ChaEditor *editor);

G_END_DECLS

#endif /* SEARCH_CHASEARCHSERVICE_H_ */
