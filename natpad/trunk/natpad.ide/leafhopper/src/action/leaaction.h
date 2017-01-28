/*
   File:    leaaction.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jul 31, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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

#ifndef LEAACTION_H_
#define LEAACTION_H_


#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <caterpillar.h>
#include "leaiattachable.h"
#include "../key/leakeysequence.h"
#include "../key/leakeycontext.h"

G_BEGIN_DECLS

#define LEA_TYPE_ACTION            (lea_action_get_type())
#define LEA_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_action_get_type(), LeaAction))
#define LEA_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_ACTION, LeaActionClass))
#define LEA_IS_ACTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_ACTION))
#define LEA_IS_ACTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_ACTION))
#define LEA_ACTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_ACTION, LeaActionClass))
#define LEA_ACTION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_ACTION, LeaActionPrivate))

typedef struct _LeaAction         LeaAction;
typedef struct _LeaActionClass    LeaActionClass;
typedef struct _LeaActionPrivate  LeaActionPrivate;

struct _LeaAction {
	GObject parent;
};

struct _LeaActionClass {
	GObjectClass parent_class;
	void (*action_run)(LeaAction *self);

	/* private */
	gboolean (*calculateActualSensitivity)(LeaAction *self);
	void (*dispatchSensitivityChanged)(LeaAction *self, gboolean sensitivity);
	gboolean (*calculateActualVisibility)(LeaAction *self);
	void (*dispatchVisibilityChanged)(LeaAction *self, gboolean visibility);
	void (*refresh)(LeaAction *self);
};

GType lea_action_get_type(void);

void lea_action_construct(LeaAction *action, CatStringWo *a_name, CatStringWo *a_label, CatStringWo *a_stock_id);

CatStringWo *lea_action_get_label(LeaAction *action);
CatStringWo *lea_action_get_name(LeaAction *action);

CatStringWo *lea_action_get_stock_id(LeaAction *action);

void lea_action_set_order(LeaAction *action, int order);
int lea_action_get_order(LeaAction *action);


void lea_action_attach(LeaAction *action, LeaIAttachable *attachable);
void lea_action_detach(LeaAction *action, LeaIAttachable *attachable);

LeaKeyContext *lea_action_get_key_context(LeaAction *action);
void lea_action_set_key_context(LeaAction *action, LeaKeyContext *new_key_context);

/**
 * Set the sensitivity flag of this action. The actual sensitivity of the action is the combination ('and') of the
 * action its own sensitivity flag and that of the action-group it has been added to.
 */
void lea_action_set_sensitive_self(LeaAction *action, gboolean flag);

/**
 * Returns the sensitivity flag of this action. The actual sensitivity of the action is the combination ('and') of the
 * action its own sensitivity flag and that of the action-group it has been added to. If you want to test for the
 * actual sensitivity use lea_action_is_sensitive instead
 */
gboolean lea_action_is_sensitive_self(LeaAction *action);

gboolean lea_action_is_sensitive_group(LeaAction *action);

gboolean lea_action_is_sensitive(LeaAction *action);

gboolean lea_action_update_sensitivity(LeaAction *action);

/**
 * Set the visibility flag of this action. The actual visibility of the action is the combination ('and') of the
 * action its own visibility flag and that of the action-group it has been added to.
 *
 * The method returns TRUE when the actual visibility changed otherwise FALSE is returned.
 */
gboolean lea_action_set_visible_self(LeaAction *action, gboolean flag);

/**
 * Returns the visibility flag of this action. The actual visibility of the action is the combination ('and') of the
 * action its own visibility flag and that of the action-group it has been added to. If you want to test for the
 * actual visibility use lea_action_is_visible instead
 */
gboolean lea_action_is_visible_self(LeaAction *action);

gboolean lea_action_is_visible_group(LeaAction *action);

gboolean lea_action_is_visible(LeaAction *action);

gboolean lea_action_update_visibility(LeaAction *action);


void lea_action_set_default_key_sequence(LeaAction *action, LeaKeySequence *key_sequence);

int lea_action_compare(LeaAction *action_a, LeaAction *action_b);

G_END_DECLS

#endif /* LEAACTION_H_ */
