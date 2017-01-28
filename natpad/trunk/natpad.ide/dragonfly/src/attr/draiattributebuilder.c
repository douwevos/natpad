/*
   File:    draiattributebuilder.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 8, 2015
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

#include "draiattributebuilder.h"

G_DEFINE_INTERFACE(DraIAttributeBuilder, dra_iattribute_builder, G_TYPE_OBJECT);

static void dra_iattribute_builder_default_init (DraIAttributeBuilderInterface *iface) {
	iface->createCopy = NULL;
}


gboolean dra_iattribute_builder_equal(const DraIAttributeBuilder *ab_a, const DraIAttributeBuilder *ab_b) {
	if (ab_a==ab_b) {
		return TRUE;
	} else if (ab_a==NULL || ab_b==NULL) {
		return FALSE;
	}
	return DRA_IATTRIBUTE_BUILDER_GET_INTERFACE(ab_a)->equal(ab_a, ab_b);
}
