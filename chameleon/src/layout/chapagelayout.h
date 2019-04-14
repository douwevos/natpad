/*
   File:    chapagelayout.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 22, 2015
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

#ifndef CHAPAGELAYOUT_H_
#define CHAPAGELAYOUT_H_

#include "../document/chalinewo.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CHA_TYPE_PAGE_LAYOUT              (cha_page_layout_get_type())
#define CHA_PAGE_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_page_layout_get_type(), ChaPageLayout))
#define CHA_PAGE_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PAGE_LAYOUT, ChaPageLayoutClass))
#define CHA_IS_PAGE_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PAGE_LAYOUT))
#define CHA_IS_PAGE_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PAGE_LAYOUT))
#define CHA_PAGE_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PAGE_LAYOUT, ChaPageLayoutClass))


typedef struct _ChaPageLayout               ChaPageLayout;
typedef struct _ChaPageLayoutPrivate        ChaPageLayoutPrivate;
typedef struct _ChaPageLayoutClass          ChaPageLayoutClass;

typedef struct _ChaPageLayoutContext ChaPageLayoutContext;

struct _ChaPageLayoutContext {
	GtkRequisition requisition;
	int text_layout_width;
	int page_version;
	short font_version;
	int settings_version;
	ChaLineEnd line_ends;
	gboolean line_ends_are_mixed;
};

struct _ChaPageLayout {
	GObject parent;
};

struct _ChaPageLayoutClass {
	GObjectClass parent_class;
};


GType cha_page_layout_get_type();

ChaPageLayout *cha_page_layout_new();

int cha_page_layout_get_height(ChaPageLayout *page_view);
int cha_page_layout_get_width(ChaPageLayout *page_view);

int cha_page_layout_get_page_version(ChaPageLayout *page_view);

gboolean cha_page_layout_versions_start_update(ChaPageLayout *page_view, int page_version, int text_layout_width, short font_version);
gboolean cha_page_layout_version_is_updating(ChaPageLayout *page_view, int page_version, int text_layout_width, short font_version);

gboolean cha_page_layout_apply_update_result(ChaPageLayout *e_page_view, ChaPageLayoutContext *context);


G_END_DECLS

#endif /* CHAPAGELAYOUT_H_ */
