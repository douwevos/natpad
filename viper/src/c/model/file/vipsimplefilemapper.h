/*
   File:    vipsimplefilemapper.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef VIPSIMPLEFILEMAPPER_H_
#define VIPSIMPLEFILEMAPPER_H_

#include "../../vipisequence.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_SIMPLE_FILE_MAPPER              (vip_simple_file_mapper_get_type())
#define VIP_SIMPLE_FILE_MAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_simple_file_mapper_get_type(), VipSimpleFileMapper))
#define VIP_SIMPLE_FILE_MAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_SIMPLE_FILE_MAPPER, VipSimpleFileMapperClass))
#define VIP_IS_SIMPLE_FILE_MAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_SIMPLE_FILE_MAPPER))
#define VIP_IS_SIMPLE_FILE_MAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_SIMPLE_FILE_MAPPER))
#define VIP_SIMPLE_FILE_MAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_SIMPLE_FILE_MAPPER, VipSimpleFileMapperClass))

typedef struct _VipSimpleFileMapper               VipSimpleFileMapper;
typedef struct _VipSimpleFileMapperPrivate        VipSimpleFileMapperPrivate;
typedef struct _VipSimpleFileMapperClass          VipSimpleFileMapperClass;

struct _VipSimpleFileMapper {
	GObject parent;
};

struct _VipSimpleFileMapperClass {
	GObjectClass parent_class;
};


GType vip_simple_file_mapper_get_type();

VipSimpleFileMapper *vip_simple_file_mapper_new(VipISequence *sequence);

G_END_DECLS

#endif /* VIPSIMPLEFILEMAPPER_H_ */
