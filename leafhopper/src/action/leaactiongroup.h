/*
 * leaactiongroup.h
 */

#ifndef LEAACTIONGROUP_H_
#define LEAACTIONGROUP_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "leaaction.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_ACTION_GROUP              (lea_action_group_get_type())
#define LEA_ACTION_GROUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_action_group_get_type(), LeaActionGroup))
#define LEA_ACTION_GROUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_ACTION_GROUP, LeaActionGroupClass))
#define LEA_IS_ACTION_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_ACTION_GROUP))
#define LEA_IS_ACTION_GROUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_ACTION_GROUP))
#define LEA_ACTION_GROUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_ACTION_GROUP, LeaActionGroupClass))

typedef struct _LeaActionGroup             LeaActionGroup;
typedef struct _LeaActionGroupClass        LeaActionGroupClass;
typedef struct _LeaActionGroupPrivate      LeaActionGroupPrivate;

struct _LeaActionGroup {
	LeaAction parent;
};

struct _LeaActionGroupClass {
	LeaActionClass parent_class;
};


GType lea_action_group_get_type(void);

void lea_action_group_construct(LeaActionGroup *group, CatStringWo *a_group_name, CatStringWo *label);
LeaActionGroup *lea_action_group_new(CatStringWo *a_group_name, CatStringWo *a_label);

LeaAction *lea_action_group_get(LeaActionGroup *action_group, CatStringWo *action_name);

LeaActionGroup *lea_action_group_create_sub(LeaActionGroup *action_group, CatStringWo *a_sub_group_name, CatStringWo *a_sub_label);

void lea_action_group_add(LeaActionGroup *action_group, LeaAction *action);

void lea_action_group_remove(LeaActionGroup *action_group, LeaAction *action);

CatArrayWo *lea_action_group_enlist_keys(LeaActionGroup *action_group, CatArrayWo *e_use_array);

int lea_action_group_size(LeaActionGroup *action_group);


CatStringWo *lea_action_group_get_name(LeaActionGroup *action_group);


CatArrayWo *lea_action_group_enlist_ordered(LeaActionGroup *action_group);

void lea_action_group_enlist_recursive(LeaActionGroup *action_group, CatArrayWo *e_list, gboolean include_groups);


//void lea_action_group_merge(LeaActionGroup *action_group, LeaActionGroup *merge_group);
//
//void lea_action_group_unmerge(LeaActionGroup *action_group, LeaActionGroup *merge_group);

G_END_DECLS

#endif /* LEAACTIONGROUP_H_ */
