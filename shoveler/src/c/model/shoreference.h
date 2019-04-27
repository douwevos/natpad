/*
   File:    shoreference.h
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
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

#ifndef SHOREFERENCE_H_
#define SHOREFERENCE_H_

#include "shoivalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_REFERENCE              (sho_reference_get_type())
#define SHO_REFERENCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_reference_get_type(), ShoReference))
#define SHO_REFERENCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_REFERENCE, ShoReferenceClass))
#define SHO_IS_REFERENCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_REFERENCE))
#define SHO_IS_REFERENCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_REFERENCE))
#define SHO_REFERENCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_REFERENCE, ShoReferenceClass))

typedef struct _ShoReference               ShoReference;
typedef struct _ShoReferencePrivate        ShoReferencePrivate;
typedef struct _ShoReferenceClass          ShoReferenceClass;


struct _ShoReference {
	GObject parent;
};

struct _ShoReferenceClass {
	GObjectClass parent_class;
};


GType sho_reference_get_type();

ShoReference *sho_reference_new(CatStringWo *ref_id);
ShoReference *sho_reference_deep_copy(ShoReference *source);

CatStringWo *sho_reference_get_id(ShoReference *reference);

void sho_reference_set_value(ShoReference *reference, ShoIValue *value);
ShoIValue *sho_reference_get_value(ShoReference *reference);

G_END_DECLS

#endif /* SHOREFERENCE_H_ */
