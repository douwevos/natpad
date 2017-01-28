/*
   File:    chauow.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 28, 2015
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

#ifndef CHAUOW_H_
#define CHAUOW_H_

#include "../layout/chadocumentview.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW              (cha_uow_get_type())
#define CHA_UOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_get_type(), ChaUow))
#define CHA_UOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW, ChaUowClass))
#define CHA_IS_UOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW))
#define CHA_IS_UOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW))
#define CHA_UOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW, ChaUowClass))


typedef struct _ChaUow               ChaUow;
typedef struct _ChaUowPrivate        ChaUowPrivate;
typedef struct _ChaUowClass          ChaUowClass;

struct _ChaEditor;

struct _ChaUow {
	GObject parent;
};

struct _ChaUowClass {
	GObjectClass parent_class;
	void (*run)(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);
};


GType cha_uow_get_type();

void cha_uow_construct(ChaUow *uow, gboolean is_reading_only);

void cha_uow_run(ChaUow *uow, struct _ChaEditor *editor, ChaDocumentView *document_view);

void cha_uow_get_first_and_last_row(ChaDocumentView *document_view, ChaLineLocationWo **first_ll, ChaLineLocationWo **last_ll, int *valid_end_column);

G_END_DECLS

#endif /* CHAUOW_H_ */
