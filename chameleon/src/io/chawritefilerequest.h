/*
   File:    chawritefilerequest.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 28, 2015
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

#ifndef IO_CHAWRITEFILEREQUEST_H_
#define IO_CHAWRITEFILEREQUEST_H_

#include "chaioasync.h"
#include "../document/chadocument.h"
#include <caterpillar.h>
#include <worm.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define CHA_TYPE_WRITE_FILE_REQUEST              (cha_write_file_request_get_type())
#define CHA_WRITE_FILE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_write_file_request_get_type(), ChaWriteFileRequest))
#define CHA_WRITE_FILE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_WRITE_FILE_REQUEST, ChaWriteFileRequestClass))
#define CHA_IS_WRITE_FILE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_WRITE_FILE_REQUEST))
#define CHA_IS_WRITE_FILE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_WRITE_FILE_REQUEST))
#define CHA_WRITE_FILE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_WRITE_FILE_REQUEST, ChaWriteFileRequestClass))


typedef struct _ChaWriteFileRequest               ChaWriteFileRequest;
typedef struct _ChaWriteFileRequestPrivate        ChaWriteFileRequestPrivate;
typedef struct _ChaWriteFileRequestClass          ChaWriteFileRequestClass;


struct _ChaWriteFileRequest {
	WorRequest parent;
};

struct _ChaWriteFileRequestClass {
	WorRequestClass parent_class;
};


GType cha_write_file_request_get_type();

ChaWriteFileRequest *cha_write_file_request_new(ChaDocument *document, GFile *file, ChaIConverter *output_converter, ChaIOAsync *async, gboolean create_backup);

G_END_DECLS

#endif /* IO_CHAWRITEFILEREQUEST_H_ */
