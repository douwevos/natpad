/*
   File:    shoreferencelist.h
   Project: shoveler
   Author:  Douwe Vos
   Date:    Oct 5, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef SHOREFERENCELIST_H_
#define SHOREFERENCELIST_H_

#include "shoreference.h"
#include "shosimplevalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_REFERENCE_LIST              (sho_reference_list_get_type())
#define SHO_REFERENCE_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_reference_list_get_type(), ShoReferenceList))
#define SHO_REFERENCE_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_REFERENCE_LIST, ShoReferenceListClass))
#define SHO_IS_REFERENCE_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_REFERENCE_LIST))
#define SHO_IS_REFERENCE_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_REFERENCE_LIST))
#define SHO_REFERENCE_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_REFERENCE_LIST, ShoReferenceListClass))


typedef struct _ShoReferenceList               ShoReferenceList;
typedef struct _ShoReferenceListPrivate        ShoReferenceListPrivate;
typedef struct _ShoReferenceListClass          ShoReferenceListClass;


struct _ShoReferenceList {
	GObject parent;
};

struct _ShoReferenceListClass {
	GObjectClass parent_class;
};


GType sho_reference_list_get_type();

void sho_reference_list_construct(ShoReferenceList *reference_list);

void sho_reference_list_construct_with(ShoReferenceList *reference_list, CatArrayWo *list_to_copy);


void sho_reference_list_add_reference(ShoReferenceList *reference_list, ShoReference *reference);

/**
 * Add or replace the reference. The first reference with the same id as the provided reference is
 * replaced with the latter. If the id was not found then the reference is simply added.
 */
void sho_reference_list_set_reference(ShoReferenceList *reference_list, ShoReference *reference);


ShoReference *sho_reference_list_get_first_reference_by_id(ShoReferenceList *reference_list, CatStringWo *ref_id);

ShoSimpleValue *sho_reference_list_get_simple_value(ShoReferenceList *reference_list, CatStringWo *ref_id);

CatArrayWo *sho_reference_list_enlist_reference_by_id(ShoReferenceList *reference_list, CatStringWo *ref_id);

void sho_reference_list_set_string(ShoReferenceList *reference_list, CatStringWo *ref_id, CatStringWo *string_val);

void sho_reference_list_set_boolean(ShoReferenceList *reference_list, CatStringWo *ref_id, gboolean val);
gboolean sho_reference_list_get_boolean(ShoReferenceList *reference_list, CatStringWo *ref_id, gboolean default_value);

void sho_reference_list_set_int(ShoReferenceList *reference_list, CatStringWo *ref_id, int val);
int sho_reference_list_get_int(ShoReferenceList *reference_list, CatStringWo *ref_id, int default_value);

CatStringWo *sho_reference_list_get_string(ShoReferenceList *reference_list, CatStringWo *ref_id, CatStringWo *default_value);


int sho_reference_list_count(ShoReferenceList *reference_list);

CatIIterator *sho_reference_list_iterator(ShoReferenceList *reference_list);

G_END_DECLS

#endif /* SHOREFERENCELIST_H_ */
