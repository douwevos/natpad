/*
   File:    chalinemultireplace.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 25, 2015
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

#ifndef CHALINEMULTIREPLACE_H_
#define CHALINEMULTIREPLACE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_LINE_MULTI_REPLACE              (cha_line_multi_replace_get_type())
#define CHA_LINE_MULTI_REPLACE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_line_multi_replace_get_type(), ChaLineMultiReplace))
#define CHA_LINE_MULTI_REPLACE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_LINE_MULTI_REPLACE, ChaLineMultiReplaceClass))
#define CHA_IS_LINE_MULTI_REPLACE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_LINE_MULTI_REPLACE))
#define CHA_IS_LINE_MULTI_REPLACE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_LINE_MULTI_REPLACE))
#define CHA_LINE_MULTI_REPLACE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_LINE_MULTI_REPLACE, ChaLineMultiReplaceClass))


typedef struct _ChaLineMultiReplace               ChaLineMultiReplace;
typedef struct _ChaLineMultiReplacePrivate        ChaLineMultiReplacePrivate;
typedef struct _ChaLineMultiReplaceClass          ChaLineMultiReplaceClass;

typedef struct _ChaLineMultiReplaceEntry ChaLineMultiReplaceEntry;

struct _ChaLineMultiReplaceEntry {
	CatStringWo *replacement;
	int start_offset;
	int end_offset;
};


struct _ChaLineMultiReplace {
	GObject parent;
};

struct _ChaLineMultiReplaceClass {
	GObjectClass parent_class;
};


GType cha_line_multi_replace_get_type();

ChaLineMultiReplace *cha_line_multi_replace_new();

void cha_line_multi_replace_append(ChaLineMultiReplace *multi_replace, CatStringWo *replacement, int start_offset, int end_offset);

int cha_line_multi_replace_entry_count(const ChaLineMultiReplace *multi_replace);
const ChaLineMultiReplaceEntry *cha_line_multi_replace_entry_at(const ChaLineMultiReplace *multi_replace, int index);

G_END_DECLS

#endif /* CHALINEMULTIREPLACE_H_ */
