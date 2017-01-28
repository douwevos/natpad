/*
   File:    chacursormwo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 10, 2015
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

#ifndef DOCUMENT_CHACURSORMWO_H_
#define DOCUMENT_CHACURSORMWO_H_

#include "chacursor.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_CURSORM_WO              (cha_cursorm_wo_get_type())
#define CHA_CURSORM_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_cursorm_wo_get_type(), ChaCursorMWo))
#define CHA_CURSORM_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_CURSORM_WO, ChaCursorMWoClass))
#define CHA_IS_CURSORM_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_CURSORM_WO))
#define CHA_IS_CURSORM_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_CURSORM_WO))
#define CHA_CURSORM_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_CURSORM_WO, ChaCursorMWoClass))


typedef struct _ChaCursorMWo               ChaCursorMWo;
typedef struct _ChaCursorMWoPrivate        ChaCursorMWoPrivate;
typedef struct _ChaCursorMWoClass          ChaCursorMWoClass;


struct _ChaCursorMWo {
	CatWo parent;
};

struct _ChaCursorMWoClass {
	CatWoClass parent_class;
};


GType cha_cursorm_wo_get_type();

ChaCursorMWo *cha_cursorm_wo_new(long long row, int column);

int cha_cursorm_wo_get_column(const ChaCursorMWo *cursor);
long long cha_cursorm_wo_get_row(const ChaCursorMWo *cursor);

const char *cha_cursorm_wo_order_text(ChaCursorOrder order);

ChaCursorOrder cha_cursorm_wo_order(const ChaCursorMWo *cursor, const ChaCursorMWo *other);
int cha_cursorm_wo_hash(const ChaCursorMWo *cursor);

void cha_cursorm_wo_set_column(ChaCursorMWo *e_cursor, int column);
void cha_cursorm_wo_set_row(ChaCursorMWo *e_cursor, long long row);

void cha_cursorm_wo_move(ChaCursorMWo *e_cursor, long long delta_row, int delta_column);

CAT_WO_BASE_H(ChaCursorMWo,cha_cursorm_wo);

G_END_DECLS

#endif /* DOCUMENT_CHACURSORMWO_H_ */
