/*
 * leaipanelowner.c
 *
 *  Created on: Mar 18, 2010
 */

#include "leaipanelowner.h"
#include "leapanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaIPanelOwner"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(LeaIPanelOwner, lea_ipanel_owner, G_TYPE_OBJECT);

static void lea_ipanel_owner_default_init(LeaIPanelOwnerInterface *iface) {
	iface->addPanel = NULL;
	iface->removePanel = NULL;
	iface->getKeyContext = NULL;
	iface->getStatusBarContent = NULL;
}

void lea_ipanel_owner_add_panel(LeaIPanelOwner *self, struct _LeaPanel *panel) {
	g_return_if_fail(LEA_IS_IPANEL_OWNER(self));
	cat_log_debug("self->add_panel=%p", LEA_IPANEL_OWNER_GET_INTERFACE(self)->addPanel);
	LEA_IPANEL_OWNER_GET_INTERFACE(self)->addPanel(self, panel);
}

void lea_ipanel_owner_remove_panel(LeaIPanelOwner *self, struct _LeaPanel *panel) {
	g_return_if_fail(LEA_IS_IPANEL_OWNER(self));
	LEA_IPANEL_OWNER_GET_INTERFACE(self)->removePanel(self, panel);
}


gboolean lea_ipanel_owner_panel_closing(LeaIPanelOwner *self, LeaPanel *panel) {
	g_return_val_if_fail(LEA_IS_IPANEL_OWNER(self), FALSE);
	return LEA_IPANEL_OWNER_GET_INTERFACE(self)->panel_closing(self, panel);
}

LeaKeyContext *lea_ipanel_owner_get_key_context(LeaIPanelOwner *self) {
	g_return_val_if_fail(LEA_IS_IPANEL_OWNER(self), NULL);
	LeaIPanelOwnerInterface *iface = LEA_IPANEL_OWNER_GET_INTERFACE(self);
	if (iface->getKeyContext) {
		return iface->getKeyContext(self);
	}
	return NULL;
}

GtkWidget *lea_ipanel_owner_get_status_bar_content(LeaIPanelOwner *self) {
	g_return_val_if_fail(LEA_IS_IPANEL_OWNER(self), NULL);
	LeaIPanelOwnerInterface *iface = LEA_IPANEL_OWNER_GET_INTERFACE(self);
	if (iface->getStatusBarContent) {
		return iface->getStatusBarContent(self);
	}
	return NULL;
}
