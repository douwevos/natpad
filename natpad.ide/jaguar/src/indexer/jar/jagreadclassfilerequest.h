/*
   File:    jagreadclassfilerequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 28, 2014
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

#ifndef JAGREADCLASSFILEREQUEST_H_
#define JAGREADCLASSFILEREQUEST_H_

#include "jgijarentry.h"
#include <caterpillar.h>
#include <worm.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_READ_CLASSFILE_REQUEST              (jag_read_classfile_request_get_type())
#define JAG_READ_CLASSFILE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_read_classfile_request_get_type(), JagReadClassfileRequest))
#define JAG_READ_CLASSFILE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_READ_CLASSFILE_REQUEST, JagReadClassfileRequestClass))
#define JAG_IS_READ_CLASSFILE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_READ_CLASSFILE_REQUEST))
#define JAG_IS_READ_CLASSFILE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_READ_CLASSFILE_REQUEST))
#define JAG_READ_CLASSFILE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_READ_CLASSFILE_REQUEST, JagReadClassfileRequestClass))


typedef struct _JagReadClassfileRequest               JagReadClassfileRequest;
typedef struct _JagReadClassfileRequestPrivate        JagReadClassfileRequestPrivate;
typedef struct _JagReadClassfileRequestClass          JagReadClassfileRequestClass;


struct _JagReadClassfileRequest {
	WorRequest parent;
};

struct _JagReadClassfileRequestClass {
	WorRequestClass parent_class;
};


GType jag_read_classfile_request_get_type();

JagReadClassfileRequest *jag_read_classfile_request_new(JgiJarEntry *jar_entry, MooNodeWo *node);

G_END_DECLS

#endif /* JAGREADCLASSFILEREQUEST_H_ */
