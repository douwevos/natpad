/*
 * leaipanelowner.h
 *
 *  Created on: Mar 18, 2010
 */

#ifndef LEAIPANELOWNER_H_
#define LEAIPANELOWNER_H_

#include <caterpillar.h>
#include <glib-object.h>
#include "leapanelfocusevent.h"
#include "../key/leakeycontext.h"


#define LEA_TYPE_IPANEL_OWNER                 (lea_ipanel_owner_get_type())
#define LEA_IPANEL_OWNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IPANEL_OWNER, LeaIPanelOwner))
#define LEA_IS_IPANEL_OWNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IPANEL_OWNER))
#define LEA_IPANEL_OWNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IPANEL_OWNER, LeaIPanelOwnerInterface))



typedef struct _LeaIPanelOwner               LeaIPanelOwner; /* dummy object */
typedef struct _LeaIPanelOwnerInterface      LeaIPanelOwnerInterface;

struct _LeaIPanelOwnerInterface {
	GTypeInterface parent_iface;

	void (*addPanel)(LeaIPanelOwner *self, struct _LeaPanel *panel);
	void (*removePanel)(LeaIPanelOwner *self, struct _LeaPanel *panel);
	LeaKeyContext *(*getKeyContext)(LeaIPanelOwner *self);
	GtkWidget *(*getStatusBarContent)(LeaIPanelOwner *self);

	gboolean (*panel_closing)(LeaIPanelOwner *self, struct _LeaPanel *panel);

};

GType lea_ipanel_owner_get_type(void);

gboolean lea_ipanel_owner_panel_closing(LeaIPanelOwner *self, struct _LeaPanel *panel);

void lea_ipanel_owner_add_panel(LeaIPanelOwner *self, struct _LeaPanel *panel);
void lea_ipanel_owner_remove_panel(LeaIPanelOwner *self, struct _LeaPanel *panel);

LeaKeyContext *lea_ipanel_owner_get_key_context(LeaIPanelOwner *self);

GtkWidget *lea_ipanel_owner_get_status_bar_content(LeaIPanelOwner *self);

#endif /* LEAIPANELOWNER_H_ */
