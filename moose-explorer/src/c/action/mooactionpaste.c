#include "mooactionpaste.h"
#include <viper.h>
#include "../moonodelayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooActionPaste"
#include <logging/catlog.h>

struct _MooActionPastePrivate {
	MooClipboard *moo_clipboard;
	MooNodeLayout *destination;
	gulong gtk_clip_signal;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooActionPaste, moo_action_paste, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);
static void l_clipboard_owner_changed(GtkClipboard *clipboard, GdkEvent *event, gpointer user_data);

static void moo_action_paste_class_init(MooActionPasteClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void moo_action_paste_init(MooActionPaste *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooActionPaste *instance = MOO_ACTION_PASTE(object);
	MooActionPastePrivate *priv = moo_action_paste_get_instance_private(instance);
	if (priv->moo_clipboard && priv->gtk_clip_signal!=-1) {
		GtkClipboard *clipboard = moo_clipboard_get_gtk_clipboard(priv->moo_clipboard);
		g_signal_handler_disconnect(clipboard, priv->gtk_clip_signal);
		priv->gtk_clip_signal=-1;
	}
	cat_unref_ptr(priv->moo_clipboard);
	cat_unref_ptr(priv->destination);
	G_OBJECT_CLASS(moo_action_paste_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_action_paste_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooActionPaste *moo_action_paste_new(MooClipboard *moo_clipboard) {
	MooActionPaste *result = g_object_new(MOO_TYPE_ACTION_PASTE, NULL);
	cat_ref_anounce(result);
	MooActionPastePrivate *priv = moo_action_paste_get_instance_private(result);
	priv->moo_clipboard = cat_ref_ptr(moo_clipboard);
	priv->destination = NULL;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("moose.clipboard.paste"), cat_string_wo_new_with("_Paste resources"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_data("Ctrl+V")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);

	GtkClipboard *clipboard = moo_clipboard_get_gtk_clipboard(moo_clipboard);
	priv->gtk_clip_signal = g_signal_connect(clipboard, "owner_change", G_CALLBACK(l_clipboard_owner_changed), result);
	return result;
}

static void l_update_sensitivity(MooActionPaste *action_paste) {
	MooActionPastePrivate *priv = moo_action_paste_get_instance_private(action_paste);
	gboolean sensitive = FALSE;
	if (priv->destination!=NULL) {
		MooNodeWo *node = moo_node_layout_get_node(priv->destination);
		MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
		if (resource_content) {
			VipNode *vip_node = moo_resource_content_wo_get_viper_node(resource_content);
			if (vip_node) {
				VipIResource *vip_res = (VipIResource *) vip_node_get_content(vip_node);
				if (VIP_IS_IMAP(vip_res)) {
					sensitive = moo_clipboard_contains_resources(priv->moo_clipboard);
				}
			}
		}
	}

	lea_action_set_sensitive_self(LEA_ACTION(action_paste), sensitive);
}

static void l_clipboard_owner_changed(GtkClipboard *clipboard, GdkEvent *event, gpointer user_data) {
	MooActionPaste *action_paste = MOO_ACTION_PASTE(user_data);
	l_update_sensitivity(action_paste);
}

void moo_action_paste_set_selection(MooActionPaste *action_paste, gboolean has_resource, CatArrayWo *selection) {
	MooActionPastePrivate *priv = moo_action_paste_get_instance_private(action_paste);
	MooNodeLayout *new_destination = NULL;
	if (has_resource && selection!=NULL) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_array_wo_get_first(selection);
		if (node_layout) {
			MooNodeWo *node = moo_node_layout_get_node(node_layout);
			MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
			if (resource_content) {
				VipNode *vip_node = moo_resource_content_wo_get_viper_node(resource_content);
				if (vip_node) {
					VipIResource *vip_res = (VipIResource *) vip_node_get_content(vip_node);
					if (VIP_IS_IMAP(vip_res)) {
						new_destination = node_layout;
					}
				}
			}

		}
	}
	cat_ref_swap(priv->destination, new_destination);
	l_update_sensitivity(action_paste);
}

static void l_action_run(LeaAction *self) {
	MooActionPaste *instance = MOO_ACTION_PASTE(self);
	MooActionPastePrivate *priv = moo_action_paste_get_instance_private(instance);
	if (priv->destination == NULL) {
		return;
	}
	MooNodeWo *node = moo_node_layout_get_node(priv->destination);
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
	if (resource_content) {
		CatReadableTreeNode *tree_vip_node = moo_resource_content_wo_get_node(resource_content);
		moo_clipboard_paste_resources(priv->moo_clipboard, tree_vip_node);
	}
}
