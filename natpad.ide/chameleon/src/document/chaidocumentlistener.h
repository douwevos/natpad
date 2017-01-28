/*
   File:    chaidocumentlistener.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 31, 2015
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

#ifndef CHAIDOCUMENTLISTENER_H_
#define CHAIDOCUMENTLISTENER_H_

#include "charevisionwo.h"
#include <caterpillar.h>

#define CHA_TYPE_IDOCUMENT_LISTENER                 (cha_idocument_listener_get_type())
#define CHA_IDOCUMENT_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CHA_TYPE_IDOCUMENT_LISTENER, ChaIDocumentListener))
#define CHA_IS_IDOCUMENT_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CHA_TYPE_IDOCUMENT_LISTENER))
#define CHA_IDOCUMENT_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CHA_TYPE_IDOCUMENT_LISTENER, ChaIDocumentListenerInterface))


typedef struct _ChaIDocumentListener               ChaIDocumentListener; /* dummy object */
typedef struct _ChaIDocumentListenerInterface      ChaIDocumentListenerInterface;

typedef struct _ChaModeInfo ChaModeInfo;

struct _ChaModeInfo {
	gboolean read_only;
	gboolean big_file_mode;
};

struct _ChaIDocumentListenerInterface {
	GTypeInterface parent_iface;
	void (*onNewRevision)(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision);
	void (*onSlotRegistered)(ChaIDocumentListener *self, ChaRevisionWo *a_revision, ChaEnrichmentDataMapWo *a_map, GObject *slot_key, int slot_index);
	void (*onEnrichmentSlotNotify)(ChaIDocumentListener *self, ChaRevisionWo *a_revision, GObject *slot_key, gpointer user_data);
	void (*onHistoryModified)(ChaIDocumentListener *self, int history_index, int history_length);
	void (*onNewSavedRevision)(ChaIDocumentListener *self, ChaRevisionWo *a_revision);
	void (*onModeChanged)(ChaIDocumentListener *self, const ChaModeInfo *mode_info);
};

GType cha_idocument_listener_get_type(void);

#endif /* CHAIDOCUMENTLISTENER_H_ */
