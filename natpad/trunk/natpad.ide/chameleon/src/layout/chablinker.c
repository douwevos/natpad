/*
   File:    chablinker.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 26, 2015
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

#include "chablinker.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaBlinker"
#include <logging/catlog.h>

struct _ChaBlinkerPrivate {
	GThread *thread;
	CatLock *lock;
	volatile gboolean stop_request;
	volatile gboolean stop_response;
	ChaEditor *active_editor;
	ChaEditor *blinking_editor;
	uint64_t last_active_time;
	gboolean blinker_on_off;
	volatile gboolean is_active;
};

static void l_uow_iface_init(ChaIUowListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaBlinker, cha_blinker, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaBlinker)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IUOW_LISTENER, l_uow_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_blinker_class_init(ChaBlinkerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_blinker_init(ChaBlinker *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaBlinker *instance = CHA_BLINKER(object);
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(instance);
	if (priv->lock) {
		cha_blinker_request_shutdown(instance);

		int64_t end = cat_date_current_time()+30000;
		gboolean keep_waiting = TRUE;
		while(keep_waiting) {
			cat_lock_lock(priv->lock);
			if (priv->stop_response) {
				keep_waiting = FALSE;
			} else {
				cat_lock_wait_timed(priv->lock, 1000);
				if (cat_date_current_time()>=end) {
					keep_waiting = FALSE;
				}
			}
			cat_lock_unlock(priv->lock);
		}
	}

	if (priv->stop_response) {
		cat_unref_ptr(priv->active_editor);
		cat_unref_ptr(priv->lock);
	}
	G_OBJECT_CLASS(cha_blinker_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_blinker_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void *l_blinker_run(void *ptr_to_self);

ChaBlinker *cha_blinker_new() {
	ChaBlinker *result = g_object_new(CHA_TYPE_BLINKER, NULL);
	cat_ref_anounce(result);
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(result);
	priv->lock = cat_lock_new();
	priv->active_editor = NULL;
	priv->blinking_editor = NULL;
	priv->stop_request = FALSE;
	priv->stop_response = FALSE;
	priv->thread = g_thread_new("AST editor blinker", l_blinker_run, result); // enable dispose stuff aswell
	priv->is_active = TRUE;
	return result;
}



static gboolean l_repaint_blink_line(gpointer blinker_as_pointer) {
	cat_log_detail("idle paint !!!");
	ChaBlinker *blinker = CHA_BLINKER(blinker_as_pointer);
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(blinker);
	if (priv->stop_request || priv->lock==NULL) {
		cat_unref_ptr(blinker);
		return FALSE;
	}
	cat_lock_lock(priv->lock);
	cat_log_detail("locked");
	if (priv->blinking_editor!=priv->active_editor || priv->blinking_editor==NULL) {
		cat_lock_unlock(priv->lock);
		cat_log_detail("unlocked");
		cat_unref_ptr(blinker);
		return FALSE;
	}

	ChaEditor *ast_editor = cat_ref_ptr(priv->blinking_editor);
	cat_lock_unlock(priv->lock);
	cat_log_detail("unlocked");
	gboolean do_paint = TRUE;

//	if (do_paint) {
//		if (!lea_panel_is_active(LEA_PANEL(ast_editor))) {
//			do_paint = FALSE;
//		}
//	}

	if (do_paint && ast_editor) {
		ChaDocumentView *document_view = cha_editor_get_document_view(ast_editor);
		if (document_view) {
			cha_document_view_invalidate_lines(document_view);
		}
	}
	cat_unref_ptr(ast_editor);
	cat_unref_ptr(blinker);
	return FALSE;
}



static void *l_blinker_run(void *ptr_to_self) {
	ChaBlinker *blinker = CHA_BLINKER(ptr_to_self);
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(blinker);

	uint64_t last_blink_time = 0;
	priv->blinker_on_off = TRUE;

	uint64_t time_out = 700;

	cat_lock_lock(priv->lock);

	while(!priv->stop_request) {
		uint64_t now = cat_date_current_time();
		if (priv->active_editor!=priv->blinking_editor) {
			priv->blinking_editor = priv->active_editor;
//			if (blinking_editor) {
//				ast_editor_add_uow_listener(blinking_editor, AST_IUOW_LISTENER(blinker));
//			}
			priv->last_active_time = now;
		}

		if (priv->last_active_time>last_blink_time || !priv->is_active) {
			last_blink_time = now;
			priv->blinker_on_off = TRUE;
		} else if (last_blink_time+time_out<=now) {
			last_blink_time = now;
			priv->blinker_on_off = !priv->blinker_on_off;
		}


		int64_t time_to_wait = last_blink_time+time_out - now;
		if (time_to_wait<50) {
			time_to_wait = 50;
		}

		if (priv->blinking_editor) {
			if (cha_editor_set_cursor_blink_up(priv->blinking_editor, priv->blinker_on_off)) {
				cat_log_debug("adding idle");
				cat_ref_ptr(blinker);
				g_idle_add(l_repaint_blink_line, blinker);

//				ast_editor_repaint_after_hook_update(blinking_editor);
			}
		}


		cat_log_debug("time_to_wait=%ld, blinker_on_off=%d, blinking_editor=%p", time_to_wait, priv->blinker_on_off, priv->blinking_editor);

		cat_lock_wait_timed(priv->lock, time_to_wait);
	}

	priv->stop_response = TRUE;
	cat_lock_notify_all(priv->lock);
	cat_lock_unlock(priv->lock);
	return NULL;
}



void cha_blinker_set_active_editor(ChaBlinker *blinker, ChaEditor *active_editor) {
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(blinker);
	cat_log_debug("active_editor=%p, priv->active_editor=%p", active_editor, priv->active_editor);
	ChaEditor *old_active_editor = active_editor;
	cat_lock_lock(priv->lock);
	if (priv->active_editor!=active_editor) {
		old_active_editor = priv->active_editor;
		priv->active_editor = cat_ref_ptr(active_editor);
	}
	cat_lock_unlock(priv->lock);
	cat_log_debug("active_editor=%p, old_active_editor=%p", active_editor, old_active_editor);
	if (old_active_editor!=active_editor) {
		if (old_active_editor) {
			cha_editor_set_cursor_blink_up(old_active_editor, TRUE);

//			ast_editor_remove_uow_listener(old_active_editor, AST_IUOW_LISTENER(blinker));
			cha_editor_remove_uow_listener(active_editor, CHA_IUOW_LISTENER(blinker));
		}
		cat_unref_ptr(old_active_editor);
		if (active_editor) {
			cha_editor_add_uow_listener(active_editor, CHA_IUOW_LISTENER(blinker));
			ChaPreferencesWo *prefs = cha_editor_get_preferences(active_editor);
			if (prefs) {
				priv->is_active = cha_preferences_wo_get_blink_cursor(prefs);
			}
		}
	}
}

void cha_blinker_request_shutdown(ChaBlinker *blinker) {
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(blinker);
	if (priv->lock) {
		cat_lock_lock(priv->lock);
		priv->stop_request = TRUE;
		cat_lock_notify_all(priv->lock);
		cat_lock_unlock(priv->lock);
	} else {
		cat_log_warn("lock was already released");
		priv->stop_request = TRUE;
	}
}

void cha_blinker_inform_activity(ChaBlinker *blinker) {
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(blinker);
	cat_lock_lock(priv->lock);
	priv->last_active_time = cat_date_current_time();
	if (!priv->blinker_on_off) {
		cat_lock_notify(priv->lock);
	}
	cat_lock_unlock(priv->lock);

}

void cha_blinker_set_blinker_active(ChaBlinker *blinker, gboolean active) {
	cat_log_detail("activ = %d", active);
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(blinker);
	priv->is_active = active;
}


/********************* start ChaIUowListener implementation *********************/

static void l_uow_event(ChaIUowListener *self, ChaUow *uow, ChaDocumentView *document_view, ChaDocument *document) {
	cha_blinker_inform_activity(CHA_BLINKER(self));
}


static void l_uow_iface_init(ChaIUowListenerInterface *iface) {
	iface->uowEvent = l_uow_event;
}

/********************* end ChaIUowListener implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaBlinker *instance = CHA_BLINKER(self);
	ChaBlinkerPrivate *priv = cha_blinker_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p active_editor=%p]", iname, self, priv->active_editor);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
