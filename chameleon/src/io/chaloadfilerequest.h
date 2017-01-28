/*
   File:    chaloadfilerequest.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 26, 2015
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

#ifndef CHALOADFILEREQUEST_H_
#define CHALOADFILEREQUEST_H_

#include "../document/chadocument.h"
#include <caterpillar.h>
#include <worm.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define CHA_TYPE_LOAD_FILE_REQUEST              (cha_load_file_request_get_type())
#define CHA_LOAD_FILE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_load_file_request_get_type(), ChaLoadFileRequest))
#define CHA_LOAD_FILE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_LOAD_FILE_REQUEST, ChaLoadFileRequestClass))
#define CHA_IS_LOAD_FILE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_LOAD_FILE_REQUEST))
#define CHA_IS_LOAD_FILE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_LOAD_FILE_REQUEST))
#define CHA_LOAD_FILE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_LOAD_FILE_REQUEST, ChaLoadFileRequestClass))


typedef struct _ChaLoadFileRequest               ChaLoadFileRequest;
typedef struct _ChaLoadFileRequestPrivate        ChaLoadFileRequestPrivate;
typedef struct _ChaLoadFileRequestClass          ChaLoadFileRequestClass;


struct _ChaLoadFileRequest {
	WorRequest parent;
};

struct _ChaLoadFileRequestClass {
	WorRequestClass parent_class;
};


GType cha_load_file_request_get_type();

ChaLoadFileRequest *cha_load_file_request_new(ChaDocument *document, GFile *file);

G_END_DECLS

#endif /* CHALOADFILEREQUEST_H_ */
