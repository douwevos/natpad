/*
   File:    draiattributebuilder.h
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

#ifndef ATTR_DRAIATTRIBUTEBUILDER_H_
#define ATTR_DRAIATTRIBUTEBUILDER_H_

#include <chameleon.h>
#include <caterpillar.h>

#define DRA_TYPE_IATTRIBUTE_BUILDER                 (dra_iattribute_builder_get_type())
#define DRA_IATTRIBUTE_BUILDER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), DRA_TYPE_IATTRIBUTE_BUILDER, DraIAttributeBuilder))
#define DRA_IS_IATTRIBUTE_BUILDER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), DRA_TYPE_IATTRIBUTE_BUILDER))
#define DRA_IATTRIBUTE_BUILDER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), DRA_TYPE_IATTRIBUTE_BUILDER, DraIAttributeBuilderInterface))


typedef struct _DraIAttributeBuilder               DraIAttributeBuilder; /* dummy object */
typedef struct _DraIAttributeBuilderInterface      DraIAttributeBuilderInterface;


struct _DraIAttributeBuilderInterface {
	GTypeInterface parent_iface;
	DraIAttributeBuilder *(*createCopy)(DraIAttributeBuilder *self);
	void (*setBeginAndEnd)(DraIAttributeBuilder *self, int begin_idx, int end_idx);
	void (*getBeginAndEnd)(DraIAttributeBuilder *self, int *begin_idx, int *end_idx);
	PangoAttribute *(*build)(DraIAttributeBuilder *self, ChaPrefsColorMapWo *color_map);
	gboolean (*equal)(const DraIAttributeBuilder *ab_a, const DraIAttributeBuilder *ab_b);
};

GType dra_iattribute_builder_get_type(void);

gboolean dra_iattribute_builder_equal(const DraIAttributeBuilder *ab_a, const DraIAttributeBuilder *ab_b);

#endif /* ATTR_DRAIATTRIBUTEBUILDER_H_ */
