/*
   File:    dratagoverviewbar.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 19, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef DRATAGOVERVIEWBAR_H_
#define DRATAGOVERVIEWBAR_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_TAG_OVERVIEW_BAR              (dra_tag_overview_bar_get_type())
#define DRA_TAG_OVERVIEW_BAR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_tag_overview_bar_get_type(), DraTagOverviewBar))
#define DRA_TAG_OVERVIEW_BAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_TAG_OVERVIEW_BAR, DraTagOverviewBarClass))
#define DRA_IS_TAG_OVERVIEW_BAR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_TAG_OVERVIEW_BAR))
#define DRA_IS_TAG_OVERVIEW_BAR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_TAG_OVERVIEW_BAR))
#define DRA_TAG_OVERVIEW_BAR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_TAG_OVERVIEW_BAR, DraTagOverviewBarClass))


typedef struct _DraTagOverviewBar               DraTagOverviewBar;
typedef struct _DraTagOverviewBarPrivate        DraTagOverviewBarPrivate;
typedef struct _DraTagOverviewBarClass          DraTagOverviewBarClass;


struct _DraTagOverviewBar {
	GtkWidget parent;
};

struct _DraTagOverviewBarClass {
	GtkWidgetClass parent_class;
};


GType dra_tag_overview_bar_get_type();

DraTagOverviewBar *dra_tag_overview_bar_new();

void dra_tag_overview_bar_set_tagged_lines(DraTagOverviewBar *bar, CatIntArrayWo *tagged_lines, int total_line_count);


G_END_DECLS

#endif /* DRATAGOVERVIEWBAR_H_ */
