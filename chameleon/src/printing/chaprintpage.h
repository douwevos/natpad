/*
   File:    chaprintpage.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 6, 2015
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

#ifndef PRINTING_CHAPRINTPAGE_H_
#define PRINTING_CHAPRINTPAGE_H_

#include "chaprintline.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_PRINT_PAGE              (cha_print_page_get_type())
#define CHA_PRINT_PAGE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_print_page_get_type(), ChaPrintPage))
#define CHA_PRINT_PAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PRINT_PAGE, ChaPrintPageClass))
#define CHA_IS_PRINT_PAGE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PRINT_PAGE))
#define CHA_IS_PRINT_PAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PRINT_PAGE))
#define CHA_PRINT_PAGE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PRINT_PAGE, ChaPrintPageClass))


typedef struct _ChaPrintPage               ChaPrintPage;
typedef struct _ChaPrintPagePrivate        ChaPrintPagePrivate;
typedef struct _ChaPrintPageClass          ChaPrintPageClass;


struct _ChaPrintPage {
	GObject parent;
};

struct _ChaPrintPageClass {
	GObjectClass parent_class;
};


GType cha_print_page_get_type();

ChaPrintPage *cha_print_page_new();

void cha_print_page_append_line(ChaPrintPage *print_page, ChaPrintLine *print_line);

double cha_print_page_get_lines_view_height(ChaPrintPage *print_page);

int cha_print_page_line_count(ChaPrintPage *print_page);

ChaPrintLine *cha_print_page_line_at(ChaPrintPage *print_page, int line_idx);

G_END_DECLS

#endif /* PRINTING_CHAPRINTPAGE_H_ */
