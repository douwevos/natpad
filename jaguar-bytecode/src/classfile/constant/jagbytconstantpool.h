/*
   File:    jagbytconstantpool.h
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

#ifndef JAGBYTCONSTANTPOOL_H_
#define JAGBYTCONSTANTPOOL_H_

#include <caterpillar.h>

#include "jagbyticonstant.h"
#include "jagbytconstantutf8text.h"
#include "jagbytconstantint.h"
#include "jagbytconstantfloat.h"
#include "jagbytconstantlong.h"
#include "jagbytconstantdouble.h"
#include "jagbytconstantclazz.h"
#include "jagbytconstantstring.h"
#include "jagbytconstantfieldref.h"
#include "jagbytconstantmethodref.h"
#include "jagbytconstantinterfacemethodref.h"
#include "jagbytconstantnameandtp.h"

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_POOL              (jag_byt_constant_pool_get_type())
#define JAG_BYT_CONSTANT_POOL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_pool_get_type(), JagBytConstantPool))
#define JAG_BYT_CONSTANT_POOL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_POOL, JagBytConstantPoolClass))
#define JAG_BYT_IS_CONSTANT_POOL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_POOL))
#define JAG_BYT_IS_CONSTANT_POOL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_POOL))
#define JAG_BYT_CONSTANT_POOL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_POOL, JagBytConstantPoolClass))

typedef struct _JagBytConstantPool               JagBytConstantPool;
typedef struct _JagBytConstantPoolPrivate        JagBytConstantPoolPrivate;
typedef struct _JagBytConstantPoolClass          JagBytConstantPoolClass;


struct _JagBytConstantPool {
	GInitiallyUnowned parent;
};

struct _JagBytConstantPoolClass {
	GInitiallyUnownedClass parent_class;
};


GType jag_byt_constant_pool_get_type();

JagBytConstantPool *jag_byt_constant_pool_new(CatArrayWo *a_new_pool);

void jag_byt_constant_pool_set_this_class_index(JagBytConstantPool *pool, int this_class_index);

int jag_byt_constant_pool_size(JagBytConstantPool *pool);

JagBytIConstant *jag_byt_constant_pool_get_this_class_constant(JagBytConstantPool *pool);

// deprecated
JagBytIConstant *jag_byt_constant_pool_get_at(JagBytConstantPool *pool, int index);

JagBytIConstant *jag_byt_constant_pool_get_unresolved(JagBytConstantPool *pool, int index);

JagBytIConstant *jag_byt_constant_pool_get_resolved(JagBytConstantPool *pool, int index);

gboolean jag_byt_constant_pool_resolve(JagBytConstantPool *pool, JagBytIConstant *constant);

CatStringWo *jag_byt_descriptor_as_source(CatStringWo *a_descriptor, CatStringWo *e_source, int in_idx, int *out_idx);

G_END_DECLS

#endif /* JAGBYTCONSTANTPOOL_H_ */
