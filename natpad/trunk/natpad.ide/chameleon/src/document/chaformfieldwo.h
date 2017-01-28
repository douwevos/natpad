/*
   File:    chaformfieldwo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 7, 2015
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

#ifndef DOCUMENT_CHAFORMFIELDWO_H_
#define DOCUMENT_CHAFORMFIELDWO_H_

#include "chacursormwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_FORM_FIELD_WO              (cha_form_field_wo_get_type())
#define CHA_FORM_FIELD_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_form_field_wo_get_type(), ChaFormFieldWo))
#define CHA_FORM_FIELD_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_FORM_FIELD_WO, ChaFormFieldWoClass))
#define CHA_IS_FORM_FIELD_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_FORM_FIELD_WO))
#define CHA_IS_FORM_FIELD_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_FORM_FIELD_WO))
#define CHA_FORM_FIELD_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_FORM_FIELD_WO, ChaFormFieldWoClass))


typedef struct _ChaFormFieldWo               ChaFormFieldWo;
typedef struct _ChaFormFieldWoPrivate        ChaFormFieldWoPrivate;
typedef struct _ChaFormFieldWoClass          ChaFormFieldWoClass;

typedef enum _ChaFormFieldCoverage ChaFormFieldCoverage;


enum _ChaFormFieldCoverage {
										/*  []=form <>=other       */
	CHA_FFC_NO_OVERLAP=0,				/*  [..]....<..>....       */
	CHA_FFC_OVERLAPS_FULL,	            /*  ....[.<..>.]....       */
	CHA_FFC_IS_OVERLAPPED_FULL,         /*  ....<.[..].>....       */
	CHA_FFC_LEFT_OVERLAPPED,            /*  ..<..[..>..]....       */
	CHA_FFC_RIGHT_OVERLAPPED,           /*  ..[..<..]..>....       */
	CHA_FFC_SAME_SAME                   /*  ..[<....>]......       */
};

struct _ChaFormFieldWo {
	CatWo parent;
};

struct _ChaFormFieldWoClass {
	CatWoClass parent_class;
};


GType cha_form_field_wo_get_type();

ChaFormFieldWo *cha_form_field_wo_new(ChaCursorMWo *a_start_cursor, ChaCursorMWo *a_end_cursor, int field_index, gboolean editable);

ChaCursorMWo *cha_form_field_wo_get_start(const ChaFormFieldWo *field);
ChaCursorMWo *cha_form_field_wo_get_end(const ChaFormFieldWo *field);
int cha_form_field_wo_get_field_index(const ChaFormFieldWo *field);

gboolean cha_form_field_wo_on_row(const ChaFormFieldWo *field, int row);
gboolean cha_form_field_wo_test_cursor_in_field(const ChaFormFieldWo *field, const ChaCursorMWo *cursor);
ChaFormFieldCoverage cha_form_field_wo_test_coverage(const ChaFormFieldWo *field, const ChaCursorMWo *other_start, const ChaCursorMWo *other_end);

gboolean cha_form_field_wo_is_marked_as_modified(const ChaFormFieldWo *field);
void cha_form_field_wo_mark_as_modified(ChaFormFieldWo *field);


void cha_form_field_wo_set_start_and_end(ChaFormFieldWo *e_field, ChaCursorMWo *start_cursor, ChaCursorMWo *end_cursor);

int cha_form_field_wo_hash(const ChaFormFieldWo *field);

CAT_WO_BASE_H(ChaFormFieldWo,cha_form_field_wo);

G_END_DECLS

#endif /* DOCUMENT_CHAFORMFIELDWO_H_ */
