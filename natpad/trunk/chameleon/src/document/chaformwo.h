/*
   File:    chaformwo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 4, 2015
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

#ifndef DOCUMENT_CHAFORMWO_H_
#define DOCUMENT_CHAFORMWO_H_

#include "chacursormwo.h"
#include "chaformfieldwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_FORM_WO              (cha_form_wo_get_type())
#define CHA_FORM_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_form_wo_get_type(), ChaFormWo))
#define CHA_FORM_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_FORM_WO, ChaFormWoClass))
#define CHA_IS_FORM_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_FORM_WO))
#define CHA_IS_FORM_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_FORM_WO))
#define CHA_FORM_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_FORM_WO, ChaFormWoClass))


typedef struct _ChaFormWo               ChaFormWo;
typedef struct _ChaFormWoPrivate        ChaFormWoPrivate;
typedef struct _ChaFormWoClass          ChaFormWoClass;


struct _ChaFormWo {
	CatWo parent;
};

struct _ChaFormWoClass {
	CatWoClass parent_class;
};


GType cha_form_wo_get_type();

ChaFormWo *cha_form_wo_new();

void cha_form_wo_insert(ChaFormWo *e_form, ChaCursorMWo *cursor, ChaCursorMWo *new_cursor_placement);

void cha_form_wo_remove(ChaFormWo *e_form, ChaCursorMWo *rem_start, ChaCursorMWo *rem_end);

void cha_form_wo_create_field(ChaFormWo *e_form, ChaCursorMWo *c_start_cursor, ChaCursorMWo *c_end_cursor, int field_index, gboolean editable);

ChaFormFieldWo *cha_form_wo_get_at(const ChaFormWo *form, int index);

ChaFormFieldWo *cha_form_wo_editable_field_at(ChaFormWo *e_form, int index);

int cha_form_wo_field_count(const ChaFormWo *form);

int cha_form_wo_get_index_of_field_at_location(const ChaFormWo *form, const ChaCursorMWo *cursor);

int cha_form_wo_get_first_index_of_field_at_row(const ChaFormWo *form, long long row);

int cha_form_wo_get_index_of_next_field(const ChaFormWo *form, int current_index);

CAT_WO_BASE_H(ChaFormWo,cha_form_wo);

G_END_DECLS

#endif /* DOCUMENT_CHAFORMWO_H_ */
