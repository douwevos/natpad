/*
   File:    leascrolledwindow.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Apr 17, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

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


#ifndef LEASCROLLEDWINDOW_H_
#define LEASCROLLEDWINDOW_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>
//#include <glib.h>


#ifdef LEA_X11_SUPPORT
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif


G_BEGIN_DECLS

#define LEA_TYPE_SCROLLED_WINDOW            (lea_scrolled_window_get_type())
#define LEA_SCROLLED_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_scrolled_window_get_type(), LeaScrolledWindow))
#define LEA_SCROLLED_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SCROLLED_WINDOW, LeaScrolledWindowClass))
#define LEA_IS_SCROLLED_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SCROLLED_WINDOW))
#define LEA_IS_SCROLLED_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SCROLLED_WINDOW))
#define LEA_SCROLLED_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SCROLLED_WINDOW, LeaScrolledWindowClass))

typedef struct _LeaScrolledWindow       LeaScrolledWindow;
typedef struct _LeaScrolledWindowClass  LeaScrolledWindowClass;


struct _LeaScrolledWindow {
	GtkBin parent;
  	GtkWidget *widget;
  	GdkWindow *parent_window;
  	GdkWindow *bin_window;
  	GdkWindow *view_window;
  	int in_scrolling;
  	GtkAdjustment *vadjustment, *hadjustment;
  	GtkWidget *vscrollbar, *hscrollbar;
  	GtkPolicyType vscrollbar_policy, hscrollbar_policy;
  	gboolean vscrollbar_visible, hscrollbar_visible;
  	guint view_width;
  	guint view_height;
  	gint view_x;
  	gint view_y;

#ifdef LEA_X11_SUPPORT
  	gboolean use_x11_scroll;
  	GC xgc;
  	gboolean xgc_created;
#endif
};


struct _LeaScrolledWindowClass {
	GtkBinClass parent_class;
};


GType lea_scrolled_window_get_type(void);

LeaScrolledWindow *lea_scrolled_window_new(GtkWidget *child);

void lea_scrolled_window_set_view_size(LeaScrolledWindow *scrolled_window, gint view_width, gint view_height);
void lea_scrolled_window_set_view_location(LeaScrolledWindow *scrolled_window, gint view_x, gint view_y);
void lea_scrolled_window_get_view_location(LeaScrolledWindow *scrolled_window, gint *view_x, gint *view_y);

void lea_scrolled_window_set_use_x11_scroll(LeaScrolledWindow *scrolled_window, gboolean x11_scroll_enabled);

G_END_DECLS

#endif /* LEASCROLLEDWINDOW_H_ */
