/*
 * leadigitpanelowner.h
 *
 *  Created on: Mar 18, 2010
 */

#ifndef LEADIGITPANELOWNER2_H_
#define LEADIGITPANELOWNER2_H_

#include <caterpillar.h>
#include "../../src/leaframe.h"
#include "../../src/action/leaactiongroup.h"

G_BEGIN_DECLS

#define LEA_TYPE_DIGIT_PANEL_OWNER            (lea_digit_panel_owner_get_type())
#define LEA_DIGIT_PANEL_OWNER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_digit_panel_owner_get_type(), LeaDigitPanelOwner))
#define LEA_DIGIT_PANEL_OWNER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_DIGIT_PANEL_OWNER, LeaDigitPanelOwnerClass))
#define LEA_IS_DIGIT_PANEL_OWNER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_DIGIT_PANEL_OWNER))
#define LEA_IS_DIGIT_PANEL_OWNER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_DIGIT_PANEL_OWNER))
#define LEA_DIGIT_PANEL_OWNER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_DIGIT_PANEL_OWNER, LeaDigitPanelOwnerClass))


typedef struct _LeaDigitPanelOwner             LeaDigitPanelOwner;

typedef struct _LeaDigitPanelOwnerClass        LeaDigitPanelOwnerClass;

struct _LeaDigitPanelOwner {
	GObject parent;
	CatArrayWo *e_panel_list;
	LeaKeyContext *key_context;
	LeaActionGroup *digit_action_group;
	LeaFrame *frame;
};

struct _LeaDigitPanelOwnerClass {
	GObjectClass parent_class;
};


GType lea_digit_panel_owner_get_type(void);

LeaDigitPanelOwner *lea_digit_panel_owner_new(LeaFrame *frame);

LeaActionGroup *lea_digit_panel_owner_get_action_group(LeaDigitPanelOwner *panel_owner);

G_END_DECLS

#endif /* LEADIGITPANELOWNER2_H_ */
