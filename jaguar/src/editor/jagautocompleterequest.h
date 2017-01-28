///*
//   File:    jagautocompleterequest.h
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Jun 16, 2014
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2014 Douwe Vos.
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
//#ifndef JAGAUTOCOMPLETEREQUEST_H_
//#define JAGAUTOCOMPLETEREQUEST_H_
//
//#include <caterpillar.h>
//#include <elk.h>
//#include <elkdeprecated.h>
//
//
//G_BEGIN_DECLS
//
//#define JAG_TYPE_AUTO_COMPLETE_REQUEST              (jag_auto_complete_request_get_type())
//#define JAG_AUTO_COMPLETE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_auto_complete_request_get_type(), JagAutoCompleteRequest))
//#define JAG_AUTO_COMPLETE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AUTO_COMPLETE_REQUEST, JagAutoCompleteRequestClass))
//#define JAG_IS_AUTO_COMPLETE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AUTO_COMPLETE_REQUEST))
//#define JAG_IS_AUTO_COMPLETE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AUTO_COMPLETE_REQUEST))
//#define JAG_AUTO_COMPLETE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AUTO_COMPLETE_REQUEST, JagAutoCompleteRequestClass))
//#define JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_AUTO_COMPLETE_REQUEST, JagAutoCompleteRequestPrivate))
//
//
//typedef struct _JagAutoCompleteRequest               JagAutoCompleteRequest;
//typedef struct _JagAutoCompleteRequestPrivate        JagAutoCompleteRequestPrivate;
//typedef struct _JagAutoCompleteRequestClass          JagAutoCompleteRequestClass;
//
//
//struct _JagAutoCompleteRequest {
//	WorRequest parent;
//	JagAutoCompleteRequestPrivate *priv;
//};
//
//struct _JagAutoCompleteRequestClass {
//	WorRequestClass parent_class;
//};
//
//
//GType jag_auto_complete_request_get_type();
//
//JagAutoCompleteRequest *jag_auto_complete_request_new(ElkEditor *elk_editor);
//
//G_END_DECLS
//
//#endif /* JAGAUTOCOMPLETEREQUEST_H_ */
