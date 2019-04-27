/*
   File:    jagbytclassfile.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef JAGBYTCLASSFILE_H_
#define JAGBYTCLASSFILE_H_

#include "jagbytdefines.h"
#include "constant/jagbytconstantpool.h"
#include "jagbytfieldlist.h"
#include "jagbytmethodlist.h"
#include "jagbytinterfacelist.h"
#include "jagbytname.h"
#include "attribute/jagbytattributemap.h"
#include "../ast/jagastmodifiers.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CLASSFILE              (jag_byt_classfile_get_type())
#define JAG_BYT_CLASSFILE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_classfile_get_type(), JagBytClassfile))
#define JAG_BYT_CLASSFILE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CLASSFILE, JagBytClassfileClass))
#define JAG_BYT_IS_CLASSFILE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CLASSFILE))
#define JAG_BYT_IS_CLASSFILE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CLASSFILE))
#define JAG_BYT_CLASSFILE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CLASSFILE, JagBytClassfileClass))

typedef struct _JagBytClassfile               JagBytClassfile;
typedef struct _JagBytClassfilePrivate        JagBytClassfilePrivate;
typedef struct _JagBytClassfileClass          JagBytClassfileClass;


struct _JagBytClassfile {
	GObject parent;
};

struct _JagBytClassfileClass {
	GObjectClass parent_class;
};


GType jag_byt_classfile_get_type();

JagBytClassfile *jag_byt_classfile_new(uint32_t magicNr, uint16_t minorVersion, uint16_t majorVersion,
			JagBytConstantPool *constant_pool, uint16_t accessFlags, int thisClass, int superClass,
			int interfaceCount, uint16_t *interfaces, JagBytInterfaceList *interface_list, JagBytFieldList *fieldList, JagBytMethodList *methodList,
			JagBytAttributeMap *attribute_map);


JagAstModifiers *jag_byt_classfile_get_modifiers(JagBytClassfile *classfile);

JagBytName *jag_byt_classfile_get_main_type_name(JagBytClassfile *classfile);
JagBytName *jag_byt_classfile_get_super_type_name(JagBytClassfile *classfile);

JagBytInterfaceList *jag_byt_classfile_get_interface_list(JagBytClassfile *classfile);

JagBytFieldList *jag_byt_classfile_get_field_list(JagBytClassfile *classfile);

JagBytMethodList *jag_byt_classfile_get_method_list(JagBytClassfile *classfile);

JagBytConstantPool *jag_byt_classfile_get_constant_pool(JagBytClassfile *classfile);


gboolean jag_byt_classfile_resolve_field_details(JagBytClassfile *classfile, JagBytField *field, CatStringWo *a_field_name, CatStringWo *a_field_descriptor, uint16_t *field_access_flags);

CatStringWo *jag_byt_classfile_get_fq_classname(JagBytClassfile *classfile);
CatStringWo *jag_byt_classfile_get_fq_super_classname(JagBytClassfile *classfile);

G_END_DECLS

#endif /* JAGBYTCLASSFILE_H_ */
