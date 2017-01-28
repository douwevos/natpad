/*
   File:    astcompletionentry.h
   Project: astarte
   Author:  Douwe Vos
   Date:    Sep 18, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef ELKCOMPLETIONENTRY_H_
#define ELKCOMPLETIONENTRY_H_


#include <caterpillar.h>
#include <leafhopper.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ELK_TYPE_COMPLETION_ENTRY            (elk_completion_entry_get_type())
#define ELK_COMPLETION_ENTRY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_completion_entry_get_type(), ElkCompletionEntry))
#define ELK_COMPLETION_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_COMPLETION_ENTRY, ElkCompletionEntryClass))
#define ELK_IS_COMPLETION_ENTRY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_COMPLETION_ENTRY))
#define ELK_IS_COMPLETION_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_COMPLETION_ENTRY))
#define ELK_COMPLETION_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_COMPLETION_ENTRY, ElkCompletionEntryClass))

typedef struct _ElkCompletionEntry       ElkCompletionEntry;
typedef struct _ElkCompletionEntryClass  ElkCompletionEntryClass;


struct _ElkCompletionEntry {
	GObject parent;
	CatStringWo *a_text;
};


struct _ElkCompletionEntryClass {
	GObjectClass parent_class;
	void (*run)(ElkCompletionEntry *entry, LeaPanel *editor_panel);
};


GType elk_completion_entry_get_type(void);

ElkCompletionEntry *elk_completion_entry_new(CatStringWo *a_text);
void elk_completion_entry_construct(ElkCompletionEntry *entry, CatStringWo *a_text);


void elk_completion_entry_paint(GtkWidget *widget, ElkCompletionEntry *entry, cairo_t *cairo, PangoLayout *pango_layout, int ypos, int entry_height, gboolean selected);
int elk_completion_entry_get_width(ElkCompletionEntry *entry, PangoLayout *pango_layout);

void elk_completion_entry_run(ElkCompletionEntry *entry, LeaPanel *editor_panel);


G_END_DECLS


#endif /* ELKCOMPLETIONENTRY_H_ */
