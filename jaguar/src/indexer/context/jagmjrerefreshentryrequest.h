/*
   File:    jagmjrerefreshentryrequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef JAGMJREREFRESHENTRYREQUEST_H_
#define JAGMJREREFRESHENTRYREQUEST_H_

#include "jgijreentry.h"
#include "../jar/jgijarentry.h"
#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define JAGM_TYPE_JRE_REFRESH_ENTRY_REQUEST              (jagm_jre_refresh_entry_request_get_type())
#define JAGM_JRE_REFRESH_ENTRY_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagm_jre_refresh_entry_request_get_type(), JagmJreRefreshEntryRequest))
#define JAGM_JRE_REFRESH_ENTRY_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGM_TYPE_JRE_REFRESH_ENTRY_REQUEST, JagmJreRefreshEntryRequestClass))
#define JAGM_IS_JRE_REFRESH_ENTRY_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGM_TYPE_JRE_REFRESH_ENTRY_REQUEST))
#define JAGM_IS_JRE_REFRESH_ENTRY_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGM_TYPE_JRE_REFRESH_ENTRY_REQUEST))
#define JAGM_JRE_REFRESH_ENTRY_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGM_TYPE_JRE_REFRESH_ENTRY_REQUEST, JagmJreRefreshEntryRequestClass))


typedef struct _JagmJreRefreshEntryRequest               JagmJreRefreshEntryRequest;
typedef struct _JagmJreRefreshEntryRequestPrivate        JagmJreRefreshEntryRequestPrivate;
typedef struct _JagmJreRefreshEntryRequestClass          JagmJreRefreshEntryRequestClass;


struct _JagmJreRefreshEntryRequest {
	WorRequest parent;
};

struct _JagmJreRefreshEntryRequestClass {
	WorRequestClass parent_class;
};


GType jagm_jre_refresh_entry_request_get_type();

JagmJreRefreshEntryRequest *jagm_jre_refresh_entry_request_new(JgiJreEntry *jre_entry, JgiJarEntry *jar_entry, int last_jar_entry_index);

G_END_DECLS

#endif /* JAGMJREREFRESHENTRYREQUEST_H_ */
