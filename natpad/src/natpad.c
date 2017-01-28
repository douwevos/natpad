/*
   File:    natpad.c
   Project: natpad
   Author:  Douwe Vos
   Date:    Oct 29, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#include "natpad.h"
#include "natpad-app.h"

#include <profiling/catstacktracer.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "Natpad"
#include <logging/catlog.h>

#define NATPAD_CLEAN_EXIT 1

static void my_g_log_default_handler(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
//	cat_log_fatal("%s(level:%d) %s", log_domain, log_level, message);
	cat_stacktrace_print();
//
//
//#ifdef NATPAD_TESTING
//
//#ifdef MONITOR_CLAZZ
//	cat_ref_report();
//	cat_ref_summary();
//	cat_ref_cleanup();
//#endif
//
//	cat_ref_entry_report();
//#endif
//
}



int main(int argc, char **argv) {

//	putenv("UBUNTU_MENUPROXY=");	// otherwise it crashes on Ubuntu
	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gdk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gdk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gtk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler(NULL, G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler(NULL, G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
//
//	daemon(-1,-1);

	NatpadApp *natpad_app = natpad_app_new();
	int status = g_application_run((GApplication *) natpad_app, argc, argv);
	cat_unref_ptr(natpad_app);

#ifdef NATPAD_TESTING

#ifdef MONITOR_CLAZZ
	cat_ref_report();
	cat_ref_summary();
	cat_ref_cleanup();
#endif

	cat_ref_entry_report();
#endif

	cat_log_info("Goodbye");


	return status;
}

