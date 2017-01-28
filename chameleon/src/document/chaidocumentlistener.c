/*
   File:    chaidocumentlistener.c
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

#include "chaidocumentlistener.h"

G_DEFINE_INTERFACE(ChaIDocumentListener, cha_idocument_listener, G_TYPE_OBJECT);

static void cha_idocument_listener_default_init (ChaIDocumentListenerInterface *iface) {
	iface->onNewRevision = NULL;
	iface->onSlotRegistered = NULL;
	iface->onEnrichmentSlotNotify = NULL;
	iface->onHistoryModified = NULL;
	iface->onNewSavedRevision = NULL;
	iface->onModeChanged = NULL;
}

