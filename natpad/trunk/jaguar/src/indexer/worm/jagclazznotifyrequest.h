///*
//   File:    jagclazznotifyrequest.h
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Aug 21, 2012
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
//#ifndef JAGCLAZZNOTIFYREQUEST_H_
//#define JAGCLAZZNOTIFYREQUEST_H_
//
//
//#include <caterpillar.h>
//#include <worm.h>
//#include "../jagclazznotify.h"
//#include "../model/jagnamemap.h"
//
//G_BEGIN_DECLS
//
//#define JAG_TYPE_CLAZZ_NOTIFY_REQUEST              (jag_clazz_notify_request_get_type())
//#define JAG_CLAZZ_NOTIFY_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_clazz_notify_request_get_type(), JagClazzNotifyRequest))
//#define JAG_CLAZZ_NOTIFY_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_CLAZZ_NOTIFY_REQUEST, JagClazzNotifyRequestClass))
//#define JAG_IS_CLAZZ_NOTIFY_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_CLAZZ_NOTIFY_REQUEST))
//#define JAG_IS_CLAZZ_NOTIFY_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_CLAZZ_NOTIFY_REQUEST))
//#define JAG_CLAZZ_NOTIFY_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_CLAZZ_NOTIFY_REQUEST, JagClazzNotifyRequestClass))
//#define JAG_CLAZZ_NOTIFY_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_CLAZZ_NOTIFY_REQUEST, JagClazzNotifyRequestPrivate))
//
//typedef struct _JagClazzNotifyRequest               JagClazzNotifyRequest;
//typedef struct _JagClazzNotifyRequestPrivate        JagClazzNotifyRequestPrivate;
//typedef struct _JagClazzNotifyRequestClass          JagClazzNotifyRequestClass;
//
//
//struct _JagClazzNotifyRequest {
//	WorRequest parent;
//	JagClazzNotifyRequestPrivate *priv;
//};
//
//struct _JagClazzNotifyRequestClass {
//	WorRequestClass parent_class;
//};
//
//
//GType jag_clazz_notify_request_get_type();
//
//JagClazzNotifyRequest *jag_clazz_notify_request_new(JagClazzNotify *clazz_notify);
//
//G_END_DECLS
//
//
//#endif /* JAGCLAZZNOTIFYREQUEST_H_ */
