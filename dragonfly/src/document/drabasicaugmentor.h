/*
   File:    drabasicaugmentor.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 12, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef DOCUMENT_DRABASICAUGMENTOR_H_
#define DOCUMENT_DRABASICAUGMENTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_BASIC_AUGMENTOR              (dra_basic_augmentor_get_type())
#define DRA_BASIC_AUGMENTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_basic_augmentor_get_type(), DraBasicAugmentor))
#define DRA_BASIC_AUGMENTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_BASIC_AUGMENTOR, DraBasicAugmentorClass))
#define DRA_IS_BASIC_AUGMENTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_BASIC_AUGMENTOR))
#define DRA_IS_BASIC_AUGMENTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_BASIC_AUGMENTOR))
#define DRA_BASIC_AUGMENTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_BASIC_AUGMENTOR, DraBasicAugmentorClass))

struct _DraBasicAugmentor {
	GObject parent;
};

struct _DraBasicAugmentorClass {
	GObjectClass parent_class;
};

typedef struct _DraBasicAugmentor               DraBasicAugmentor;
typedef struct _DraBasicAugmentorPrivate        DraBasicAugmentorPrivate;
typedef struct _DraBasicAugmentorClass          DraBasicAugmentorClass;


GType dra_basic_augmentor_get_type();

DraBasicAugmentor *dra_basic_augmentor_new();

G_END_DECLS

#endif /* DOCUMENT_DRABASICAUGMENTOR_H_ */
