/*
   File:    draacreplaceentry.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 17, 2015
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

#ifndef AUTOCOMPLETE_DRAACREPLACEENTRY_H_
#define AUTOCOMPLETE_DRAACREPLACEENTRY_H_

#include "draacentry.h"
#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_AC_REPLACE_ENTRY              (dra_ac_replace_entry_get_type())
#define DRA_AC_REPLACE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_ac_replace_entry_get_type(), DraAcReplaceEntry))
#define DRA_AC_REPLACE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AC_REPLACE_ENTRY, DraAcReplaceEntryClass))
#define DRA_IS_AC_REPLACE_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AC_REPLACE_ENTRY))
#define DRA_IS_AC_REPLACE_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AC_REPLACE_ENTRY))
#define DRA_AC_REPLACE_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AC_REPLACE_ENTRY, DraAcReplaceEntryClass))


typedef struct _DraAcReplaceEntry               DraAcReplaceEntry;
typedef struct _DraAcReplaceEntryPrivate        DraAcReplaceEntryPrivate;
typedef struct _DraAcReplaceEntryClass          DraAcReplaceEntryClass;


struct _DraAcReplaceEntry {
	DraAcEntry parent;
};

struct _DraAcReplaceEntryClass {
	DraAcEntryClass parent_class;
};


GType dra_ac_replace_entry_get_type();

DraAcReplaceEntry *dra_ac_replace_entry_new(CatStringWo *entry_txt, CatStringWo *replace_with);

G_END_DECLS

#endif /* AUTOCOMPLETE_DRAACREPLACEENTRY_H_ */
