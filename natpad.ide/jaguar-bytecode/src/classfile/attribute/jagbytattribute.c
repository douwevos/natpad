/*
   File:    jagbytattribute.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Mar 6, 2012
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


#include "jagbytattribute.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttribute"
#include <logging/catlog.h>


G_DEFINE_TYPE (JagBytAttribute, jag_byt_attribute, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_attribute_class_init(JagBytAttributeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_attribute_init(JagBytAttribute *instance) {
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytAttribute *instance = JAG_BYT_ATTRIBUTE(object);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_byt_attribute_construct(JagBytAttribute *attribute) {
}







