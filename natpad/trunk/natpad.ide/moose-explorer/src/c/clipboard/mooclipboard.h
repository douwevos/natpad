/*
   File:    mooclipboard.h
   Project: monkey-explorer
   Author:  Douwe Vos
   Date:    Jul 19, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef MOOCLIPBOARD_H_
#define MOOCLIPBOARD_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include <moose.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_CLIPBOARD              (moo_clipboard_get_type())
#define MOO_CLIPBOARD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_clipboard_get_type(), MooClipboard))
#define MOO_CLIPBOARD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_CLIPBOARD, MooClipboardClass))
#define MOO_IS_CLIPBOARD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_CLIPBOARD))
#define MOO_IS_CLIPBOARD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_CLIPBOARD))
#define MOO_CLIPBOARD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_CLIPBOARD, MooClipboardClass))
#define MOO_CLIPBOARD_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_CLIPBOARD, MooClipboardPrivate))

typedef struct _MooClipboard               MooClipboard;
typedef struct _MooClipboardPrivate        MooClipboardPrivate;
typedef struct _MooClipboardClass          MooClipboardClass;


struct _MooClipboard {
	GObject parent;
	MooClipboardPrivate *priv;
};

struct _MooClipboardClass {
	GObjectClass parent_class;
};


GType moo_clipboard_get_type();

MooClipboard *moo_clipboard_new(MooService *moo_service, LeaFrame *frame);

GtkClipboard *moo_clipboard_get_gtk_clipboard(MooClipboard *moo_clipboard);

void moo_clipboard_paste_resources(MooClipboard *moo_clipboard, CatReadableTreeNode *destination_vip);

void moo_clipboard_cut_or_copy(MooClipboard *moo_clipboard, CatArrayWo *selected_nodes, gboolean is_copy);

gboolean moo_clipboard_contains_resources(MooClipboard *moo_clipboard);



void moo_clipboard_set_selection(MooClipboard *moo_clipboard, CatArrayWo *selected);

G_END_DECLS


#endif /* MOOCLIPBOARD_H_ */
