/*
   File:    draacentry.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 17, 2015
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

#ifndef AUTOCOMPLETE_DRAACENTRY_H_
#define AUTOCOMPLETE_DRAACENTRY_H_

#include "../draeditor.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_AC_ENTRY              (dra_ac_entry_get_type())
#define DRA_AC_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_ac_entry_get_type(), DraAcEntry))
#define DRA_AC_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AC_ENTRY, DraAcEntryClass))
#define DRA_IS_AC_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AC_ENTRY))
#define DRA_IS_AC_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AC_ENTRY))
#define DRA_AC_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AC_ENTRY, DraAcEntryClass))


typedef struct _DraAcEntry               DraAcEntry;
typedef struct _DraAcEntryPrivate        DraAcEntryPrivate;
typedef struct _DraAcEntryClass          DraAcEntryClass;


struct _DraAcContext;

struct _DraAcEntry {
	GObject parent;
};

struct _DraAcEntryClass {
	GObjectClass parent_class;
	void (*run)(DraAcEntry *entry, struct _DraAcContext *ac_context);
};


GType dra_ac_entry_get_type();

void dra_ac_entry_construct(DraAcEntry *entry, CatStringWo *entry_text);

DraAcEntry *dra_ac_entry_new(CatStringWo *entry_text);

CatStringWo *dra_ac_entry_get_filter_text(DraAcEntry *entry);

int dra_ac_entry_get_width(DraAcEntry *entry, PangoLayout *pango_layout);

void dra_ac_entry_paint(DraAcEntry *entry, GtkWidget *widget, cairo_t *cairo, PangoLayout *pango_layout, int ypos, int entry_height, gboolean selected);

void dra_ac_entry_run(DraAcEntry *entry, struct _DraAcContext *ac_context);

G_END_DECLS

#endif /* AUTOCOMPLETE_DRAACENTRY_H_ */
