/*
   File:    chalineiterator.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 16, 2015
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

#ifndef DOCUMENT_CHALINEITERATOR_H_
#define DOCUMENT_CHALINEITERATOR_H_

#include "charevisionwo.h"
#include "chalinelocationwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_LINE_ITERATOR              (cha_line_iterator_get_type())
#define CHA_LINE_ITERATOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_line_iterator_get_type(), ChaLineIterator))
#define CHA_LINE_ITERATOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_LINE_ITERATOR, ChaLineIteratorClass))
#define CHA_IS_LINE_ITERATOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_LINE_ITERATOR))
#define CHA_IS_LINE_ITERATOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_LINE_ITERATOR))
#define CHA_LINE_ITERATOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_LINE_ITERATOR, ChaLineIteratorClass))


typedef struct _ChaLineIterator               ChaLineIterator;
typedef struct _ChaLineIteratorPrivate        ChaLineIteratorPrivate;
typedef struct _ChaLineIteratorClass          ChaLineIteratorClass;


struct _ChaLineIterator {
	GObject parent;
};

struct _ChaLineIteratorClass {
	GObjectClass parent_class;
};


GType cha_line_iterator_get_type();

ChaLineIterator *cha_line_iterator_new(ChaRevisionWo *revision, unsigned long long start_row);

ChaLineIterator *cha_line_iterator_new_line_location(ChaRevisionWo *revision, ChaLineLocationWo *ll);

G_END_DECLS

#endif /* DOCUMENT_CHALINEITERATOR_H_ */
