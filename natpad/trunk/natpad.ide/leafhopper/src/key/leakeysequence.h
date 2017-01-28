/*
   File:    leakeysequence.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 30, 2013
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

#ifndef LEAKEYSEQUENCE_H_
#define LEAKEYSEQUENCE_H_

#include "leakey.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_KEY_SEQUENCE              (lea_key_sequence_get_type())
#define LEA_KEY_SEQUENCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_key_sequence_get_type(), LeaKeySequence))
#define LEA_KEY_SEQUENCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_KEY_SEQUENCE, LeaKeySequenceClass))
#define LEA_IS_KEY_SEQUENCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_KEY_SEQUENCE))
#define LEA_IS_KEY_SEQUENCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_KEY_SEQUENCE))
#define LEA_KEY_SEQUENCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_KEY_SEQUENCE, LeaKeySequenceClass))
#define LEA_KEY_SEQUENCE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_KEY_SEQUENCE, LeaKeySequencePrivate))


typedef struct _LeaKeySequence               LeaKeySequence;
typedef struct _LeaKeySequencePrivate        LeaKeySequencePrivate;
typedef struct _LeaKeySequenceClass          LeaKeySequenceClass;


struct _LeaKeySequence {
	GInitiallyUnowned parent;
	LeaKeySequencePrivate *priv;
};

struct _LeaKeySequenceClass {
	GInitiallyUnownedClass parent_class;
};


GType lea_key_sequence_get_type();

LeaKeySequence *lea_key_sequence_new(LeaKey *firstKey);

LeaKeySequence *lea_key_sequence_new_with(LeaKey *firstKey, ...);

LeaKeySequence *lea_key_sequence_from_string(const CatStringWo *ca_txt);

CatStringWo *lea_key_sequence_to_string(LeaKeySequence *sequence);

CatIIterator *lea_key_sequence_iterator(LeaKeySequence *sequence);

int lea_key_sequence_hash(LeaKeySequence *sequence);

gboolean lea_key_sequence_equal(LeaKeySequence *sequence_a, LeaKeySequence *sequence_b);

G_END_DECLS

#endif /* LEAKEYSEQUENCE_H_ */
