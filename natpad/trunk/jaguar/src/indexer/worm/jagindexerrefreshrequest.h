///*
//   File:    jagindexerrefreshrequest.h
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Aug 16, 2012
//   Web:     http://www.natpad.net/
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2012 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//
//#ifndef JAGINDEXERREFRESHREQUEST_H_
//#define JAGINDEXERREFRESHREQUEST_H_
//
//#include <caterpillar.h>
//#include <worm.h>
//#include "../jagindexer.h"
//
//G_BEGIN_DECLS
//
//#define JAG_TYPE_INDEXER_REFRESH_REQUEST              (jag_indexer_refresh_request_get_type())
//#define JAG_INDEXER_REFRESH_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_indexer_refresh_request_get_type(), JagIndexerRefreshRequest))
//#define JAG_INDEXER_REFRESH_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEXER_REFRESH_REQUEST, JagIndexerRefreshRequestClass))
//#define JAG_IS_INDEXER_REFRESH_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEXER_REFRESH_REQUEST))
//#define JAG_IS_INDEXER_REFRESH_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEXER_REFRESH_REQUEST))
//#define JAG_INDEXER_REFRESH_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEXER_REFRESH_REQUEST, JagIndexerRefreshRequestClass))
//#define JAG_INDEXER_REFRESH_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_INDEXER_REFRESH_REQUEST, JagIndexerRefreshRequestPrivate))
//
//typedef struct _JagIndexerRefreshRequest               JagIndexerRefreshRequest;
//typedef struct _JagIndexerRefreshRequestPrivate        JagIndexerRefreshRequestPrivate;
//typedef struct _JagIndexerRefreshRequestClass          JagIndexerRefreshRequestClass;
//
//
//struct _JagIndexerRefreshRequest {
//	WorRequest parent;
//	JagIndexerRefreshRequestPrivate *priv;
//};
//
//struct _JagIndexerRefreshRequestClass {
//	WorRequestClass parent_class;
//};
//
//
//GType jag_indexer_refresh_request_get_type();
//
//JagIndexerRefreshRequest *jag_indexer_refresh_request_new(JagIndexer *indexer);
//
//WorCondition *jag_indexer_refresh_request_get_condition(JagIndexerRefreshRequest *refresh_request);
//
//
//gboolean jag_indexer_refresh_request_is_running(JagIndexerRefreshRequest *request);
//
//void jag_indexer_refresh_request_stop(JagIndexerRefreshRequest *request);
//
//
//void jag_indexer_refresh_request_report_has_stopped(JagIndexerRefreshRequest *request);
//
//G_END_DECLS
//
//
//#endif /* JAGINDEXERREFRESHREQUEST_H_ */
